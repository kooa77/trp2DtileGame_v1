#pragma once

#include "StageLoader.h"

class PathfinderStageLoader : public StageLoader
{
public:
	PathfinderStageLoader(Stage* stage);
	~PathfinderStageLoader();

public:
	void CreateComponents(std::wstring mapName);
};
