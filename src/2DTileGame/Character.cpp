#include <list>

#include "ComponentSystem.h"
#include "ComponentMessage.h"

#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Sprite.h"
#include "Font.h"

#include "MoveState.h"
#include "IdleState.h"
#include "AttackState.h"
#include "DefenseState.h"
#include "DeadState.h"
#include "MeetState.h"

#include "Character.h"

Character::Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename) : Component(name)
{
	_state = NULL;
	_moveTime = 1.0f;

	_scriptFilename = scriptName;
	_textrueFilename = textrueFilename;

	_attackPoint = 10;
	_hp = 100;

	_attackCooltime = 1.0f;
	_attackCooltimeDuration = 0.0f;
}

Character::~Character()
{
	delete _font;
}

void Character::Init()
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	int tileX = rand() % (map->GetWidth() - 1) + 1;
	int tileY = rand() % (map->GetHeight() - 1) + 1;
	while (1)
	{
		if (true == map->CanMoveTileMap(tileX, tileY))
		{
			break;
		}
		tileX = rand() % (map->GetWidth() - 1) + 1;
		tileY = rand() % (map->GetHeight() - 1) + 1;
	}
	Init(tileX, tileY);
}

void Character::Init(int tileX, int tileY)
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	
	_tileX = tileX;
	_tileY = tileY;
		
	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);
	map->SetTileComponent(_tileX, _tileY, this, false);

	InitMove();

	InitState();
	ChangeState(eStateType::ET_IDLE);

	D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0);
	_font = new Font(L"Arial", 15, color);

	_font->SetRect(100, 100, 400, 100);
	UpdateText();
}

void Character::InitTilePosition(int tileX, int tileY)
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();

	map->ResetTileComponent(_tileX, _tileY, this);

	_tileX = tileX;
	_tileY = tileY;

	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);
	map->SetTileComponent(_tileX, _tileY, this, false);
}

void Character::Deinit()
{
	std::map<eStateType, State*>::iterator it = _stateMap.begin();
	while (it != _stateMap.end())
	{
		State* state = it->second;
		delete state;
		it++;
	}
	_stateMap.clear();
}

void Character::Update(float deltaTime)
{
	UpdateAttackCooltime(deltaTime);
	_state->Update(deltaTime);

	UpdateText();
}

void Character::Render()
{
	_state->Render();

	_font->SetPosition(_x-200, _y-30);
	_font->Render();
}

void Character::Release()
{
	_state->Release();
}

void Character::Reset()
{
	_state->Reset();
}

void Character::DecreaseHP(int decreaseHP)
{
	_hp -= decreaseHP;
	if (_hp < 0)
	{
		_isLive = false;
		_state->NextState(eStateType::ET_DEAD);
	}
}

void Character::IncreaseHP(int increaseHP)
{
	_hp += increaseHP;
	if (100 < _hp)
	{
		_hp = 100;
	}
}

void Character::SetPosition(float posX, float posY)
{
	_x = posX;
	_y = posY;
}

void Character::MoveDeltaPosition(float deltaX, float deltaY)
{
	_x += deltaX;
	_y += deltaY;
}

void Character::UpdateAI(float deltaTime)
{
	_currentDirection = (eDirection)(rand() % 4);
	_state->NextState(eStateType::ET_MOVE);
}

void Character::InitState()
{
	ReplaceState(eStateType::ET_IDLE, new IdleState());
	ReplaceState(eStateType::ET_MOVE, new MoveState());
	ReplaceState(eStateType::ET_ATTACK, new AttackState());
	ReplaceState(eStateType::ET_MEET, new MeetState());
	ReplaceState(eStateType::ET_DEFENSE, new DefenseState());
	ReplaceState(eStateType::ET_DEAD, new DeadState());
}

void Character::ReplaceState(eStateType changeType, State* replaceState)
{
	std::map<eStateType, State*>::iterator it =
		_stateMap.find(changeType);
	if (it != _stateMap.end())
	{
		delete it->second;
		_stateMap.erase(changeType);
	}

	State* state = replaceState;
	state->Init(this);
	_stateMap[changeType] = state;
}

void Character::ChangeState(eStateType stateType)
{
	if (NULL != _state)
	{
		_state->Stop();
	}	
	_state = _stateMap[stateType];
	_state->Start();
}

void Character::InitMove()
{
	_isMoving = false;
	_currentDirection = eDirection::DOWN;
}

void Character::MoveStart(int newTileX, int newTileY)
{
	_isMoving = true;

	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	map->ResetTileComponent(_tileX, _tileY, this);
	_tileX = newTileX;
	_tileY = newTileY;

	// 자연스러운 이동을 위한 보간
	{
		map->SetTileComponent(_tileX, _tileY, this, false);

		// 이동거리
		_targetX = map->GetPositionX(_tileX, _tileY);
		_targetY = map->GetPositionY(_tileX, _tileY);

		float distanceX = _targetX - _x;
		float distanceY = _targetY - _y;

		// 최소 이동거리
		_moveDistancePerTimeX = distanceX / _moveTime;
		_moveDistancePerTimeY = distanceY / _moveTime;
	}
}

void Character::MoveStop()
{
	_isMoving = false;

	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);;

	_moveDistancePerTimeX = 0.0f;
	_moveDistancePerTimeY = 0.0f;
}

void Character::Moving(float deltaTime)
{
	float moveDistanceX = _moveDistancePerTimeX * deltaTime;
	float moveDistanceY = _moveDistancePerTimeY * deltaTime;
	_x += moveDistanceX;
	_y += moveDistanceY;
}

Component* Character::Collision(std::list<Component*>& collisionList)
{
	return NULL;
}

void Character::ReceiveMessage(const sComponentMsgParam& msgParam)
{
	if (L"Attack" == msgParam.message)
	{
		_attackedPoint = msgParam.attackPoint;
		_state->NextState(eStateType::ET_DEFENSE);
	}
}

void Character::UpdateAttackCooltime(float deltaTime)
{
	if (_attackCooltimeDuration < _attackCooltime)
	{
		_attackCooltimeDuration += deltaTime;
	}
	else
	{
		_attackCooltimeDuration = _attackCooltime;
	}
}

bool Character::IsAttackCooltime()
{
	if (_attackCooltime <= _attackCooltimeDuration)
		return true;
	return false;
}

void Character::ResetAttackCooltime()
{
	_attackCooltimeDuration = 0.0f;
}

void Character::UpdateText()
{
	int coolTime = (int)(_attackCooltimeDuration * 1000.0f);

	WCHAR text[256];
	wsprintf(text, L"HP %d\nAttack %d", _hp, coolTime);
	_font->SetText(text);
}

void Character::PushPathTileCell(TileCell* tileCell)
{
	_pathTileCellStack.push(tileCell);
}

void Character::ClearPathTileCellStack()
{
	while (0 != _pathTileCellStack.size())
	{
		_pathTileCellStack.pop();
	}
}