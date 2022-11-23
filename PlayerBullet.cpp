#include "PlayerBullet.h"

PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initlize(RailCamera* camera, const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity)
{
	//Obj3d bullet{ "Resources/3dModels/bit/bit.obj", &camera->getView() };


}

void PlayerBullet::Update()
{
}

void PlayerBullet::Draw()
{
}

void PlayerBullet::OnCollision()
{
}

XMFLOAT3 PlayerBullet::GetWorldPosition()
{
	return XMFLOAT3();
}

XMFLOAT3 PlayerBullet::GetScale()
{
	return XMFLOAT3();
}
