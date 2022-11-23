#include "Player.h"
#include"easing.h"
#include <cassert>

DirectX::XMFLOAT3 VectorMatDivW(XMMATRIX mat, XMFLOAT3 pos)
{
	float w = pos.x * mat.r[0].m128_f32[3] + pos.y * mat.r[1].m128_f32[3] + pos.z * mat.r[2].m128_f32[3] + mat.r[3].m128_f32[3];

	XMFLOAT3 result =
	{
		(pos.x * mat.r[0].m128_f32[0] + pos.y * mat.r[1].m128_f32[0] + pos.z * mat.r[2].m128_f32[0] + mat.r[3].m128_f32[0]) / w,
		(pos.x * mat.r[0].m128_f32[1] + pos.y * mat.r[1].m128_f32[1] + pos.z * mat.r[2].m128_f32[1] + mat.r[3].m128_f32[1]) / w,
		(pos.x * mat.r[0].m128_f32[2] + pos.y * mat.r[1].m128_f32[2] + pos.z * mat.r[2].m128_f32[2] + mat.r[3].m128_f32[2]) / w
	};

	return result;
}




Player::Player()
{

}

Player::~Player()
{
}

void Player::Initialize(RailCamera* Rcamera, bosstest* boss)
{
	this->boss = boss;

	player = Obj3d{ "Resources/3dModels/player/Player.obj", Rcamera->getView()};
	arrow = Obj3d{ "Resources/3dModels/arrow/arrow.obj", Rcamera->getView() };

	
	kyozou=WorldCoordinate{ &viewProjection_ };

	for (size_t i = 0; i < gunbitnum; i++)
	{
		gunbit[i] = Obj3d{ "Resources/3dModels/bit/bit.obj", Rcamera->getView() };
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

	arrowrange = 4.0f;
	arrowvec = 1.0f;

	NormalTimer = 0;

	bitmovetimer = 60;

	lockmove = false;


	hopper_speed = 5.0f;

	cooldown = false;

	
	latetime = 0;

	isDead = false;

	hoppertimer = 0;

	hozon = { 0,0,0 };

	height = Window::GetInstance()->height_;
	width  = Window::GetInstance()->width_;

	Reticle.SetPosition({width/2.0f,height/2.0f});
	Reticle.SetSize({ 1280.0f,720.0f });
	Reticle.SetAnchorPoint({ 0.5f,0.5f });

	bosstarget.SetAnchorPoint({ 0.5f,0.5f });
	bosstarget.SetSize({ 64.0f,64.0f });

	player.worldCoordinate_.position_.z = -350.0f;
	player.Update();

}

void Player::Update(RailCamera* camera)
{
	


	moveVec = { 0,0,0 };
	
	XMFLOAT3 Flont = camera->getForwardVec();
	XMFLOAT3 KyozouFlont = Flont;
	Flont.y = 0;
	normalize(Flont);
	

	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	XMFLOAT2 inputnum = XPAD::GetLStick();
	
	moveVec.x += (float)inputnum.x / SHRT_MAX;
	moveVec.z += (float)inputnum.y / SHRT_MAX;
	


	if (KEYS::IsDown(DIK_UP))
	{
		moveVec.z = 1;
	}
	if (KEYS::IsDown(DIK_DOWN))
	{
		moveVec.z = -1;
	}
	if (KEYS::IsDown(DIK_RIGHT))
	{
		moveVec.x = 1;
	}
	if (KEYS::IsDown(DIK_LEFT))
	{
		moveVec.x = -1;
	}

	//移動


	float p_pos = atan2(moveVec.x, moveVec.z);
	float c_vec = atan2(Flont.x, Flont.z);

	player.worldCoordinate_.rotation_.y = (p_pos + c_vec);

	XMFLOAT3 mae = { 0,0,1.0f };

	mae = VectorMat(mae, player.worldCoordinate_.matWorld_);

	normalize(mae);

	if (!hopper_dash && !cooldown)
	{
		if (moveVec.x != 0 || moveVec.z != 0)
		{
			player.worldCoordinate_.position_ += mae * move_speed;
		}
	}

	EnemyArrow();

	if (LockOn(camera))
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

	if (LockOn(camera))
	{


		for (size_t i = 0; i < gunbitnum; i++)
		{
			XMFLOAT3 PtoB = BitVec - gunbit[i].worldCoordinate_.position_;
			XMFLOAT3 kari = PtoB;
			kari.y = 0;
			float kariy = -PtoB.y;
			normalize(PtoB);
			float gomi = atan2(PtoB.x, PtoB.z);
			gunbit[i].worldCoordinate_.rotation_.y = easeOutSine(gunbit[i].worldCoordinate_.rotation_.y, gomi, timer / bitmovetimer);
			gomi = atan2(kariy, length(kari));
			kyozou.rotation_.y = gomi;
			gunbit[i].worldCoordinate_.rotation_.x = easeOutSine(gunbit[i].worldCoordinate_.rotation_.x, gomi, timer / bitmovetimer);
			kyozou.rotation_.x = gomi;
		}
		XMFLOAT3 KtoB = BitVec - player.worldCoordinate_.position_;
		XMFLOAT3 Kkari = KtoB;
		Kkari.y = 0;
		float Kkariy = -KtoB.y;
		normalize(KtoB);
		float gomi = atan2(KtoB.x, KtoB.z);
		kyozou.rotation_.y = gomi;
		gomi = atan2(Kkariy, length(Kkari));
		kyozou.rotation_.x = gomi;

		for (size_t i = 0; i < gunbitnum; i++)
		{
			XMFLOAT3 bitpos = nannka[i];
			bitpos = VectorMat(bitpos, kyozou.matWorld_);
			normalize(bitpos);
			gunbit[i].worldCoordinate_.position_ = easeOutSineVec3(gunbit[i].worldCoordinate_.position_, player.worldCoordinate_.position_ + (bitpos * 2.0f), timer / bitmovetimer);
		}


	}
	else
	{
		for (size_t i = 0; i < gunbitnum; i++)
		{
			XMFLOAT3 bitpos = nannka[i];
			bitpos = VectorMat(bitpos, player.worldCoordinate_.matWorld_);
			normalize(bitpos);
			gunbit[i].worldCoordinate_.position_ = player.worldCoordinate_.position_ + (bitpos * 2.0f);
		}

		for (size_t i = 0; i < gunbitnum; i++)
		{

			gunbit[i].worldCoordinate_.rotation_.y = player.worldCoordinate_.rotation_.y;
			gunbit[i].worldCoordinate_.rotation_.x = player.worldCoordinate_.rotation_.x;
		}
	}



	
	

	if ((KEYS::IsTrigger(DIK_Z) || XPAD::IsTrigger(XINPUT_GAMEPAD_A)) && !cooldown&&!hopper_dash)
	{
		hopper_dash = true;

		cooltime = 45;

		hoppertimer = 0;

		dash_vec = mae;

		normalize(dash_vec);

	}
	if (hopper_dash)
	{
		if (!cooldown)
		{
			player.worldCoordinate_.position_ = easeOutSineVec3(player.worldCoordinate_.position_, player.worldCoordinate_.position_ + dash_vec * hopper_speed, hoppertimer / hoppertime);

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


	player.worldCoordinate_.position_.y -= 0.1f;
	moveVec.y = -1;




	if ((KEYS::IsDown(DIK_SPACE) || (XPAD::IsDown( XINPUT_GAMEPAD_RIGHT_SHOULDER)) && hoppertimer >= 0))
	{
		if (!LockOn(camera))
		{
			XMFLOAT3 kari2 = KyozouFlont;
			kari2.y = 0;
			float kariy2 = -KyozouFlont.y;

			float gomi = atan2(Flont.x, Flont.z);
			for (size_t i = 0; i < gunbitnum; i++)
			{
				gunbit[i].worldCoordinate_.rotation_.y = easeOutSine(gunbit[i].worldCoordinate_.rotation_.y, gomi, NormalTimer / BitNormalAttakTime);
			}
			kyozou.rotation_.y = gomi;
			gomi = atan2(kariy2, length(kari2));
			for (size_t i = 0; i < gunbitnum; i++)
			{
				gunbit[i].worldCoordinate_.rotation_.x = easeOutSine(gunbit[i].worldCoordinate_.rotation_.x, gomi, NormalTimer / BitNormalAttakTime);
			}
			kyozou.rotation_.x = gomi;

			for (size_t i = 0; i < gunbitnum; i++)
			{
				XMFLOAT3 bitpos = nannka[i];
				bitpos = VectorMat(bitpos, kyozou.matWorld_);
				normalize(bitpos);
				gunbit[i].worldCoordinate_.position_ = easeOutSineVec3(gunbit[i].worldCoordinate_.position_, player.worldCoordinate_.position_ + (bitpos * 2.0f), NormalTimer / BitNormalAttakTime);
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
			Attack(camera->getForwardVec(),camera);
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
			gunbit[i].worldCoordinate_.rotation_.z += 0.05f;
		}
		else
		{
			gunbit[i].worldCoordinate_.rotation_.z -= 0.05f;
		}
		gunbit[i].Update();
	}

	kyozou.Update();

	player.Update();

	Reticle.Update();
	bosstarget.Update();

	//bosstarget->SetPosition(kasu(boss->getPos().translation_));




}

void Player::GameOverUpdate()
{
	player.worldCoordinate_.rotation_.z = XMConvertToRadians(55.0f);
	player.worldCoordinate_.rotation_.y = 0.0f;
	player.worldCoordinate_.rotation_.x = 0.0f;
	
	player.worldCoordinate_.position_ = { 0,0.7f,0 };
	
	gunbit[0].worldCoordinate_.position_ = player.worldCoordinate_.position_ + XMFLOAT3(1.0f,-0.5f,1.0f);
	gunbit[0].worldCoordinate_.rotation_ = { 0,XMConvertToRadians(-45.0f),XMConvertToRadians(90.0f) };
	gunbit[1].worldCoordinate_.position_ = player.worldCoordinate_.position_ + XMFLOAT3(6.0f, -0.5f, -2.0f);
	gunbit[1].worldCoordinate_.rotation_ = { 0,XMConvertToRadians(128.0f),XMConvertToRadians(90.0f) };
	gunbit[2].worldCoordinate_.position_ = player.worldCoordinate_.position_ + XMFLOAT3(-2.0f, -0.5f, -4.0f);
	gunbit[2].worldCoordinate_.rotation_ = { 0,XMConvertToRadians(240.0f),XMConvertToRadians(90.0f) };
	gunbit[3].worldCoordinate_.position_ = player.worldCoordinate_.position_ + XMFLOAT3(-5.0f, 0, 2.0f);
	gunbit[3].worldCoordinate_.rotation_ = { XMConvertToRadians(75.0f),XMConvertToRadians(240.0f),XMConvertToRadians(0.0f) };
	
	
	player.Update();
	for (size_t i = 0; i < gunbitnum; i++)
	{
		gunbit[i].Update();
	}

}

void Player::Draw(RailCamera* camera)
{
	//3Dモデルを描画
	player.Draw();

	for (size_t i = 0; i < gunbitnum; i++)
	{
		gunbit[i].Draw();
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}

	if (!screenLock(boss->getPos(),camera))
	{
		arrow.Draw();
	}
}

void Player::DrawUI(RailCamera* camera)
{
	Reticle.Draw();
	if (LockOn(camera))
	{
		bosstarget.Draw();
	}
}

XMFLOAT2 Player::kasu(XMFLOAT3 obj,RailCamera* camera)
{
	XMFLOAT3 positionReticle = obj;

	XMMATRIX matViewport = {
		1280 / 2,0,0,0,
		0,-720 / 2,0,0,
		0,0,1,0,
		1280 / 2 + 0,720 / 2 + 0,0,1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	XMMATRIX matViewProjectionViewport = camera->getView()->GetView();
	matViewProjectionViewport *= camera->getView()->GetProjection();
	matViewProjectionViewport *= matViewport;

	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = VectorMatDivW(matViewProjectionViewport, positionReticle);

	//スプライトのレティクルに座標設定
	return XMFLOAT2(positionReticle.x, positionReticle.y);
}

bool Player::screenLock(WorldCoordinate pos, RailCamera* camera)
{
	XMMATRIX Pos = pos.matWorld_;
	Pos *= camera->getView()->GetView();
	Pos *= camera->getView()->GetProjection();

	float objZ = Pos.r[3].m128_f32[2];

	XMFLOAT2 scr_pos = kasu(pos.position_,camera);

	if ((width / 2) - 120.0f < scr_pos.x && (width / 2) + 120.0f > scr_pos.x && (height / 2) - 120.0f < scr_pos.y && (height / 2) + 120.0f > scr_pos.y && objZ > 0)
	{
		return true;
	}

	else
	{
		return false;
	}
}

void Player::Attack(XMFLOAT3 flont, RailCamera* camera)
{
	const float kBulletSpeed = 5.0f;
	for (size_t i = 0; i < gunbitnum; i++)
	{
		XMFLOAT3 velocity(0, 0, 1);

		velocity = VectorMat(velocity, gunbit[i].worldCoordinate_.matWorld_);
		normalize(velocity);

		XMFLOAT3 BulletStart = gunbit[i].worldCoordinate_.position_ - velocity * 3.0f;
		velocity *= kBulletSpeed;


		std::unique_ptr <PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(camera, BulletStart, gunbit[i].worldCoordinate_.rotation_, velocity);

		bullets_.push_back(std::move(newBullet));

	}

}

XMFLOAT3 Player::VectorMat(XMFLOAT3 vector, XMMATRIX mat)
{
	DirectX::XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.r[0].m128_f32[0] + vector.y * mat.r[1].m128_f32[0] + vector.z * mat.r[2].m128_f32[0] + 0.0f * mat.r[3].m128_f32[0];
	changeVector.y = vector.x * mat.r[0].m128_f32[1] + vector.y * mat.r[1].m128_f32[1] + vector.z * mat.r[2].m128_f32[1] + 0.0f * mat.r[3].m128_f32[1];
	changeVector.z = vector.x * mat.r[0].m128_f32[2] + vector.y * mat.r[1].m128_f32[2] + vector.z * mat.r[2].m128_f32[2] + 0.0f * mat.r[3].m128_f32[2];

	return changeVector;
}

XMFLOAT3 Player::VectorMat(XMMATRIX mat, XMFLOAT3 vector)
{
	DirectX::XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = mat.r[0].m128_f32[0] * vector.x + mat.r[0].m128_f32[1] * vector.y + mat.r[0].m128_f32[2] * vector.z + mat.r[0].m128_f32[3] * 0.0f;
	changeVector.y = mat.r[1].m128_f32[0] * vector.x + mat.r[1].m128_f32[1] * vector.y + mat.r[1].m128_f32[2] * vector.z + mat.r[1].m128_f32[3] * 0.0f;
	changeVector.z = mat.r[2].m128_f32[0] * vector.x + mat.r[2].m128_f32[1] * vector.y + mat.r[2].m128_f32[2] * vector.z + mat.r[2].m128_f32[3] * 0.0f;

	return changeVector;
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

bool Player::LockOn(RailCamera* camera)
{
	const std::vector<bossHand*>& bosshands = boss->getHand();
	for (int i = 0; i < bosshands.size(); i++)
	{
		if (screenLock(boss->getPos(),camera) && screenLock(bosshands[i]->GetwroldTransform(),camera))
		{
			if (bosshands[i]->getisAttackFlag())
			{
				XMFLOAT3 hozon2 = Hikaku(boss->GetWorldPosition(), bosshands[i]->GetwroldTransform().position_, hozon);
				//bosstarget->SetPosition(kasu(hozon2));
				bosstarget.SetPosition(kasu(hozon2,camera));
				BitVec = hozon2;
				hozon = hozon2;

			}
			else
			{
				bosstarget.SetPosition(kasu(boss->GetWorldPosition(),camera));
				BitVec = boss->GetWorldPosition();
			}


			return true;
		}
		else if (screenLock(boss->getPos(),camera))
		{

			bosstarget.SetPosition(kasu(boss->GetWorldPosition(),camera));
			BitVec = boss->GetWorldPosition();
			return true;
		}
		else if (screenLock(bosshands[i]->GetwroldTransform(),camera) && bosshands[i]->getisAttackFlag())
		{

			XMFLOAT3 hozon2 = Hikaku2(bosshands[i]->GetwroldTransform().position_, hozon);
			bosstarget.SetPosition(kasu(hozon2,camera));
			hozon = hozon2;
			BitVec = hozon2;


			return true;
		}
		else
		{
			hozon = { 0,0,0 };
			return false;

		}
	}


}

void Player::SetWorldPosition(XMFLOAT3 osimodosi)
{
	player.worldCoordinate_.position_ = osimodosi;
}

XMFLOAT3 Player::Hikaku(XMFLOAT3 boss, XMFLOAT3 hand1, XMFLOAT3 hand2)
{
	XMFLOAT3 kariA, kariB, kariC;
	kariA = boss -  player.worldCoordinate_.position_;
	kariB = hand1 - player.worldCoordinate_.position_;
	kariC = hand2 - player.worldCoordinate_.position_;

	if (length(kariA) < length(kariB) && length(kariA) < length(kariC)) return boss;
	else if (length(kariB) < length(kariA) && length(kariB) < length(kariC)) return hand1;
	else if (length(kariC) < length(kariA) && length(kariC) < length(kariB)) return hand2;
	else
	{
		return boss;
	}

}

XMFLOAT3 Player::Hikaku2(XMFLOAT3 hand1, XMFLOAT3 hand2)
{
	XMFLOAT3 kariB, kariC;
	kariB = hand1 - player.worldCoordinate_.position_;
	kariC = hand2 - player.worldCoordinate_.position_;

	if (length(kariB) <= length(kariC)) return hand1;
	else if (length(kariC) < length(kariB)) return hand2;
	else
	{
		return hand1;
	}

}

WorldCoordinate Player::GetHitArea()
{
	WorldCoordinate a = player.worldCoordinate_;
	a.scale_.y = 1.5f;

	return a;
}

void Player::EnemyArrow()
{
	XMFLOAT3 PtoB = boss->GetWorldPosition() - player.worldCoordinate_.position_;
	XMFLOAT3 kari = PtoB;
	float kariy = -PtoB.y;
	normalize(PtoB);

	if (arrowrange >= 4.0f || arrowrange <= 3.0f)
	{
		arrowvec *= -1;
	}

	arrowrange += (0.05f * arrowvec);

	arrow.worldCoordinate_.position_ = player.worldCoordinate_.position_+ (PtoB * arrowrange);
	kari.y = 0;
	float gomi = atan2(PtoB.x, PtoB.z);
	arrow.worldCoordinate_.rotation_.y = gomi;
	gomi = atan2(kariy, length(kari));
	arrow.worldCoordinate_.rotation_.x = gomi;

	arrow.worldCoordinate_.rotation_.z += 0.05f;


	arrow.Update();
}

int Player::SetDamege(int attacknum)
{

	return 0;
}

void Player::OnCollision()
{
}

void Player::reset()
{
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
		{
			return 1;
		});

	player.worldCoordinate_.position_ = { 0,0,50 };
	player.Update();

}
