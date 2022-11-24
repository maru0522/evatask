#include "bossHand.h"
#include <cassert>

const DirectX::XMFLOAT3 operator-= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{

	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	DirectX::XMFLOAT3 temp(v1);
	return temp -= v2;
}

const DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& v, float s)
{

	v.x *= s;
	v.y *= s;
	v.z *= s;

	return v;
}

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s)
{
	DirectX::XMFLOAT3 temp(v);
	return temp *= s;
}

const DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v)
{
	return v * s;
}

const DirectX::XMFLOAT3 operator+= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{

	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	DirectX::XMFLOAT3 temp(v1);
	return temp += v2;
}

float length(DirectX::XMFLOAT3& a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

void normalize(DirectX::XMFLOAT3& a)
{
	float len = length(a);
	if (len != 0)
	{
		a.x /= len;
		a.y /= len;
		a.z /= len;
		return;
	}

	return;
}

//�P����!=
bool vector3IsDiffer(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

//�P����==
bool vector3Issame(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);

bossHand::bossHand()
{
	
}

void bossHand::init(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT3 translation, Camera* camera)
{

	Hand= Obj3d("Resources/3dModels/muso/muso.obj", camera);

	Hand.worldCoordinate_.scale_ = scale;
	Hand.worldCoordinate_.rotation_ = rotate;
	Hand.worldCoordinate_.position_ = translation;

	Hand.Update();
}

void bossHand::update(WorldCoordinate worldTransform)
{

	Hand.Update();

	punch(worldTransform);
	press();
	stoneFall();
	beam();
	pillarFall();
	pillarRoll();
	pillarPushUp();

}

void bossHand::draw()
{

	Hand.Draw();

}

void bossHand::reset()
{

	//�肪�U������
	isAttackFlag = false;
	//�U�����I����Ė߂��Ă��鎞��
	isAttackReturnFlag = false;

	isFallTargetMoveFlag = false;
	isFallFallFlag = false;
	isFallReturnFlag = false;

	isPillarFallReturnFlag = false;

	isBeamFirstStart = false;

	isBeamEnd = false;

	isPillarRollFirstStart = false;


	isPushUpTargetMoveFlag = false;
	isPushUpUpFlag = false;
	isPushUpFallFlag = false;
	isPushUpReturnFlag = false;
	isPushUpReturnUpFlag = false;

	isGetTargetPos = false;

	//��𒵂˕Ԃ������̃t���O
	isReturnHand = false;

	//�^�C���J�E���^�[
	timeCount = 0;
	returnTimeCount = 0;
	returnAttackTimeCount = 0;
	ActionType4TimeCount = 0;
	ActionType5TimeCount = 0;
	ActionType6TimeCount = 0;

	//���s�����Ă��邩
	isAction = false;
	isStoneFallAction = false;

	//
	isPress = false;
	isPressEnd = true;
	isStoneFall = false;
	isPillarFall = false;
	isPillarRoll = false;
	isBeam = false;
	isPillarPushUp = false;

}

void bossHand::punch(WorldCoordinate worldTransform)
{

	if (isReturnHand==false)
	{
		if (isAttackFlag)
		{
			isAction = true;
			//�J�E���g
			if (timeCount < maxTime and waitTime > attackWaitTime)
			{
				timeCount++;
			}

			//�\������
			if (waitTime < attackWaitTime and isAttackReturnFlag == false)
			{

				Hand.worldCoordinate_.position_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

				Hand.Update();

			}

			//�U��
			if (timeCount != maxTime and isAttackReturnFlag == false and waitTime > attackWaitTime)
			{


				//���̈ʒu����v���C���[�̈ʒu(���݂�0,0,0�ŏI�I�ɑ_�������肵�Ă��炻���̈ʒu��)�ɐ��`���
				Hand.worldCoordinate_.position_ = lerp(defaultPos, targetPos, timeCount / maxTime);

				Hand.Update();
			}
			else if (isAttackReturnFlag == false and waitTime > attackWaitTime)
			{
				isAttackReturnFlag = true;
				waitTime = 0;
				XAudio::PlayWave(SE_BOSSAttack, 1.0f);
			}

			//�J�E���g
			if (isAttackReturnFlag and returnTimeCount < maxReturnTime and waitTime > returnWaitTime)
			{
				returnTimeCount++;
			}

			//�A�҂̗\������(80�͓����Ȃ�����)
			if (waitTime > 80 and waitTime < returnWaitTime and isAttackReturnFlag)
			{

				Hand.worldCoordinate_.position_ = { cosf(waitTime) / 10 + targetPos.x,  targetPos.y,  targetPos.z };

				Hand.Update();

			}

			//�A��
			if (isAttackReturnFlag and returnTimeCount != maxReturnTime and waitTime > returnWaitTime)
			{

				Hand.worldCoordinate_.position_ = lerp(targetPos, defaultPos, returnTimeCount / maxReturnTime);
				Hand.Update();
			}

			waitTime++;

			//�I��
			if (returnTimeCount == maxReturnTime)
			{
				punchEnd();
			}

			

		}
	}

	//�U���ɂ���ĕԂ��ꂽ��
	if(isReturnHand)
	{

		//�J�E���g
		if (returnAttackTimeCount < maxReturnAttackTime)
		{
			returnAttackTimeCount++;
		}

		//�{�̂̂��Ƃ֋A��
		Hand.worldCoordinate_.position_ = lerp(returnPos, worldTransform.position_, returnAttackTimeCount / maxReturnAttackTime);
		Hand.Update();

		//�{�̂ɓ��������猳�̈ʒu��
		if (returnAttackTimeCount == maxReturnAttackTime)
		{
			Hand.worldCoordinate_.position_ = { defaultPos};
			Hand.Update();
			punchEnd();
		}


	}

	/*debugText_->SetPos(50, 90);
	debugText_->Printf("leftHandpos:(%f,%f,%f)", Hand.translation_.x, Hand.translation_.y, Hand.translation_.z);*/

}

void bossHand::press()
{
	if (isPress)
	{
		isAction = true;
		isPressEnd = false;
		if (timeCount < maxSetPressTime)
		{
			timeCount++;
		}

		if (timeCount != maxSetPressTime)
		{


			//���̈ʒu����v���C���[�̈ʒu(���݂�0,0,0�ŏI�I�ɑ_�������肵�Ă��炻���̈ʒu��)�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp(defaultPos, targetPos, timeCount / maxSetPressTime);

			Hand.Update();
		}
	}
	else
	{

		if (vector3IsDiffer(defaultPos, Hand.worldCoordinate_.position_) and isPressEnd ==false)
		{

			if (returnTimeCount < maxResetPressTime)
			{
				returnTimeCount++;
			}

			//���̈ʒu����v���C���[�̈ʒu(���݂�0,0,0�ŏI�I�ɑ_�������肵�Ă��炻���̈ʒu��)�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp(targetPos, defaultPos, returnTimeCount / maxResetPressTime);

			Hand.Update();

			

		}

		if (!vector3IsDiffer(defaultPos, Hand.worldCoordinate_.position_) and isPressEnd == false)
		{

			if (returnTimeCount == maxResetPressTime)
			{
				returnTimeCount = 0;
			}

			isPressEnd = true;

			timeCount = 0;

			isAction = false;

		}



	}

}

void bossHand::stoneFall()
{

	if (isStoneFall)
	{
		isAction = true;
		//�J�E���g
		if (timeCount < maxUpFallTime and waitTime > stoneFallWaitTime and isFallTargetMoveFlag == false)
		{
			timeCount++;
		}

		//�\������
		if (waitTime < stoneFallWaitTime and isFallTargetMoveFlag == false)
		{

			Hand.worldCoordinate_.position_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

			Hand.Update();

		}

		//��Ɉړ�
		if (timeCount < maxUpFallTime and isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{


			//���̈ʒu��y���W�����炵���ʒu�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp(defaultPos, {defaultPos.x,15,defaultPos.z}, timeCount / maxUpFallTime);

			Hand.Update();
		}
		//��Ɉړ����I�������
		else if (isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{
			isFallTargetMoveFlag = true;
			waitTime = 0;
		}

		//�v���C���[�̍��W�v��
		if (isFallTargetMoveFlag and returnTimeCount == 0 and isGetTargetPos == false)
		{
			isGetTargetPos = true;
			return;
		}
		else if (isFallTargetMoveFlag and returnTimeCount != 0)
		{
			isGetTargetPos = false;
		}


		//�J�E���g
		if (isFallTargetMoveFlag and returnTimeCount < maxTargetMoveTime and isFallFallFlag ==false)
		{
			//�g���܂킵���O�ɈӖ��͂Ȃ�
			returnTimeCount++;
		}

		//�Ƃ��Ă������v���C���[�̍��W�Ɉړ�
		if (isFallTargetMoveFlag and returnTimeCount != maxTargetMoveTime and isFallFallFlag == false)
		{

			Hand.worldCoordinate_.position_ = lerp({ defaultPos.x,15,defaultPos.z }, {targetPos.x,15,targetPos.z}, returnTimeCount / maxTargetMoveTime);
			Hand.Update();
		}

		//�I��
		if (returnTimeCount == maxTargetMoveTime and isFallFallFlag == false)
		{
			isFallFallFlag = true;
			waitTime = 0;
		}

		//�J�E���g
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isFallReturnFlag == false)
		{
			ActionType4TimeCount++;
		}

		//����
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isFallReturnFlag == false)
		{


			//���Ɉړ�
			Hand.worldCoordinate_.position_ = lerp({ targetPos.x,15,targetPos.z }, {targetPos.x,targetPos.y+1.1f,targetPos.z}, ActionType4TimeCount / maxFallTime);

			Hand.Update();
		}

		if (ActionType4TimeCount == maxFallTime and isFallReturnFlag == false)
		{
			isFallReturnFlag = true;
			waitTime = 0;
			XAudio::PlayWave(SE_BOSSAttack, 1.0f);
		}


		//���S�Ɏ~�߂������Ԃ�300
		if (waitTime > 150 and returnAttackTimeCount < maxFallReturnTime and waitTime > (stoneFallReturnWaitTime +150) and isFallReturnFlag)
		{
			returnAttackTimeCount++;
		}

		//�\������
		if (waitTime > 150 and waitTime < (stoneFallReturnWaitTime + 150) and isFallReturnFlag)
		{

			Hand.worldCoordinate_.position_ = { cosf(waitTime) + targetPos.x, targetPos.y+1.8f, targetPos.z };

			Hand.Update();

		}

		//��Ɉړ�
		if (returnAttackTimeCount < maxFallReturnTime and isFallReturnFlag and waitTime > (stoneFallReturnWaitTime + 150))
		{


			//���̈ʒu��y���W�����炵���ʒu�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp(targetPos, defaultPos, returnAttackTimeCount / maxFallReturnTime);

			Hand.Update();
		}


		waitTime++;

		if (returnAttackTimeCount == maxFallReturnTime)
		{
			
			isAction = false;
			isFallTargetMoveFlag = false;
			isFallFallFlag = false;
			isFallReturnFlag = false;

			timeCount = 0;
			returnTimeCount = 0;
			ActionType4TimeCount = 0;
			returnAttackTimeCount = 0;

			waitTime = 0;

			isStoneFall = false;

		}

	}

}

void bossHand::pillarFall()
{

	if (isPillarFall)
	{
		isAction = true;
		//�J�E���g
		if (timeCount < maxUpFallTime and waitTime > stoneFallWaitTime and isFallTargetMoveFlag == false)
		{
			timeCount++;
		}

		//�\������
		if (waitTime < stoneFallWaitTime and isFallTargetMoveFlag == false)
		{

			Hand.worldCoordinate_.position_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

			Hand.Update();

		}

		//��Ɉړ�
		if (timeCount < maxUpFallTime and isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{


			//���̈ʒu��y���W�����炵���ʒu�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp(defaultPos, { defaultPos.x,30,defaultPos.z }, timeCount / maxUpFallTime);

			Hand.Update();
		}
		//��Ɉړ����I�������
		else if (isFallTargetMoveFlag == false and waitTime > stoneFallWaitTime)
		{
			isFallTargetMoveFlag = true;
			waitTime = 0;
		}

		//�v���C���[�̍��W�v��
		if (isFallTargetMoveFlag and returnTimeCount == 0 and isGetTargetPos == false)
		{
			isGetTargetPos = true;
			return;
		}
		else if (isFallTargetMoveFlag and returnTimeCount != 0)
		{
			isGetTargetPos = false;
		}


		//�J�E���g
		if (isFallTargetMoveFlag and returnTimeCount < maxTargetMoveTime and isFallFallFlag == false)
		{
			//�g���܂킵���O�ɈӖ��͂Ȃ�
			returnTimeCount++;
		}

		//�Ƃ��Ă������v���C���[�̍��W�Ɉړ�
		if (isFallTargetMoveFlag and returnTimeCount != maxTargetMoveTime and isFallFallFlag == false)
		{

			Hand.worldCoordinate_.position_ = lerp({ defaultPos.x,30,defaultPos.z }, { targetPos.x,30,targetPos.z }, returnTimeCount / maxTargetMoveTime);
			Hand.Update();
		}

		//�I��
		if (returnTimeCount == maxTargetMoveTime and isFallFallFlag == false)
		{
			isFallFallFlag = true;
			waitTime = 0;
		}

		//�J�E���g
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isPillarFallReturnFlag == false)
		{
			ActionType4TimeCount++;
		}

		//����
		if (ActionType4TimeCount < maxFallTime and isFallFallFlag and isPillarFallReturnFlag == false)
		{


			//���Ɉړ�
			Hand.worldCoordinate_.position_ = lerp({ targetPos.x,30,targetPos.z }, { targetPos.x,targetPos.y + 6.1f,targetPos.z }, ActionType4TimeCount / maxFallTime);

			Hand.Update();
		}

		if (ActionType4TimeCount == maxFallTime and isPillarFallReturnFlag == false)
		{
			isPillarFallReturnFlag = true;
			waitTime = 0;
			XAudio::PlayWave(SE_BOSSAttack, 1.0f);
		}


		//���S�Ɏ~�߂������Ԃ�150
		if (waitTime > 150 and ActionType5TimeCount < maxUpPillarFallTime and waitTime >(stoneFallReturnWaitTime + 150) and isPillarFallReturnFlag and isFallReturnFlag==false)
		{
			ActionType5TimeCount++;
		}

		//�\������
		if (waitTime > 150 and waitTime < (stoneFallReturnWaitTime + 150) and isPillarFallReturnFlag and isFallReturnFlag == false)
		{

			Hand.worldCoordinate_.position_ = { cosf(waitTime) + targetPos.x, targetPos.y + 6.8f, targetPos.z };

			Hand.Update();

		}

		//��Ɉړ�
		if (ActionType5TimeCount < maxUpPillarFallTime and isPillarFallReturnFlag and waitTime >(stoneFallReturnWaitTime + 150) and isFallReturnFlag == false)
		{


			//���̈ʒu��y���W�����炵���ʒu�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp({ targetPos.x,targetPos.y + 11.1f,targetPos.z }, { targetPos.x,15,targetPos.z }, ActionType5TimeCount / maxUpPillarFallTime);

			Hand.Update();
		}

		if (ActionType5TimeCount == maxUpPillarFallTime and isFallReturnFlag == false)
		{
			isFallReturnFlag = true;
			waitTime = 0;
		}

		//�J�E���g
		if (returnAttackTimeCount < maxFallReturnTime and isFallReturnFlag)
		{
			returnAttackTimeCount++;
		}

		//�߂�
		if (returnAttackTimeCount < maxFallReturnTime and isFallReturnFlag)
		{


			//���Ɉړ�
			Hand.worldCoordinate_.position_ = lerp({ targetPos.x,30,targetPos.z }, defaultPos, returnAttackTimeCount / maxFallReturnTime);

			Hand.Update();
		}


		waitTime++;

		if (returnAttackTimeCount == maxFallReturnTime)
		{

			isAction = false;
			isFallTargetMoveFlag = false;
			isFallFallFlag = false;
			isFallReturnFlag = false;
			isPillarFallReturnFlag = false;

			timeCount = 0;
			returnTimeCount = 0;
			ActionType4TimeCount = 0;
			ActionType5TimeCount = 0;
			returnAttackTimeCount = 0;

			waitTime = 0;

			isPillarFall = false;

		}

	}

}

void bossHand::beam()
{

	if (isBeam)
	{

		isAction = true;


		if (isBeamFirstStart == false)
		{

			Hand.worldCoordinate_.scale_ = startBeamScale;

			//���݈ʒu�擾
			returnPos = Hand.worldCoordinate_.position_;

			Hand.Update();
			isBeamFirstStart = true;

		}
		else if(returnTimeCount < maxFirstBeamTime)
		{

			returnTimeCount++;
			Hand.worldCoordinate_.position_ = lerp(returnPos, targetPos, returnTimeCount / maxFirstBeamTime);
			Hand.Update();

		}


		//�J�E���g
		if (timeCount < maxBeamTime and waitTime > 30)
		{
			timeCount++;
		}

		if (timeCount != maxBeamTime and waitTime > 30)
		{

			Hand.worldCoordinate_.position_ = lerp(targetPos, { targetPos.x,targetPos.y,targetPos.z-600 }, timeCount / maxBeamTime);
			Hand.Update();
		}

		//�I��

		if (timeCount == maxBeamTime)
		{
			Hand.worldCoordinate_.scale_ = { 1.0f,1.0f,1.0f };
			Hand.Update();

			isAction = false;
			isBeamFirstStart = false;
			isBeamEnd = false;
			isBeam = false;

			timeCount = 0;
			waitTime = 0;
			returnTimeCount = 0;
		}

		if (returnTimeCount == maxFirstBeamTime)
		{
			waitTime++;
		}

	}




}

void bossHand::pillarRoll()
{
	if (isPillarRoll)
	{

		if (isPillarRollFirstStart == false)
		{
			returnPos = Hand.worldCoordinate_.position_;
			isPillarRollFirstStart = true;
		}

		if (timeCount < maxPillarRollTime)
		{
			timeCount++;
		}

		if (timeCount < maxPillarRollTime)
		{


			//���̈ʒu����v���C���[�̈ʒu(���݂�0,0,0�ŏI�I�ɑ_�������肵�Ă��炻���̈ʒu��)�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp(returnPos, defaultPos, timeCount / maxPillarRollTime);

			Hand.Update();
		}

		if (timeCount == maxPillarRollTime)
		{

			isPillarRollFirstStart = false;
			timeCount = 0;
			isAction = false;
			isPillarRoll = false;
		}

	}
}

void bossHand::pillarPushUp()
{

	if (isPillarPushUp)
	{
		isAction = true;
		//�J�E���g
		if (timeCount < maxPushUpFallTime and waitTime > PushUpWaitTime and isPushUpTargetMoveFlag == false)
		{
			timeCount++;
		}

		//�\������
		if (waitTime < maxPushUpFallTime and isPushUpTargetMoveFlag == false)
		{

			Hand.worldCoordinate_.position_ = { cosf(waitTime) + defaultPos.x, defaultPos.y, defaultPos.z };

			Hand.Update();

		}

		//���Ɉړ�
		if (timeCount < maxUpFallTime and isPushUpTargetMoveFlag == false and waitTime > PushUpWaitTime)
		{


			//���̈ʒu��y���W�����炵���ʒu�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp(defaultPos, { defaultPos.x,-18,defaultPos.z }, timeCount / maxPushUpFallTime);

			Hand.Update();
		}
		//���Ɉړ����I�������
		else if (isPushUpTargetMoveFlag == false and waitTime > PushUpWaitTime)
		{
			isPushUpTargetMoveFlag = true;
			waitTime = 0;
		}

		//�J�E���g
		if (isPushUpTargetMoveFlag and returnTimeCount < maxPushUpTargetMoveTime and isPushUpUpFlag == false)
		{
			//�g���܂킵���O�ɈӖ��͂Ȃ�
			returnTimeCount++;
		}

		//�Ƃ��Ă������v���C���[�̍��W�Ɉړ�
		if (isPushUpTargetMoveFlag and returnTimeCount < maxPushUpTargetMoveTime and isPushUpUpFlag == false)
		{

			Hand.worldCoordinate_.position_ = lerp({ defaultPos.x,-18,defaultPos.z }, { targetPos.x,-18,targetPos.z }, returnTimeCount / maxPushUpTargetMoveTime);
			Hand.Update();
		}

		//�I��
		if (returnTimeCount == maxPushUpTargetMoveTime and isPushUpUpFlag == false)
		{
			isPushUpUpFlag = true;
		}

		//�J�E���g
		if (ActionType4TimeCount < maxPushUpUpTime and isPushUpUpFlag and isPushUpFallFlag == false)
		{
			ActionType4TimeCount++;
		}

		//����
		if (ActionType4TimeCount < maxPushUpUpTime and isPushUpUpFlag and isPushUpFallFlag == false)
		{


			//���Ɉړ�
			Hand.worldCoordinate_.position_ = lerp({ targetPos.x,-18,targetPos.z }, { targetPos.x,targetPos.y + 1.1f,targetPos.z }, ActionType4TimeCount / maxPushUpUpTime);

			Hand.Update();
		}

		if (ActionType4TimeCount == maxPushUpUpTime and isPushUpFallFlag == false)
		{
			isPushUpFallFlag = true;
			XAudio::PlayWave(SE_BOSSAttack, 1.0f);
		}


		//���S�Ɏ~�߂������Ԃ�150
		if (waitTime > 150 and ActionType5TimeCount < maxPushUpUpReturnTime and waitTime >(PushUpReturnWaitTime + 150) and isPushUpFallFlag and isPushUpReturnFlag == false)
		{
			ActionType5TimeCount++;
		}

		//�\������
		if (waitTime > 150 and waitTime < (PushUpReturnWaitTime + 150) and isPushUpFallFlag and isPushUpReturnFlag == false)
		{

			Hand.worldCoordinate_.position_ = { cosf(waitTime) + targetPos.x, targetPos.y + 1.1f, targetPos.z };

			Hand.Update();

		}

		//��Ɉړ�
		if (ActionType5TimeCount < maxPushUpUpReturnTime and isPushUpFallFlag and waitTime >(PushUpReturnWaitTime + 150) and isPushUpReturnFlag == false)
		{


			//���̈ʒu��y���W�����炵���ʒu�ɐ��`���
			Hand.worldCoordinate_.position_ = lerp({ targetPos.x,targetPos.y + 1.1f,targetPos.z }, { targetPos.x,-18,targetPos.z }, ActionType5TimeCount / maxPushUpUpReturnTime);

			Hand.Update();
		}

		if (ActionType5TimeCount == maxPushUpUpReturnTime and isPushUpReturnFlag == false)
		{
			isPushUpReturnFlag = true;
			waitTime = 0;
		}

		//�J�E���g
		if (returnAttackTimeCount < maxPushUpReturnTime and isPushUpReturnFlag and isPushUpReturnUpFlag == false)
		{
			returnAttackTimeCount++;
		}

		//�߂�
		if (returnAttackTimeCount < maxPushUpReturnTime and isPushUpReturnFlag and isPushUpReturnUpFlag == false)
		{


			//���Ɉړ�
			Hand.worldCoordinate_.position_ = lerp({ targetPos.x,-18,targetPos.z }, { defaultPos.x,-18,defaultPos.z }, returnAttackTimeCount / maxPushUpReturnTime);

			Hand.Update();
		}

		if (returnAttackTimeCount == maxPushUpReturnTime and isPushUpReturnUpFlag == false)
		{
			isPushUpReturnUpFlag = true;
		}

		//�J�E���g
		if (ActionType6TimeCount < maxPushUpDefaultUpTime and isPushUpReturnUpFlag)
		{
			ActionType6TimeCount++;
		}

		//�߂�
		if (ActionType6TimeCount < maxPushUpDefaultUpTime and isPushUpReturnUpFlag)
		{


			//���Ɉړ�
			Hand.worldCoordinate_.position_ = lerp({ defaultPos.x,-18,defaultPos.z }, defaultPos, ActionType6TimeCount / maxPushUpDefaultUpTime);

			Hand.Update();
		}

		waitTime++;

		if (ActionType6TimeCount == maxPushUpDefaultUpTime)
		{

			isAction = false;
			isPushUpTargetMoveFlag = false;
			isPushUpUpFlag = false;
			isPushUpFallFlag = false;
			isPushUpReturnFlag = false;
			isPushUpReturnUpFlag = false;

			timeCount = 0;
			returnTimeCount = 0;
			ActionType4TimeCount = 0;
			ActionType5TimeCount = 0;
			ActionType6TimeCount = 0;
			returnAttackTimeCount = 0;

			waitTime = 0;

			isPillarPushUp = false;

		}

	}

}

void bossHand::setisActionFlag(bool flag) 
{
	isAction = flag;
}

void bossHand::setisAttackFlag(bool flag)
{
	isAttackFlag = flag;
}

void bossHand::setisPressFlag(bool flag)
{
	isPress = flag;
}

void bossHand::setisPressEndFlag(bool flag)
{
	isPressEnd = flag;
}

void bossHand::setisStoneFallFlag(bool flag)
{
	isStoneFall = flag;
}

void bossHand::setisPillarFallFlag(bool flag)
{
	isPillarFall = flag;
}

void  bossHand::setisBeamFlag(bool flag)
{
	isBeam = flag;
}


void bossHand::setisPillarRollFlag(bool flag)
{
	isPillarRoll = flag;
}

void bossHand::setisPillarPushUpFlag(bool flag)
{
	isPillarPushUp = flag;
}

void bossHand::playerAttackReturn()
{

	isReturnHand = true;
	returnPos = Hand.worldCoordinate_.position_;

}

void bossHand::punchEnd()
{
	isAction = false;
	isReturnHand = false;
	isAttackFlag = false;
	isAttackReturnFlag = false;
	returnAttackTimeCount = 0;
	timeCount = 0;
	returnTimeCount = 0;
	waitTime = 0;

}

void bossHand::setPos(DirectX::XMFLOAT3 pos)
{
	Hand.worldCoordinate_.position_ = pos;

	Hand.Update();
}

void bossHand::setScale(DirectX::XMFLOAT3 scale)
{

	Hand.worldCoordinate_.scale_ = scale;

	Hand.Update();

}

void bossHand::setRotate(DirectX::XMFLOAT3 rotate)
{
	
	Hand.worldCoordinate_.rotation_ = rotate;

	Hand.Update();

}

void bossHand::setTargetPos(DirectX::XMFLOAT3 target)
{
	targetPos = target;
}

const DirectX::XMFLOAT3 lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const float t)
{
	return start + t * (end - start);
}

bool vector3IsDiffer(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	if (a.x != b.x or a.y != b.y or a.z != b.z)
	{
		return true;
	}

	return false;
}

bool vector3Issame(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{

	if (a.x == b.x and a.y == b.y and a.z == b.z)
	{
		return true;
	}

	return false;

}