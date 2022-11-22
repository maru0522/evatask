#include "Player.h"
#include <cassert>
void Player::Initialize(Camera* camera)
{
	assert(camera);

	this->camera = camera;

	for (size_t i = 0; i < gunbitnum; i++)
	{
		gunbit[i].worldCoordinate_.scale_ = { 1.0f,1.0f,1.5f };
	}
	nannka[0] = { 0.5f,1,0 };
	nannka[1] = { -0.5f,1,0 };
	nannka[2] = { 1,0,0 };
	nannka[3] = { -1,0,0 };

	moveVec = { 0,0,0 };

	move_speed = 0.3f;

	hopper_dash = false;

	cooltime = 0;

	timer = 0;

	NormalTimer = 0;

	bitmovetimer = 60;

	lockmove = false;


	hopper_speed = 5.0f;

	cooldown = false;

	B_bottan = false;
	old_B_bottan = false;

	latetime = 0;

	hoppertimer = 0;

	hozon = { 0,0,0 };

	height = Window::GetInstance()->height_;
	width  = Window::GetInstance()->width_;

}

void Player::Update(Camera viewProjection)
{

	moveVec = { 0,0,0 };


}

XMFLOAT3 Player::PlayerMoveVec()
{
	return moveVec;
}

XMFLOAT3 Player::GetWorldPosition()
{
	return player.worldCoordinate_.position_;
}

WorldCoordinate Player::GetMat()
{
	return player.worldCoordinate_;
}
