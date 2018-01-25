#pragma once

#include "State.h"

class Character;

class CombatMoveState : public State
{
public:
	CombatMoveState();
	~CombatMoveState();

private:
	float _movingDuration;

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();
};