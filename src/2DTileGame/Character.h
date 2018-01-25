#pragma once

#include <Windows.h>
#include <map>
#include <vector>
#include <string>
#include <list>
#include <stack>
#include "GlobalTypes.h"
#include "Component.h"

enum eStateType
{
	ET_IDLE,
	ET_MOVE,
	ET_ATTACK,
	ET_MEET,
	ET_DEFENSE,
	ET_DEAD,
	ET_PATHFINDING,
	ET_NONE,
};

class Sprite;
class State;
class Font;
class TileCell;

class Character : public Component
{
private:
	std::wstring _textrueFilename;
	std::wstring _scriptFilename;
	float _x;
	float _y;

protected:
	int _hp;

public:
	Character(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename);
	virtual ~Character();

	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void Init(int tileX, int tileY);
	void InitTilePosition(int tileX, int tileY);

	std::wstring GetTextureFilename() { return _textrueFilename;  }
	std::wstring GetScriptFilename() { return _scriptFilename; }

	// Message
public:
	void ReceiveMessage(const sComponentMsgParam& msgParam);

	// Common
public:
	void DecreaseHP(int decreaseHP);
	void IncreaseHP(int increaseHP);

	// transform
public:
	void SetPosition(float posX, float posY);
	void MoveDeltaPosition(float deltaX, float deltaY);

	// AI
public:
	virtual void UpdateAI(float deltaTime);

	// State
protected:
	std::map<eStateType, State*> _stateMap;

public:
	virtual void InitState();
	void ReplaceState(eStateType changeType, State* replaceState);
	void ChangeState(eStateType stateType);

	// Move
protected:
	bool _isMoving;
	float _moveTime;
	State* _state;

	float _targetX;
	float _targetY;

	eDirection _currentDirection;

public:
	void InitMove();

	void MoveStart(int newTileX, int newTileY);
	void MoveStop();
	void Moving(float deltaTime);

	bool IsMoving() { return _isMoving;  }
	float GetMoveTime() { return _moveTime;  }

	float GetX() { return _x; }
	float GetY() { return _y; }

	eDirection GetDirection() { return _currentDirection; }
	void SetDirection(eDirection direction) { _currentDirection = direction;  }
	virtual Component* Collision(std::list<Component*>& collisionList);

	// Attack
protected:
	Component* _target;
	int _attackPoint;

	float _attackCooltime;
	float _attackCooltimeDuration;

public:
	int GetAttackPoint() { return _attackPoint;  }
	Component* GetTarget() { return _target;  }
	void SetTarget(Component* target) { _target = target;  }
	void ResetTargert() { _target = NULL;  }

	void UpdateAttackCooltime(float deltaTime);
	bool IsAttackCooltime();
	void ResetAttackCooltime();

	// Defense
protected:
	int _attackedPoint;

public:
	int GetAttackedPoint() { return _attackedPoint; }

	// UI
protected:
	Font* _font;

public:
	void Character::UpdateText();


	// Pathfinding
private:
	std::stack<TileCell*> _pathTileCellStack;

public:
	std::stack<TileCell*> GetPathTileCellStack() { return _pathTileCellStack; }
	void PushPathTileCell(TileCell* tileCell);
	void ClearPathTileCellStack();
};
