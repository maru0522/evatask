#include "bosstest.h"
#include <cassert>
#include <cstdlib>

//const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);




int RNG(int min, int max, bool preciseMode = false);







DirectX::XMFLOAT3 nainavec3(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	DirectX::XMFLOAT3 ans = {};

	ans = { a.x - b.x,a.y - b.y,a.z - b.z };

	return ans;

}

DirectX::XMFLOAT3 VectorMat(DirectX::XMFLOAT3 vector, DirectX::XMMATRIX mat)
{
	DirectX::XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.r[0].m128_f32[0] + vector.y * mat.r[1].m128_f32[0] + vector.z * mat.r[2].m128_f32[0] + 0.0f * mat.r[3].m128_f32[0];
	changeVector.y = vector.x * mat.r[0].m128_f32[1] + vector.y * mat.r[1].m128_f32[1] + vector.z * mat.r[2].m128_f32[1] + 0.0f * mat.r[3].m128_f32[1];
	changeVector.z = vector.x * mat.r[0].m128_f32[2] + vector.y * mat.r[1].m128_f32[2] + vector.z * mat.r[2].m128_f32[2] + 0.0f * mat.r[3].m128_f32[2];

	return changeVector;
}

DirectX::XMFLOAT3 VectorMat(DirectX::XMMATRIX mat, DirectX::XMFLOAT3 vector)
{
	DirectX::XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = mat.r[0].m128_f32[0] * vector.x + mat.r[0].m128_f32[1] * vector.y + mat.r[0].m128_f32[2] * vector.z + mat.r[0].m128_f32[3] * 0.0f;
	changeVector.y = mat.r[1].m128_f32[0] * vector.x + mat.r[1].m128_f32[1] * vector.y + mat.r[1].m128_f32[2] * vector.z + mat.r[1].m128_f32[3] * 0.0f;
	changeVector.z = mat.r[2].m128_f32[0] * vector.x + mat.r[2].m128_f32[1] * vector.y + mat.r[2].m128_f32[2] * vector.z + mat.r[2].m128_f32[3] * 0.0f;

	return changeVector;
}

bosstest::bosstest()
{

}
bosstest::~bosstest()
{
	for (int i = 0; i < hand.size(); i++)
	{
		delete hand[i];
	}
}

void bosstest::reset()
{

	isHandMove = false;

	isbossPunch = false;
	isBossPress = false;
	isBossStoneFall = false;
	isBossPillarFall = false;
	isBossPillarRoll = false;
	isBossBeam = false;
	isBossPushUp = false;

	isPressStart = false;
	isPressFall = false;
	isPressReturn = false;
	pressFirstStart = false;
	pillarRollFirstStart = false;
	isPillarRollEnd = false;
	pillarPushUpFirstStart = false;
	beamFirstStart = false;
	beamEndStart = false;

	isbossStart = false;

	punchCount = 0;
	pressCount = 0;
	bossStoneFallCount = 0;
	bossPillarFallCount = 0;
	bossBeamCount = 0;

	timeCount = 0;
	fallTimeCount = 0;
	returnTimeCount = 0;
	moveEndTimeCount = 0;

	beamFirstMoveCount = 0;
	beamEndMoveCount = 0;

	bosspillarMoveTime = 0;

	bosspillarRollTime = 0;
	bosspillarRollEndTime = 0;

	bosspillarDefaultPosCount = 0;

	waitTime = 0;
	punchWaitTime = 0;
	bossActionSelectWait = bossActionSelectWaitTime;
	bossHP = 1000;

	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->reset();
	}


}

void bosstest::FinalizeSound(void)
{
    XAudio::UnLoad(&SE_BOSSAttack);
    XAudio::UnLoad(&SE_spark);
}

void bosstest::bossStart(DirectX::XMFLOAT3 pos)
{
	if (isbossStart)
	{

		if (timeCount < maxbossStartTime)
		{

			timeCount++;

			worldTransform.worldCoordinate_.position_ = lerp(pos, { pos.x, 7, pos.z }, timeCount / maxbossStartTime);

			worldTransform.Update();

			setPressHandPos();

		}

		if (timeCount == maxbossStartTime)
		{

			timeCount = 0;
			isbossStart = false;
		}

	}
}

void bosstest::SetStartFlag(bool flag)
{
	isbossStart = flag;
}

void bosstest::Initialize(RailCamera* camera, DirectX::XMFLOAT3 pos)
{

	worldTransform = Obj3d("Resources/3dModels/core/core.obj", camera->getView());

	//引数で受け取った初期座標をセット
	worldTransform.worldCoordinate_.position_ = pos;
	worldTransform.worldCoordinate_.scale_ = { 3.0f,3.0f,3.0f };

	worldTransform.Update();

	for (int i = 0; i < 8; i++)
	{

		bossHand* newHand=new bossHand();
		newHand->init({ 3.0f,3.0f,3.0f }, {}, { sinf(i * 2) * 5,cosf(i * 2) * 5,200.0f }, camera->getView());

		hand.push_back(newHand);
	}

	setPressHandPos();

	HelthBar.SetPosition({ 140,30 });

	HelthBar.SetSize({ bossHP * 1.0f,32.0f });

}

void bosstest::Update(DirectX::XMFLOAT3 player)
{

	if (!isBossBeam)
	{
		DirectX::XMFLOAT3 front = nainavec3(player, worldTransform.worldCoordinate_.position_);
		float fVec = atan2(front.x, front.z);
		worldTransform.worldCoordinate_.rotation_.y = fVec;
	}
	else
	{
		worldTransform.worldCoordinate_.rotation_.y = 0.0f;
	}

	worldTransform.Update();

	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisGetTargetPosFlag())
		{
			hand[i]->setTargetPos(player);
		}
	}

	if (bossHP < 200)
	{
		state = pillar;
	}

	cubeActionDecision();

		if (state == Cube)
		{
			
			

			setCubeDefaultPos();

			if (isHandMove == false)
			{
				setPressHandPos();
			}

			bossPunch(player);
			bossPress(player);
			bossStoneFall(player);
			bossBeam();

		}

		if (state == pillar)
		{
			setPillarDefaultPos();
			setPillarHandPos();
			bossPillarFall(player);
			bossPillarRoll();
			bossPushUp(player);
		}

 		if (state == ophanim)
		{
			setOphanimDefaultPos();
			setOphanimHandPos();
		}
		

	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setRotate({ 0.0f,worldTransform.worldCoordinate_.rotation_.y,0.0f });
		hand[i]->update(worldTransform.worldCoordinate_);
	}

	
	HelthBar.SetSize({ bossHP * 1.0f,32.0f });
	HelthBar.Update();
	
}

void bosstest::Draw()
{
	
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->draw();
		/*debugText_->SetPos(0, i * 20 + 50);
		debugText_->Printf("%d", hand[i]->getisReturnHand());*/
	}
	worldTransform.Draw();
	
	
}

void bosstest::DrawUI(void)
{
	HelthBar.Draw();
}

void bosstest::cubeActionDecision()
{

	if (isbossPunch == false and isBossPress == false and isBossStoneFall == false and isBossBeam == false and state == Cube)
	{
		if (bossActionSelectWait == 0)
		{

			int Select = RNG(0, 3);

			switch (Select)
			{
			case 0:

				isbossPunch = true;
				break;

			case 1:

				isBossPress = true;

				break;

			case 2:

				isBossStoneFall = true;

				break;

			case 3:

				isBossBeam = true;

				break;

			default:
				break;
			}

			bossActionSelectWait = bossActionSelectWaitTime;
		}

		if (bossActionSelectWait > 0)
		{
			bossActionSelectWait--;
		}

	}

	if (isBossPillarFall == false and isBossPillarRoll == false and isBossPushUp == false and state == pillar)
	{
		if (bossActionSelectWait > 0)
		{
			bossActionSelectWait--;
		}

		if (bossActionSelectWait == 0)
		{

			int Select = RNG(0, 2);

			switch (Select)
			{
			case 0:

				isBossPillarFall = true;
				break;

			case 1:

				isBossPillarRoll = true;

				break;

			case 2:

				isBossPushUp = true;

				break;

			default:
				break;
			}

			bossActionSelectWait = bossActionSelectWaitTime;
		}

		

	}

}

void bosstest::bossPunch(DirectX::XMFLOAT3 player)
{

	if (isbossPunch)
	{
		if (punchWaitTime == 0)
		{
			if (punchCount == hand.size()/2)
			{
				if (hand[4]->getisAction() == false and hand[5]->getisAction() == false and hand[6]->getisAction() == false and hand[7]->getisAction() == false)
				{
					isbossPunch = false;
					punchCount = 0;
				}
				return;
			}

			for (int i = 4; i < hand.size(); i++)
			{
				if (hand[i]->getisAction() == false)
				{
					hand[i]->setisAttackFlag(true);
					hand[i]->setTargetPos({player.x,player.y+2.0f ,player.z });
					punchWaitTime = bossPunchWaitTime;
					punchCount++;
					break;
				}
			}
		}

		if (punchWaitTime > 0)
		{
			punchWaitTime--;
		}
	}

}

void bosstest::bossPress(DirectX::XMFLOAT3 player)
{

	if (isBossPress)
	{
		if (pressFirstStart == false)
		{
			setPressPos();

			for (int i = 0; i < hand.size(); i++)
			{
				hand[i]->setisPressEndFlag(false);
			}

			pressFirstStart = true;

			isPressReturn = true;
		}

		if (isPressStart)
		{

			if (timeCount == 0)
			{
				targetPos = player;
			}

			if (timeCount < maxTime)
			{
				timeCount++;
			}

			if (timeCount != maxTime)
			{

				worldTransform.worldCoordinate_.position_ = lerp(returnPos, { targetPos.x, pressPosY, targetPos.z }, timeCount / maxTime);

				worldTransform.Update();

			}

			if (timeCount == maxTime)
			{

				isPressStart = false;
				isPressFall = true;
				timeCount = 0;

			}

		}

		if (isPressFall)
		{

			if (waitTime < fallWaitTime)
			{

				worldTransform.worldCoordinate_.position_ = { cosf(waitTime) + targetPos.x, pressPosY, targetPos.z };

				worldTransform.Update();

			}

			if (fallTimeCount < maxfallTime and waitTime >= fallWaitTime)
			{
				fallTimeCount++;
			}

			if (fallTimeCount != maxfallTime and waitTime >= fallWaitTime)
			{

				worldTransform.worldCoordinate_.position_ = lerp({ targetPos.x, pressPosY, targetPos.z }, { targetPos.x, targetPos.y+1.1f, targetPos.z }, fallTimeCount / maxfallTime);

				worldTransform.Update();

			}

			if (waitTime < fallWaitTime)
			{
				waitTime++;
			}

			if (fallTimeCount == maxfallTime)
			{

				isPressFall = false;
				isPressReturn = true;
				fallTimeCount = 0;
				waitTime = 0;

			}

		}

		if (isPressReturn)
		{

			if (pressCount < 3)
			{

				if (waitTime >= returnWaitTime)
				{

					returnPos = worldTransform.worldCoordinate_.position_;

				}

				if (returnTimeCount < maxReturnTime and waitTime >= returnWaitTime)
				{
					returnTimeCount++;
				}

				if (returnTimeCount != maxReturnTime and waitTime >= returnWaitTime)
				{

					worldTransform.worldCoordinate_.position_ = lerp(returnPos, { returnPos.x, pressPosY, returnPos.z }, returnTimeCount / maxReturnTime);

					worldTransform.Update();

				}

				if (waitTime < returnWaitTime)
				{
					waitTime++;
				}


				if (returnTimeCount == maxReturnTime)
				{

					isPressReturn = false;

					isPressStart = true;

					waitTime = 0;
					returnTimeCount = 0;

					pressCount++;

				}
			}

			else 
			{
				

				if (moveEndTimeCount == 0)
				{
					returnPos = worldTransform.worldCoordinate_.position_;
				}

				if (moveEndTimeCount < maxMoveEndTime)
				{
					moveEndTimeCount++;

					worldTransform.worldCoordinate_.position_ = lerp(returnPos, { returnPos.x, 1, returnPos.z }, moveEndTimeCount / maxMoveEndTime);
					
					worldTransform.Update();
				}

				if (moveEndTimeCount == maxMoveEndTime)
				{
					pressCount = 0;
					setPressPos();
					moveEndTimeCount = 0;

					isBossPress = false;
					isPressStart = false;

					pressFirstStart = false;

					setPressEnd();

					for (int i = 0; i < hand.size(); i++)
					{
						hand[i]->setisPressFlag(false);
					}
				}
			}
		}

	}

}

void bosstest::bossStoneFall(DirectX::XMFLOAT3 player)
{

	if (isBossStoneFall)
	{
		if (waitTime == 0)
		{
			if (bossStoneFallCount == hand.size())
			{
				if (hand[0]->getisAction() == false and hand[1]->getisAction() == false and hand[2]->getisAction() == false and hand[3]->getisAction() == false and hand[4]->getisAction() == false and hand[5]->getisAction() == false and hand[6]->getisAction() == false and hand[7]->getisAction() == false)
				{
					isBossStoneFall = false;
					bossStoneFallCount = 0;
				}
				return;
			}

			for (int i = 0; i < hand.size(); i++)
			{
				if (hand[i]->getisAction() == false)
				{
					hand[i]->setisStoneFallFlag(true);
					waitTime = bossStoneFallWaitTime;
					bossStoneFallCount++;
					break;
				}
			}
		}

		if (waitTime > 0)
		{
			waitTime--;
		}
	}

}

void bosstest::bossPillarFall(DirectX::XMFLOAT3 player)
{

	if (isBossPillarFall)
	{
		if (waitTime == 0)
		{
			if (bossPillarFallCount == hand.size())
			{

				if (hand[0]->getisAction() == false and hand[1]->getisAction() == false and hand[2]->getisAction() == false and hand[3]->getisAction() == false and hand[4]->getisAction() == false and hand[5]->getisAction() == false and hand[6]->getisAction() == false and hand[7]->getisAction() == false)
				{
					isBossPillarFall = false;
					bossPillarFallCount = 0;
				}
				return;
			}

			for (int i = 0; i < hand.size(); i++)
			{
				if (hand[i]->getisAction() == false)
				{
					hand[i]->setisPillarFallFlag(true);
					waitTime = bossPillarFallWaitTime;
					bossPillarFallCount++;
					break;
				}
			}
		}

		if (waitTime > 0)
		{
			waitTime--;
		}
	}

}

void bosstest::bossBeam()
{

	if (isBossBeam)
	{
		if (waitTime > 0 and hand[0]->getisAction() == false)
		{
			waitTime--;
		}

		

		if (bossBeamCount == 0 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.worldCoordinate_.position_.x, worldTransform.worldCoordinate_.position_.y - 5, worldTransform.worldCoordinate_.position_.z+200});
			hand[4]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 1 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.worldCoordinate_.position_.x, worldTransform.worldCoordinate_.position_.y - 5, worldTransform.worldCoordinate_.position_.z + 200 });
			hand[4]->setisBeamFlag(true);

			hand[5]->setTargetPos({ worldTransform.worldCoordinate_.position_.x - 20, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[5]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 2 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.worldCoordinate_.position_.x, worldTransform.worldCoordinate_.position_.y - 5, worldTransform.worldCoordinate_.position_.z + 200 });
			hand[4]->setisBeamFlag(true);

			hand[5]->setTargetPos({ worldTransform.worldCoordinate_.position_.x + 20, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[5]->setisBeamFlag(true);

			hand[6]->setTargetPos({ worldTransform.worldCoordinate_.position_.x + 40, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[6]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 3 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.worldCoordinate_.position_.x, worldTransform.worldCoordinate_.position_.y - 5, worldTransform.worldCoordinate_.position_.z + 200 });
			hand[4]->setisBeamFlag(true);

			hand[5]->setTargetPos({ worldTransform.worldCoordinate_.position_.x + 20, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[5]->setisBeamFlag(true);

			hand[6]->setTargetPos({ worldTransform.worldCoordinate_.position_.x + 40, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[6]->setisBeamFlag(true);

			hand[7]->setTargetPos({ worldTransform.worldCoordinate_.position_.x - 20, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[7]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 4 and hand[4]->getisAction() == false and waitTime == 0)
		{
			hand[4]->setTargetPos({ worldTransform.worldCoordinate_.position_.x, worldTransform.worldCoordinate_.position_.y - 5, worldTransform.worldCoordinate_.position_.z + 200 });
			hand[4]->setisBeamFlag(true);

			hand[5]->setTargetPos({ worldTransform.worldCoordinate_.position_.x + 30, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[5]->setisBeamFlag(true);

			hand[6]->setTargetPos({ worldTransform.worldCoordinate_.position_.x + 60, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[6]->setisBeamFlag(true);

			hand[7]->setTargetPos({ worldTransform.worldCoordinate_.position_.x - 30, worldTransform.worldCoordinate_.position_.y - 5,worldTransform.worldCoordinate_.position_.z + 200 });
			hand[7]->setisBeamFlag(true);
			bossBeamCount++;
			waitTime = bossBeamWaitTime;
			return;
		}

		if (bossBeamCount == 5 and hand[4]->getisAction() == false )
		{

			
				bossBeamCount = 0;
				
				
				isBossBeam = false;
			
		}

	}

}

void bosstest::bossPillarRoll()
{

	if (isBossPillarRoll)
	{

		if (pillarRollFirstStart == false)
		{
			for (int i = 0; i < hand.size(); i++)
			{
				hand[i]->setisActionFlag(true);
			}
			pillarRollFirstStart = true;
		}

		DirectX::XMFLOAT3 bossSft;
		DirectX::XMFLOAT3 bossSftMove;

		//カウント
		if (bosspillarRollTime < maxBosspillarRollTime and isPillarRollEnd == false)
		{

			bosspillarRollTime++;
			bosspillarRollEndTime++;
		}

		//とっておいたプレイヤーの座標に移動
		if (bosspillarRollTime < maxBosspillarRollTime and isPillarRollEnd == false)
		{
			for (int i = 0; i < hand.size(); i++)
			{


					bossSft = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i) % 8];
					bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
					normalize(bossSft);

					bossSftMove = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i + 1) % 8];
					bossSftMove = VectorMat(bossSftMove, worldTransform.worldCoordinate_.matWorld_);
					normalize(bossSftMove);
					hand[i]->setPos(lerp(worldTransform.worldCoordinate_.position_ + (bossSft * setBossPillarRollDistance), worldTransform.worldCoordinate_.position_ + (bossSftMove * setBossPillarRollDistance), bosspillarRollTime / maxBosspillarRollTime));


			}
		}

		//終了
		if (bosspillarRollTime == maxBosspillarRollTime and isPillarRollEnd == false)
		{
			if (bosspillarDefaultPosCount < _countof(pillarDefaultPosRotate) - 1)
			{
				bosspillarDefaultPosCount++;
			}
			else
			{
				bosspillarDefaultPosCount = 0;
			}
			bosspillarRollTime = 0;
			setBossPillarRollDistance+=5;
		}

		if (bosspillarRollEndTime == maxBosspillarRollEndTime and isPillarRollEnd == false)
		{

			for (int i = 0; i < hand.size(); i++)
			{
				hand[i]->setisPillarRollFlag(true);
			}

			isPillarRollEnd = true;

			return;
		}

		if (hand[0]->getisAction() == false and hand[1]->getisAction() == false and hand[2]->getisAction() == false and hand[3]->getisAction() == false and hand[4]->getisAction() == false and hand[5]->getisAction() == false and hand[6]->getisAction() == false and hand[7]->getisAction() == false)
		{

			pillarRollFirstStart = false;
			isPillarRollEnd = false;
			isBossPillarRoll = false;

			bosspillarRollTime = 0;
			bosspillarRollEndTime = 0;
			bosspillarDefaultPosCount = 0;
			setBossPillarRollDistance = setbossCubeDistance;
		}

	}

}

void bosstest::bossPushUp(DirectX::XMFLOAT3 player)
{

	if (isBossPushUp)
	{

		if (pillarPushUpFirstStart == false)
		{
			DirectX::XMFLOAT3 front = nainavec3(player, worldTransform.worldCoordinate_.position_);

			normalize(front);

			for (int i = 0; i < hand.size(); i++)
			{

				DirectX::XMFLOAT3 target = { worldTransform.worldCoordinate_.position_.x + (front.x*(10 * (i + 1))),1,worldTransform.worldCoordinate_.position_.z + (front.z * (10 * (i + 1))) };

				hand[i]->setTargetPos(target);
				hand[i]->setisPillarPushUpFlag(true);

			}

			pillarPushUpFirstStart = true;

			return;
		}

		if (hand[0]->getisAction() == false and hand[1]->getisAction() == false and hand[2]->getisAction() == false and hand[3]->getisAction() == false and hand[4]->getisAction() == false and hand[5]->getisAction() == false and hand[6]->getisAction() == false and hand[7]->getisAction() == false)
		{

			pillarPushUpFirstStart = false;
			isBossPushUp = false;

		}

	}

}

DirectX::XMFLOAT3 bosstest::GetWorldPosition()
{
	return worldTransform.getWorldPos();
}

void bosstest::setPos(DirectX::XMFLOAT3 pos)
{

	worldTransform.worldCoordinate_.position_ = pos;
	worldTransform.Update();

}

void bosstest::setRotate(DirectX::XMFLOAT3 rotate)
{
	worldTransform.worldCoordinate_.rotation_ = rotate;
	worldTransform.Update();
}

void bosstest::setisAttackFlagL(bool flag, DirectX::XMFLOAT3 player)
{
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setisAttackFlag(flag);
		hand[i]->setTargetPos(player);
	}

}

void bosstest::playerAttackReturnL()
{
	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->playerAttackReturn();
	}
}

void bosstest::setisbossStart(bool flag)
{
	isbossStart = flag;
}

void bosstest::setisbossPunch(bool flag)
{
	isbossPunch = flag;
}

void bosstest::setisBossPress(bool flag)
{
	isBossPress = flag;
}

void bosstest::setisBossStoneFall(bool flag)
{

	isBossStoneFall = flag;

}

void bosstest::setisBossPillarFall(bool flag)
{

	isBossPillarFall = flag;

}

void bosstest::setisBossPillarRoll(bool flag)
{
	isBossPillarRoll = flag;
}

void bosstest::setisBossPushUp(bool flag)
{
	isBossPushUp = flag;
}

void bosstest::setisBossBeam(bool flag)
{
	isBossBeam = flag;
}

void bosstest::setPressPos()
{
	DirectX::XMFLOAT3 bossSft;

	bossSft = { 1,1,-1 };
	bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
	normalize(bossSft);
	hand[0]->setTargetPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	bossSft = { -1,1,-1 };
	bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
	normalize(bossSft);
	hand[1]->setTargetPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	bossSft = { 1,-1,-1 };
	bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
	normalize(bossSft);
	hand[2]->setTargetPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	bossSft = { -1,-1,-1 };
	bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
	normalize(bossSft);
	hand[3]->setTargetPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	bossSft = { 1,1,1 };
	bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
	normalize(bossSft);
	hand[4]->setTargetPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	bossSft = { -1,1,1 };
	bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
	normalize(bossSft);
	hand[5]->setTargetPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	bossSft = { 1,-1,1 };
	bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
	normalize(bossSft);
	hand[6]->setTargetPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	bossSft = { -1,-1,1 };
	bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
	normalize(bossSft);
	hand[7]->setTargetPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	for (int i = 0; i < hand.size(); i++)
	{
		hand[i]->setisPressFlag(true);
	}

}

void bosstest::setPressHandPos()
{

	if (pressFirstStart)
	{
		DirectX::XMFLOAT3 bossSft;
		bossSft = { 1,1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[0]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

		bossSft = { -1,1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[1]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

		bossSft = { 1,-1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[2]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

		bossSft = { -1,-1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[3]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

		bossSft = { 1,1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[4]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

		bossSft = { -1,1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[5]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

		bossSft = { 1,-1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[6]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

		bossSft = { -1,-1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[7]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubePressDistance));

	}
	else if(hand[0]->getisPressEndd())
	{

		DirectX::XMFLOAT3 bossSft;
		
		if (hand[0]->getisAction() == false)
		{
			bossSft = { 1,1,-1 };
			bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
			normalize(bossSft);
			hand[0]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
		}
		if (hand[1]->getisAction() == false)
		{
			bossSft = { -1,1,-1 };
			bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
			normalize(bossSft);
			hand[1]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
		}

		if (hand[2]->getisAction() == false)
		{

			bossSft = { 1,-1,-1 };
			bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
			normalize(bossSft);
			hand[2]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
		}
		
		if (hand[3]->getisAction() == false)
		{
			bossSft = { -1,-1,-1 };
			bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
			normalize(bossSft);
			hand[3]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
		}
		
		if (hand[4]->getisAction() == false)
		{
			bossSft = { 1,1,1 };
			bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
			normalize(bossSft);
			hand[4]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
		}
		
		if (hand[5]->getisAction() == false)
		{
			bossSft = { -1,1,1 };
			bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
			normalize(bossSft);
			hand[5]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
		}
		
		if (hand[6]->getisAction() == false)
		{
			bossSft = { 1,-1,1 };
			bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
			normalize(bossSft);
			hand[6]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
		}
		
		if (hand[7]->getisAction() == false)
		{
			bossSft = { -1,-1,1 };
			bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
			normalize(bossSft);
			hand[7]->setPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
		}
	}
}

void bosstest::setPillarHandPos()
{
	DirectX::XMFLOAT3 bossSft;
	DirectX::XMFLOAT3 bossSftMove;

		//カウント
		if (bosspillarMoveTime< maxBosspillarMoveTime)
		{
			
			bosspillarMoveTime++;
		}

		//とっておいたプレイヤーの座標に移動
		if (bosspillarMoveTime < maxBosspillarMoveTime)
		{
			for (int i = 0; i < hand.size(); i++)
			{

				if (hand[i]->getisAction() == false)
				{
					
					bossSft = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i) % 8];
					bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
					normalize(bossSft);

					bossSftMove = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i + 1) % 8];
					bossSftMove = VectorMat(bossSftMove, worldTransform.worldCoordinate_.matWorld_);
					normalize(bossSftMove);
					hand[i]->setPos(lerp(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance), worldTransform.worldCoordinate_.position_ + (bossSftMove * setbossCubeDistance), bosspillarMoveTime / maxBosspillarMoveTime));
					
				}

			}
		}

		//終了
		if (bosspillarMoveTime == maxBosspillarMoveTime)
		{
			if (bosspillarDefaultPosCount < _countof(pillarDefaultPosRotate)-1)
			{
				bosspillarDefaultPosCount++;
			}
			else
			{
				bosspillarDefaultPosCount = 0;
			}
			bosspillarMoveTime = 0;
		}


}

void bosstest::setOphanimHandPos()
{
	DirectX::XMFLOAT3 bossSft;
	DirectX::XMFLOAT3 bossSftMove;

	//カウント
	if (bossOphanimMoveTime < maxBossOphanimMoveTime)
	{

		bossOphanimMoveTime++;
	}

	//とっておいたプレイヤーの座標に移動
	if (bossOphanimMoveTime < maxBossOphanimMoveTime)
	{
		for (int i = 0; i < hand.size(); i++)
		{

			if (hand[i]->getisAction() == false)
			{

				bossSft = ophanimDefaultPosRotate[(bossOphanimDefaultPosCount + i) % 8];
				bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
				normalize(bossSft);

				bossSftMove = ophanimDefaultPosRotate[(bossOphanimDefaultPosCount + i + 1) % 8];
				bossSftMove = VectorMat(bossSftMove, worldTransform.worldCoordinate_.matWorld_);
				normalize(bossSftMove);
				hand[i]->setPos(lerp(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance), worldTransform.worldCoordinate_.position_ + (bossSftMove * setbossCubeDistance), bossOphanimMoveTime / maxBossOphanimMoveTime));

			}

		}
	}

	//終了
	if (bossOphanimMoveTime == maxBossOphanimMoveTime)
	{
		if (bossOphanimDefaultPosCount < _countof(ophanimDefaultPosRotate) - 1)
		{
			bossOphanimDefaultPosCount++;
		}
		else
		{
			bossOphanimDefaultPosCount = 0;
		}
		bossOphanimMoveTime = 0;
	}


}

void bosstest::OnCollision(int damage)
{

	bossHP -= damage;

}


void bosstest::setCubeDefaultPos()
{

	DirectX::XMFLOAT3 bossSft;

	if (hand[0]->getisAction() == false)
	{
		bossSft = { 1,1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[0]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	}
	if (hand[1]->getisAction() == false)
	{
		bossSft = { -1,1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[1]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	}

	if (hand[2]->getisAction() == false)
	{

		bossSft = { 1,-1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[2]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	}

	if (hand[3]->getisAction() == false)
	{
		bossSft = { -1,-1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[3]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	}

	if (hand[4]->getisAction() == false)
	{
		bossSft = { 1,1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[4]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	}

	if (hand[5]->getisAction() == false)
	{
		bossSft = { -1,1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[5]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	}

	if (hand[6]->getisAction() == false)
	{
		bossSft = { 1,-1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[6]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	}

	if (hand[7]->getisAction() == false)
	{
		bossSft = { -1,-1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[7]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	}

	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisAction() == false)
		{
			hand[i]->setScale({ 3.0f,3.0f,3.0f });
		}

	}

}

void bosstest::setPillarDefaultPos()
{

	DirectX::XMFLOAT3 bossSft;
	DirectX::XMFLOAT3 bossSftMove;
	//とっておいたプレイヤーの座標に移動
	if (bosspillarMoveTime < maxBosspillarMoveTime)
	{
		for (int i = 0; i < hand.size(); i++)
		{
			if (hand[i]->getisAction() == false)
			{
				bossSft = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i) % 8];
				bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
				normalize(bossSft);

				bossSftMove = pillarDefaultPosRotate[(bosspillarDefaultPosCount + i + 1) % 8];
				bossSftMove = VectorMat(bossSftMove, worldTransform.worldCoordinate_.matWorld_);
				normalize(bossSftMove);
				hand[i]->setdefaultPos(lerp(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance), worldTransform.worldCoordinate_.position_ + (bossSftMove * setbossCubeDistance), bosspillarMoveTime / maxBosspillarMoveTime));
			}
		}
	}

	

	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisAction() == false)
		{
			hand[i]->setScale({ 1.0f,10.0f,1.0f });
		}

	}

}

void bosstest::setOphanimDefaultPos()
{

	DirectX::XMFLOAT3 bossSft;
	DirectX::XMFLOAT3 bossSftMove;
	//とっておいたプレイヤーの座標に移動
	if (bosspillarMoveTime < maxBosspillarMoveTime)
	{
		for (int i = 0; i < hand.size(); i++)
		{
			if (hand[i]->getisAction() == false)
			{
				bossSft = ophanimDefaultPosRotate[(bosspillarDefaultPosCount + i) % 8];
				bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
				normalize(bossSft);

				bossSftMove = ophanimDefaultPosRotate[(bosspillarDefaultPosCount + i + 1) % 8];
				bossSftMove = VectorMat(bossSftMove, worldTransform.worldCoordinate_.matWorld_);
				normalize(bossSftMove);
				hand[i]->setdefaultPos(lerp(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance), worldTransform.worldCoordinate_.position_ + (bossSftMove * setbossCubeDistance), bosspillarMoveTime / maxBosspillarMoveTime));
			}
		}
	}



	for (int i = 0; i < hand.size(); i++)
	{

		if (hand[i]->getisAction() == false)
		{
			hand[i]->setScale({ 2.0f,2.0f,2.0f });
		}

	}

}


void bosstest::setPressEnd()
{

	
	DirectX::XMFLOAT3 bossSft;

		bossSft = { 1,1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[0]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	
		bossSft = { -1,1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[1]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	
		bossSft = { 1,-1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[2]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));

		bossSft = { -1,-1,-1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[3]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	
		bossSft = { 1,1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[4]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	
		bossSft = { -1,1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[5]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	
		bossSft = { 1,-1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[6]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));
	
		bossSft = { -1,-1,1 };
		bossSft = VectorMat(bossSft, worldTransform.worldCoordinate_.matWorld_);
		normalize(bossSft);
		hand[7]->setdefaultPos(worldTransform.worldCoordinate_.position_ + (bossSft * setbossCubeDistance));

}

//DirectX::XMFLOAT2 bosstest::GetStartTimer()
//{
//	DirectX::XMFLOAT2 timekasu = { timeCount,maxbossStartTime };
//
//	return timekasu;
//}



int RNG(int min, int max, bool preciseMode)
{
	if (!preciseMode) {
		return (rand() % (max + 1 - min) + min);
	}

	int ret = 0;
	do {
		ret = rand();
	} while (ret >= RAND_MAX - RAND_MAX % (max + 1 - min));
	ret = ret % (max + 1 - min) + min;
	return ret;
}