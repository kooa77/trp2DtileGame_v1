#pragma once

#include "State.h"

class DeadState : public State
{
public:
	DeadState();
	~DeadState();

protected:

public:
	void Init(Character* character);
	void Update(float deltaTime);
	void Render();

	void Reset();
	void Release();

	void Start();
	void Stop();

	void CreateSprite();
};

