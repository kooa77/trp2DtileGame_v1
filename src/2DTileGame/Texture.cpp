#include "GameSystem.h"

#include "Texture.h"


Texture::Texture()
{
	_texture = NULL;
}

Texture::~Texture()
{

}

void Texture::Init(LPCWSTR fileName)
{
	// 파일로 부터 이미지의 폭과 높이를 얻는다.
	//D3DXIMAGE_INFO texInfo;	// 멤버 변수화
	HRESULT hr = D3DXGetImageInfoFromFile(fileName, &_texInfo);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed D3DXGetImageInfoFromFile", 0, 0);
		return;
	}

	// 파일로부터 텍스쳐를 로드해 생성한다.
	hr = D3DXCreateTextureFromFileEx(GameSystem::GetInstance().GetDevice(), fileName,
		_texInfo.Width, _texInfo.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		//D3DCOLOR_ARGB(255, 44, 133, 133),
		D3DCOLOR_ARGB(255, 255, 255, 255),
		&_texInfo,
		NULL,
		&_texture
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed D3DXCreateTextureFromFileEx", 0, 0);
		return;
	}
}

void Texture::Deinit()
{
	RELEASE_COM(_texture);
}

void Texture::Release()
{
	RELEASE_COM(_texture);
}

IDirect3DTexture9* Texture::GetTextureDX()
{
	return _texture;
}