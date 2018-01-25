#include "ResourceManager.h"


using namespace std;

ResourceManager* ResourceManager::_instance = 0;

// Singleton
ResourceManager& ResourceManager::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new ResourceManager();
	}
	return *_instance;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

Texture* ResourceManager::LoadTexture(std::wstring textureFilename)
{
	std::map<std::wstring, Texture*>::iterator it = _textureMap.find(textureFilename);
	if (it != _textureMap.end())
	{
		return it->second;
	}

	Texture* texture = new Texture();
	texture->Init(textureFilename.c_str());

	_textureMap[textureFilename] = texture;
	return texture;
}

vector<string> ResourceManager::LoadScript(std::wstring scriptFilename)
{
	map<std::wstring, vector<string>>::iterator it = _scriptMap.find(scriptFilename);
	if (it != _scriptMap.end())
	{
		return it->second;
	}

	vector<std::string> scriptTextList;

	char record[1000];
	ifstream infile(scriptFilename);
	while (!infile.eof())
	{
		infile.getline(record, 100);
		scriptTextList.push_back(record);
	}

	_scriptMap[scriptFilename] = scriptTextList;
	
	return scriptTextList;
}
