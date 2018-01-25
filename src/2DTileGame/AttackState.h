#pragma once

#include "State.h"

class AttackState : public State
{
public:
	AttackState();
	~AttackState();

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
