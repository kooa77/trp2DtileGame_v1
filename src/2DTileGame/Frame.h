#pragma once

#include <d3dx9.h>

class Texture;

class Frame
{
private:
	LPD3DXSPRITE _sprite;
	Texture* _texture;

	RECT _srcTextureRect;
	D3DCOLOR _textureColor;

	float _x;
	float _y;
	int _width;
	int _height;
	float _rotate;

	float _frameDelay;

public:
	Frame();
	~Frame();

	void Init(Texture* texture, int x, int y, int width, int height, float rotate, float frameDelay);
	void Deinit();
	void Render();
	void Release();
	void Reset();

	float GetFrameDelay();

	void SetPosition(float x, float y);
};