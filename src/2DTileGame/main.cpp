#include <time.h>
#include "GameSystem.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	srand(time(NULL));

	GameSystem::GetInstance().InitSystem(hInstance, nCmdShow);
	return GameSystem::GetInstance().Update();
}
