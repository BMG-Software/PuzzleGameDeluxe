
#ifndef TIMER_H_
#define TIMER_H_

#include <SDL.h>

#include <cstdint>

class Timer
{


public:


	Timer();


	void StartTimer();


	void RestartTimer();


	void StopTimer();


	Uint32 GetTimeMs();


	float GetTimeSeconds();


	void PauseTimer();


	bool IsStarted();


	bool IsPaused();


	bool IsStopped();


private:


	// Records ticks at start point.
	Uint32 start;


	// Records ticks at stop point.
	Uint32 stop;


	// Records ticks at the pause point.
	Uint32 paused;


	// Tracks whether the timer has started.
	bool is_started;


	// Tracks whether the timer is paused.
	bool is_paused;


	// Tracks whether the timer has stopped.
	bool is_stopped;


};


#endif // TIMER_H_