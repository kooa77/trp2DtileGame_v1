#pragma once

#include "Character.h"

class NPC : public Character
{
public:
	NPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename);
	~NPC();

	// Message
public:
	void ReceiveMessage(const sComponentMsgParam& msgParam);

	// AI
public:
	void UpdateAI(float deltaTime);
};
