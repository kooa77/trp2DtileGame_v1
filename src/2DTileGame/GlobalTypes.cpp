#include "GlobalTypes.h"


TilePosition GetNextTilePosition(TilePosition curPos, eDirection direction)
{
	TilePosition tilePosition = curPos;
	switch (direction)
	{
	case eDirection::LEFT:	// left
		tilePosition.x--;
		break;
	case eDirection::RIGHT:	// right
		tilePosition.x++;
		break;
	case eDirection::UP:	// up
		tilePosition.y--;
		break;
	case eDirection::DOWN:	// down
		tilePosition.y++;
		break;
	}
	return tilePosition;
}

eDirection GetDirection(TilePosition to, TilePosition from)
{
	if (from.x < to.x)
	{
		return eDirection::RIGHT;
	}
	else if (to.x < from.x)
	{
		return eDirection::LEFT;
	}
	else if (from.y < to.y)
	{
		return eDirection::DOWN;
	}
	else if (to.y < from.y)
	{
		return eDirection::UP;
	}

	return eDirection::NONE;
}
