

#ifndef TIMER_H
#define TIMER_H

class Timer {
public:

	Timer()
		: start_ticks(0),
		  paused_ticks(0),
		  paused(false),
		  started(false)
	{}

	void start() {
		started = true;
		paused = false;
		start_ticks = SDL_GetTicks();
		paused_ticks = 0;
	}

	void stop() {
		started = false;
		paused = false;
		start_ticks = 0;
		paused_ticks = 0;
	}

	void pause() {
	    if(started && !paused) {
	        paused = true;

	        paused_ticks = SDL_GetTicks() - start_ticks;
	        start_ticks = 0;
	    }
	}

	void unpause()
	{
	    if(started && paused) {
	        paused = false;

	        start_ticks = SDL_GetTicks() - paused_ticks;
	        paused_ticks = 0;
	    }
	}

	int get_ticks() {
		int time_ = 0;
		if (started) {
			if (paused) {
				time_ = paused_ticks;
			}
			else {
				time_ = SDL_GetTicks() - start_ticks;
			}
		}
	}


private:

	int start_ticks;
	int paused_ticks;
	bool paused;
	bool started;
};


#endif