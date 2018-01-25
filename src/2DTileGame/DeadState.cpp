#include "Sprite.h"
#include "ComponentSystem.h"
#include "Character.h"

#include "DeadState.h"


DeadState::DeadState()
{
}

DeadState::~DeadState()
{
}

void DeadState::Init(Character* character)
{
	State::Init(character);
}

void DeadState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}
}

void DeadState::Render()
{
	State::Render();
}

void DeadState::Reset()
{
	State::Reset();
}

void DeadState::Release()
{
	State::Release();
}

void DeadState::Start()
{
	State::Start();
}

void DeadState::Stop()
{
	State::Stop();
}

void DeadState::CreateSprite()
{
	_spriteList.clear();

	WCHAR textrureFilename[256];
	wsprintf(textrureFilename, L"%s.png", _character->GetTextureFilename().c_str());

	WCHAR scriptFilename[256];
	{
		wsprintf(scriptFilename, L"%s_dead_left.json", _character->GetScriptFilename().c_str());
		Sprite* sprite = new Sprite(textrureFilename, scriptFilename, 1.5f);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFilename, L"%s_dead_right.json", _character->GetScriptFilename().c_str());
		Sprite* sprite = new Sprite(textrureFilename, scriptFilename, 1.5f);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFilename, L"%s_dead_up.json", _character->GetScriptFilename().c_str());
		Sprite* sprite = new Sprite(textrureFilename, scriptFilename, 1.5f);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
	{
		wsprintf(scriptFilename, L"%s_dead_down.json", _character->GetScriptFilename().c_str());
		Sprite* sprite = new Sprite(textrureFilename, scriptFilename, 1.5f);
		sprite->Init();
		_spriteList.push_back(sprite);
	}
}
