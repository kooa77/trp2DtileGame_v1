#include "GameSystem.h"

#include "Texture.h"
#include "Frame.h"


Frame::Frame() :
	_frameDelay(0.0f)
{
}

Frame::~Frame()
{

}

void Frame::Init(Texture* texture, int x, int y, int width, int height, float rotate, float frameDelay)
{
	_sprite = GameSystem::GetInstance().GetSprite();
	_texture = texture;

	_width = width;
	_height = height;

	_rotate = rotate;

	// 소스이미지에서 출력에 사용될 영역을 지정한다.
	_srcTextureRect.left = x;
	_srcTextureRect.top = y;
	_srcTextureRect.right = x + _width;
	_srcTextureRect.bottom = y + _height;

	_textureColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	_frameDelay = frameDelay;
}

void Frame::Deinit()
{

}

void Frame::Render()
{
	// 스프라이트 출력전에 출력 모양을 조정한다.
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)_width/ 2.0f, (float)_height / 2.0f);	// 텍스쳐 피벗 조정. 중심으로
	D3DXVECTOR2 translate = D3DXVECTOR2(_x - (float)_width / 2.0f, _y - (float)_height / 2.0f);		// 이동 벡터. 찍힐 위치 조정
	D3DXVECTOR2 scaling = D3DXVECTOR2(1.0f, 1.0f);		// 크기 조정

														// 변환 행렬 생성
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,	// 행렬
		NULL,	// 크기를 조정할 때, 기준을 왼쪽 상단으로 유지
		0.0f,	// 회전 없음
		&scaling,	// 크기 조정 값
		&spriteCenter,	// 회전 중심
		//0.0f,		// 회전 각도
		_rotate,
		&translate	// x, y 위치
	);

	// 생성된 행렬 적용
	_sprite->SetTransform(&matrix);

	_sprite->Draw(_texture->GetTextureDX(), &_srcTextureRect, NULL, NULL, _textureColor);
}

void Frame::Release()
{

}

void Frame::Reset()
{

}

float Frame::GetFrameDelay()
{
	return _frameDelay;
}

void Frame::SetPosition(float x, float y)
{
	_x = x;
	_y = y;
}
