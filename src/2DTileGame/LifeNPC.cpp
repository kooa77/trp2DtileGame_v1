#include "LifeNPC.h"


LifeNPC::LifeNPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename) :
	Character(name, scriptName, textrueFilename)
{
	_type = eComponentType::CT_NPC;
}

LifeNPC::~LifeNPC()
{
}

void LifeNPC::UpdateAI(float deltaTime)
{
}
