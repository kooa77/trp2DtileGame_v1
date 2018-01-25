#pragma once

#include "State.h"

class DefenseState : public State
{
public:
	DefenseState();
	~DefenseState();

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
