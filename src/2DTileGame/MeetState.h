#pragma once

#include "State.h"

class MeetState : public State
{
public:
	MeetState();
	~MeetState();

protected:

public:
	void Init(Character* character);
	void Update(float deltaTime);
	void Render();

	void Reset();
	void Release();

	void Start();
	void Stop();
};
