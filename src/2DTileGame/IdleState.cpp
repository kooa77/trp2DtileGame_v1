#include "Character.h"
#include "IdleState.h"


IdleState::IdleState()
{
}

IdleState::~IdleState()
{
}

void IdleState::Init(Character* character)
{
	State::Init(character);
}

void IdleState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->IsLive())
		return;

	if (false == _character->IsMoving())
	{
		_character->UpdateAI(deltaTime);
	}
}

void IdleState::Start()
{
	State::Start();

	_nextState = eStateType::ET_NONE;
}

void IdleState::Stop()
{
	State::Stop();
}
