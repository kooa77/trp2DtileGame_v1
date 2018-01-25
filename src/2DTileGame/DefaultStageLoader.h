#pragma once

#include "StageLoader.h"

class DefaultStageLoader : public StageLoader
{
public:
	DefaultStageLoader(Stage* stage);
	~DefaultStageLoader();

public:
	void CreateComponents(std::wstring mapName);
};
