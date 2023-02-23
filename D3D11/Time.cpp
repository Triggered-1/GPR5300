#include "Time.h"

INT Time::init()
{
	_lastTimepoint = chrono::high_resolution_clock::now();

	return 0;
}

void Time::update()
{
	TimePoint now = chrono::high_resolution_clock::now();
	Duration diff = now - _lastTimepoint;
	_lastTimepoint = now;
	_deltaTime = diff.count();
}

void Time::deInit()
{
}
