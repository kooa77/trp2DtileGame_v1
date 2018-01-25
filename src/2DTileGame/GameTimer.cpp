#include <Windows.h>
#include "GameTimer.h"


GameTimer::GameTimer() :
	_secondsPerCount(0.0), _deltaTime(-1.0),
	_currentTime(0), _prevTime(0)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);		// 초당 갯수를 얻는다.
	_secondsPerCount = 1.0 / (double)countsPerSec;		// 초단위 시간을 계산한다.
}

GameTimer::~GameTimer()
{

}

float GameTimer::GetDeltaTime()
{
	return (float)_deltaTime;
}

void GameTimer::Reset()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	_prevTime = currentTime;
}

void GameTimer::Update()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);	// 64비트 정수를 이용해서 시간 값을 돌려준다. 시간을 개수 단위로 측정. 현재 시간을 얻는다.
	_currentTime = currentTime;

	_deltaTime = (_currentTime - _prevTime) * _secondsPerCount;	// 경과시간 계산

	_prevTime = _currentTime;

	if (_deltaTime < 0.0)
	{
		_deltaTime = 0.0;
	}
}

