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
	
	Vector3 Flont /*= camera->getForwardVec()*/;
	Vector3 KyozouFlont = Flont;
	Flont.y = 0;

	

	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	if (Input::GetInstance()->GetJoystickState(0, joystate))
	{
		moveVec.x += (float)joystate.Gamepad.sThumbLX / SHRT_MAX;
		moveVec.z += (float)joystate.Gamepad.sThumbLY / SHRT_MAX;
	}


	if (input_->PushKey(DIK_UP))
	{
		moveVec.z = 1;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		moveVec.z = -1;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		moveVec.x = 1;
	}
	if (input_->PushKey(DIK_LEFT))
	{
		moveVec.x = -1;
	}

	//ˆÚ“®


	float p_pos = atan2(moveVec.x, moveVec.z);
	float c_vec = atan2(Flont.x, Flont.z);

	worldTransform_.rotation_.y = (p_pos + c_vec);

	Vector3 mae = { 0,0,1.0f };

	mae = worldTransform_.matWorld_.VectorMat(mae, worldTransform_.matWorld_);

	mae.normalize();

	if (!hopper_dash && !cooldown)
	{
		if (moveVec.x != 0 || moveVec.z != 0)
		{

			worldTransform_.translation_ += mae * move_speed;
		}
	}

	EnemyArrow();

	if (LockOn())
	{
		lockmove = true;
	}
	else
	{
		lockmove = false;
		timer = 0;
	}

	if (lockmove)
	{
		timer++;

		if (timer >= bitmovetimer)
		{
			timer = bitmovetimer;
		}


	}

	if (LockOn())
	{


		for (size_t i = 0; i < gunbitnum; i++)
		{
			Vector3 PtoB = BitVec - target[i].translation_;
			Vector3 kari = PtoB;
			kari.y = 0;
			float kariy = -PtoB.y;
			PtoB.normalize();
			float gomi = atan2(PtoB.x, PtoB.z);
			target[i].rotation_.y = easeOutSine(target[i].rotation_.y, gomi, timer / bitmovetimer);
			gomi = atan2(kariy, kari.length());
			kyozou.rotation_.y = gomi;
			target[i].rotation_.x = easeOutSine(target[i].rotation_.x, gomi, timer / bitmovetimer);
			kyozou.rotation_.x = gomi;
		}
		Vector3 KtoB = BitVec - worldTransform_.translation_;
		Vector3 Kkari = KtoB;
		Kkari.y = 0;
		float Kkariy = -KtoB.y;
		KtoB.normalize();
		float gomi = atan2(KtoB.x, KtoB.z);
		kyozou.rotation_.y = gomi;
		gomi = atan2(Kkariy, Kkari.length());
		kyozou.rotation_.x = gomi;

		for (size_t i = 0; i < gunbitnum; i++)
		{
			Vector3 bitpos = nannka[i];
			bitpos = kyozou.matWorld_.VectorMat(bitpos, kyozou.matWorld_);
			bitpos.normalize();
			target[i].translation_ = easeOutSineVec3(target[i].translation_, worldTransform_.translation_ + (bitpos * 2.0f), timer / bitmovetimer);
		}


	}
	else
	{
		for (size_t i = 0; i < gunbitnum; i++)
		{
			Vector3 bitpos = nannka[i];
			bitpos = worldTransform_.matWorld_.VectorMat(bitpos, worldTransform_.matWorld_);
			bitpos.normalize();
			target[i].translation_ = worldTransform_.translation_ + (bitpos * 2.0f);
		}

		for (size_t i = 0; i < gunbitnum; i++)
		{

			target[i].rotation_.y = worldTransform_.rotation_.y;
			target[i].rotation_.x = worldTransform_.rotation_.x;
		}
	}





	old_B_bottan = B_bottan;

	if (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		B_bottan = true;
	}
	else
	{
		B_bottan = false;
	}


	if ((input_->TriggerKey(DIK_Z) || (B_bottan && !old_B_bottan)) && !cooldown)
	{
		hopper_dash = true;

		cooltime = 60;

		hoppertimer = 0;

		dash_vec = mae;

		dash_vec.normalize();

	}
	if (hopper_dash)
	{
		if (!cooldown)
		{
			worldTransform_.translation_ = easeOutSineVec3(worldTransform_.translation_, worldTransform_.translation_ + dash_vec * hopper_speed, hoppertimer / hoppertime);

		}
		hoppertimer++;




		if (hoppertimer >= hoppertime)
		{
			hoppertimer = hoppertime;
			cooltime--;
			cooldown = true;

		}
		if (cooltime <= 0)
		{
			hopper_dash = false;
			cooldown = false;
		}

	}


	worldTransform_.translation_.y -= 0.1f;
	moveVec.y = -1;




	if ((input_->PushKey(DIK_SPACE) || (joystate.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) && hoppertimer >= 0)
	{
		if (!LockOn())
		{
			Vector3 kari2 = KyozouFlont;
			kari2.y = 0;
			float kariy2 = -KyozouFlont.y;

			float gomi = atan2(Flont.x, Flont.z);
			for (size_t i = 0; i < gunbitnum; i++)
			{
				target[i].rotation_.y = easeOutSine(target[i].rotation_.y, gomi, NormalTimer / BitNormalAttakTime);
			}
			kyozou.rotation_.y = gomi;
			gomi = atan2(kariy2, kari2.length());
			for (size_t i = 0; i < gunbitnum; i++)
			{
				target[i].rotation_.x = easeOutSine(target[i].rotation_.x, gomi, NormalTimer / BitNormalAttakTime);
			}
			kyozou.rotation_.x = gomi;

			for (size_t i = 0; i < gunbitnum; i++)
			{
				Vector3 bitpos = nannka[i];
				bitpos = kyozou.matWorld_.VectorMat(bitpos, kyozou.matWorld_);
				bitpos.normalize();
				target[i].translation_ = easeOutSineVec3(target[i].translation_, worldTransform_.translation_ + (bitpos * 2.0f), NormalTimer / BitNormalAttakTime);
			}


			NormalTimer++;

			if (NormalTimer >= BitNormalAttakTime)
			{
				NormalTimer = BitNormalAttakTime;
			}
		}
		else
		{
			NormalTimer = 0;
		}


		if (latetime <= 0)
		{
			Attack(camera->getForwardVec());
			latetime = firelate;
		}
		latetime--;

	}
	else
	{
		NormalTimer = 0;
	}


	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	for (size_t i = 0; i < gunbitnum; i++)
	{
		if (i == 0 || i == 2)
		{
			target[i].rotation_.z += 0.05f;
		}
		else
		{
			target[i].rotation_.z -= 0.05f;
		}
		target[i].matWorldGeneration();
	}

	kyozou.matWorldGeneration();

	worldTransform_.matWorldGeneration();

	//bosstarget->SetPosition(kasu(boss->getPos().translation_));




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
