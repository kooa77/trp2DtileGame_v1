#pragma once

#include <d3dx9.h>

class Texture
{
private:
	IDirect3DTexture9* _texture;
	D3DXIMAGE_INFO _texInfo;

public:
	Texture();
	~Texture();

	void Init(LPCWSTR fileName);
	
	void Deinit();
	void Release();

	IDirect3DTexture9* GetTextureDX();
};
