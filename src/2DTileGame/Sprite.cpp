#include <fstream>
#include <reader.h>

#include "ResourceManager.h"
#include "GameSystem.h"

#include "Frame.h"
#include "Texture.h"
#include "Sprite.h"


Sprite::Sprite(LPCWSTR textureFilename, LPCWSTR scriptFilename, float rotate) :
	_currentFrame(0), _frameTime(0.0f), _srcTexture(NULL),
	_textureFilename(textureFilename), _scriptFilename(scriptFilename),
	_rotate(rotate)
{
}

Sprite::~Sprite()
{
	_srcTexture = NULL;
}

void Sprite::Init()
{
	_device3d = GameSystem::GetInstance().GetDevice();
	_sprite = GameSystem::GetInstance().GetSprite();

	_srcTexture = ResourceManager::GetInstance().LoadTexture(_textureFilename);

	// json parsing
	{
		std::vector<std::string> srciptTextList = ResourceManager::GetInstance().LoadScript(_scriptFilename);
		for(int i=0 ; i<srciptTextList.size() ; i++)
		{
			std::string record = srciptTextList[i];

			Json::Value root;
			Json::Reader reader;
			bool isSuccess = reader.parse(record, root);
			if (isSuccess)
			{
				std::string texture = root["texture"].asString();
				int x = root["x"].asInt();
				int y = root["y"].asInt();
				int width = root["width"].asInt();
				int height = root["width"].asInt();
				double delay = root["framedelay"].asDouble();

				Frame* frame = new Frame();
				frame->Init(_srcTexture, x, y, width, height, _rotate, delay);
				_frameList.push_back(frame);
			}
		}
	}

	_currentFrame = 0;
	_frameTime = 0.0f;
}

void Sprite::Init(int srcX, int srcY, int width, int height, float frameDelay)
{
	_device3d = GameSystem::GetInstance().GetDevice();
	_sprite = GameSystem::GetInstance().GetSprite();

	_srcTexture = ResourceManager::GetInstance().LoadTexture(_textureFilename);

	{
		Frame* frame = new Frame();
		frame->Init(_srcTexture, srcX, srcY, width, height, _rotate,frameDelay);
		_frameList.push_back(frame);
	}

	_currentFrame = 0;
	_frameTime = 0.0f;
}

void Sprite::Deinit()
{
	for (std::vector<Frame*>::iterator it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame* frame = (*it);
		frame->Deinit();
		delete frame;
	}
	_frameList.clear();

	_srcTexture = NULL;
}

void Sprite::Update(float deltaTime)
{
	_frameTime += deltaTime;
	if (_frameList[_currentFrame]->GetFrameDelay() <= _frameTime)
	{
		_frameTime = 0.0f;
		_currentFrame = (_currentFrame + 1) % _frameList.size();
	}
}

void Sprite::Render()
{
	if (_currentFrame < _frameList.size())
	{
		_frameList[_currentFrame]->SetPosition(_x, _y);
		_frameList[_currentFrame]->Render();
	}
}


void Sprite::Release()
{
	for (std::vector<Frame*>::iterator it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame* frame = (*it);
		frame->Release();
	}
	_srcTexture->Release();
}

void Sprite::Reset()
{
	Init();
	for (std::vector<Frame*>::iterator it = _frameList.begin(); it != _frameList.end(); it++)
	{
		Frame* frame = (*it);
		frame->Reset();
	}
}

void Sprite::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}
