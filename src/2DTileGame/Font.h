#pragma once

#include <d3dx9.h>
#include <string>

class Font
{
private:
	ID3DXFont* _dxFont;
	std::wstring _text;
	D3DCOLOR _color;

	int _x, _y, _width, _height;

public:
	Font(std::wstring fontName, int fontSize, D3DCOLOR color);
	~Font();

	void Render();

	void SetRect(int x, int y, int width, int height);
	void SetPosition(int x, int y);
	void SetText(std::wstring text);
};
