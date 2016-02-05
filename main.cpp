
#include <SDL/SDL.h>

#include <array>

const int SCREEN_W = 1024;
const int SCREEN_H = 768;
const int SCREEN_BPP = 32;

const int CELL_SIZE = 16;
const int USER_GRID_W = SCREEN_W / CELL_SIZE;
const int USER_GRID_H = SCREEN_H / CELL_SIZE;

class GolGrid {
public:
	GolGrid() 
		: running(false),
		  mouse_click(false) {
		reset_grid();
	}

	void input_reset_grid(SDL_Event event) {
		if (event.key.keysym.sym == SDLK_r) {
			reset_grid();
		}
	}

	void input_start_stop(SDL_Event event) {
		if (event.key.keysym.sym == SDLK_SPACE) {
			running = !running;
		}
	}

	void click_grid(SDL_Event event) {
		int x, y;
		if (event.button.type == SDL_MOUSEBUTTONDOWN) {
			mouse_click = true;
		}
		else if (event.button.type == SDL_MOUSEBUTTONUP) {
			mouse_click = false;
		}


		if (mouse_click) {
			x = event.button.x;
			y = event.button.y;

			int index = USER_GRID_W * int(y / CELL_SIZE) + int(x / CELL_SIZE);

			if (!current_grid[index])
				current_grid[index] = true;
			else ;
				//current_grid[index] = false;

		}
	}


	void run_gol() {

		if (running) {

			for (int i = 0; i < USER_GRID_W * USER_GRID_H; ++i) {

				if (current_grid[i]) {
					if (check_adjacent_cells(i) < 2) {
						next_grid[i] = false; // die - too little population
					}
					else if ((check_adjacent_cells(i) == 2 || check_adjacent_cells(i) == 3)) {
						next_grid[i] = true; // remains - good conditions
					}
					else if (current_grid[i] && check_adjacent_cells(i) > 3) {
						next_grid[i] = false; // die - overpopulation
					}
				}
				else if (!current_grid[i] && check_adjacent_cells(i) == 3) {
					next_grid[i] = true; // life - good conditons to generate life
				}
			}
		}
	}


	void render_grid() {

		int x = 0;
		int y = 0;

		SDL_Rect box;
		box.x = x;
		box.y = y;
		box.w = CELL_SIZE;
		box.h = CELL_SIZE;


		for (auto& element : current_grid) {

			if (element) {
				SDL_FillRect(SDL_GetVideoSurface(), &box, 0x00FF00);
			}

			x += CELL_SIZE;
			if (x >= SCREEN_W) {
				x = 0;
				y += CELL_SIZE;
			}
			box.x = x;
			box.y = y;
		}
	}
	

	void swap_grids() {
		if (running) {
			std::copy(std::begin(next_grid), std::end(next_grid), std::begin(current_grid));
		}
	}

private:


	int check_adjacent_cells(const int cell_index) const {

		int alive_neighbours = 0;

		// sides

		if (current_grid[cell_index - 1])
			++alive_neighbours;
		if (current_grid[cell_index + 1])
			++alive_neighbours;

		// below diagonals
		if (current_grid[cell_index - 64 - 1])
			++alive_neighbours;
		if (current_grid[cell_index - 64 + 1])
			++alive_neighbours;

		// below
		if (current_grid[cell_index - 64])
			++alive_neighbours;


		/// above diagonals
		if (current_grid[cell_index + 64 - 1])
			++alive_neighbours;
		if (current_grid[cell_index + 64 + 1])
			++alive_neighbours;

		// above
		if (current_grid[cell_index + 64])
			++alive_neighbours;


		return alive_neighbours;
	}

	void reset_grid() {
		for (unsigned i = 0; i < current_grid.size(); ++i) {
			current_grid[i] = false;
			next_grid[i] = false;
		}
	}

	bool running;
	bool mouse_click;

	std::array<bool, USER_GRID_W * USER_GRID_H> current_grid;
	std::array<bool, USER_GRID_W * USER_GRID_H> next_grid;

};


int main(int argc, char** args) {

	SDL_Init(SDL_INIT_VIDEO);

	SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP, SDL_SWSURFACE);

	SDL_Event event;

	bool done = false;

	GolGrid grid;

	while (!done) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				done = true;
		}

		grid.click_grid(event);
		grid.input_reset_grid(event);
		grid.input_start_stop(event);

		grid.run_gol();

		//grid.swap_grids();

		SDL_FillRect(SDL_GetVideoSurface(), nullptr, 0x000000);
		grid.render_grid();
		SDL_Flip(SDL_GetVideoSurface());

		grid.swap_grids();

		SDL_Delay(50);
		
	}
	SDL_Quit();
}