#pragma once


class GameTimer
{
private:
	double _secondsPerCount;
	double _deltaTime;

	__int64 _currentTime;
	__int64 _prevTime;

public:
	GameTimer();
	~GameTimer();

	float GetDeltaTime();

	void Reset();	// 업데이트 전 한번 호출
	void Update();	// 매 프레임마다 호출
};