#pragma once

enum eDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE,
};

typedef struct _TilePosition
{
	int x;
	int y;
} TilePosition;

TilePosition GetNextTilePosition(TilePosition curPos, eDirection direction);
eDirection GetDirection(TilePosition to, TilePosition from);
