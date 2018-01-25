#include "CombatMoveState.h"
#include "IdleState.h"
#include "AttackState.h"
#include "DefenseState.h"
#include "DeadState.h"

#include "LifePlayer.h"


LifePlayer::LifePlayer(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) :
	Player(name, scriptName, textureFilename)
{
}

LifePlayer::~LifePlayer()
{
}

void LifePlayer::InitState()
{
	Player::InitState();
	ReplaceState(eStateType::ET_ATTACK, new IdleState());
}

