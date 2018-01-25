#include "ComponentSystem.h"
#include "Character.h"

#include "MeetState.h"


MeetState::MeetState()
{
}

MeetState::~MeetState()
{
}

void MeetState::Init(Character* character)
{
	State::Init(character);
}

void MeetState::Update(float deltaTime)
{
	State::Update(deltaTime);

	_character->ResetTargert();
	_character->ChangeState(eStateType::ET_IDLE);
}

void MeetState::Render()
{
	State::Render();
}

void MeetState::Reset()
{
	State::Reset();
}

void MeetState::Release()
{
	State::Release();
}

void MeetState::Start()
{
	State::Start();

	sComponentMsgParam msgParam;
	msgParam.sender = (Component*)_character;
	msgParam.attackPoint = _character->GetAttackPoint();
	msgParam.receiver = _character->GetTarget();
	msgParam.message = L"Meet";
	ComponentSystem::GetInstance().SendMsg(msgParam);
}

void MeetState::Stop()
{
	State::Stop();
}

