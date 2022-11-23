#include "PlayerBullet.h"
#include"bossHand.h"

PlayerBullet::PlayerBullet()
{
	Velocity_ = { 0,0,0 };
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initlize(RailCamera* camera, const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity)
{
	bullet=Obj3d{ "Resources/3dModels/bit/bit.obj", camera->getView() };

	bullet.worldCoordinate_.position_ = position;

	bullet.worldCoordinate_.rotation_ = rota;

	bullet.worldCoordinate_.rotation_.z = 0;

	bullet.worldCoordinate_.scale_ = { 0.5f,0.5f,0.5f };

	Velocity_ = velocity;
}

void PlayerBullet::Update()
{
	bullet.worldCoordinate_.position_ = bullet.worldCoordinate_.position_+ Velocity_;
	bullet.Update();

	//デスタイマーをひいて0以下になったらフラグを立てる
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	bullet.Draw();
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

XMFLOAT3 PlayerBullet::GetWorldPosition()
{
	return bullet.worldCoordinate_.position_;
}

XMFLOAT3 PlayerBullet::GetScale()
{
	return bullet.worldCoordinate_.scale_;
}
