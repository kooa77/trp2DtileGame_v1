#pragma once

#include <vector>
#include <d3dx9.h>

class Frame;
class Texture;

class Sprite
{
private:
	std::vector<Frame*> _frameList;
	int _currentFrame;
	float _frameTime;

	Texture* _srcTexture;

	LPCWSTR _textureFilename;
	LPCWSTR _scriptFilename;

	float _rotate;

	float _x;
	float _y;

public:
	Sprite(LPCWSTR textureFilename, LPCWSTR scriptFilename, float rotate = 0.0f);
	~Sprite();

	void Init();
	void Init(int srcX, int srcY, int width, int height, float frameDelay);
	void Deinit();

	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	void SetPosition(float x, float y);

	// DirectX
private:
	LPDIRECT3DDEVICE9 _device3d;
	LPD3DXSPRITE _sprite;
};