#pragma once

#include <d3dx9.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "Texture.h"

class ResourceManager
{
	// Singleton
private:
	static ResourceManager* _instance;

public:
	static ResourceManager& GetInstance();

private:
	ResourceManager();		// 다른 곳에서 실수로 생성하지 않도록, 생성자를 프라이빗으로 만든다.

public:
	~ResourceManager();

	// Texture
private:
	std::map<std::wstring, Texture*> _textureMap;

public:
	Texture* LoadTexture(std::wstring textureFilename);

	// Script
private:
	std::map<std::wstring, std::vector<std::string>> _scriptMap;

public:
	std::vector<std::string> LoadScript(std::wstring scriptFilename);
};
