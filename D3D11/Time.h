#pragma once
#include <Windows.h>
#include <chrono>

using namespace std;

#define TIME_POINT chrono::high_resolution_clock::time_point
typedef chrono::high_resolution_clock::time_point TimePoint;
typedef chrono::duration<FLOAT> Duration;
using TimePoint2 = chrono::high_resolution_clock::time_point;

class Time
{
public:
	INT init();
	void update();
	void deInit();

	FLOAT getDeltaTime() const { return _deltaTime; }

private:
	FLOAT _deltaTime = 0.0f; // time between two frames

	TimePoint _lastTimepoint = {}; // time of last frame
};

