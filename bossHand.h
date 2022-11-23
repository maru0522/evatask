#pragma once
#include "Obj3d.h"
#include "Camera.h"
#include "WorldCoordinate.h"

class bossHand
{
public:

	bossHand();

	void init(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT3 translation,Camera* camera);

	void update(WorldCoordinate worldTransform);

	void draw();

	void reset();

	bool getisAttackFlag() { return isAttackFlag; };

	bool getisReturnHand() { return isReturnHand; };
	
	bool getisPressEndd() { return isPressEnd; };

	bool getisAction() { return isAction; };

	bool getisFallTargetMoveFlag() { return isFallTargetMoveFlag; };

	bool getisGetTargetPosFlag() { return isGetTargetPos; };

	void punch(WorldCoordinate worldTransform);

	void press();

	void stoneFall();
	void pillarFall();

	void beam();


	void pillarRoll();

	void pillarPushUp();

	//����Ńt���O��true�ɂ�������Ƌ��������������Ȃ�̂Œ���(�g������߂�����)
	void setisActionFlag(bool flag);

	void setisAttackFlag(bool flag);

	void setisPressFlag(bool flag);

	void setisPressEndFlag(bool flag);

	void setisStoneFallFlag(bool flag);

	void setisPillarFallFlag(bool flag);

	void setisBeamFlag(bool flag);
	
	void setisPillarRollFlag(bool flag);

	void setisPillarPushUpFlag(bool flag);

	//�v���C���[�̍U���ɂ���Ē��˕Ԃ��ꂽ���P�񂾂��s������
	void playerAttackReturn();

	void punchEnd();

	void setdefaultPos(DirectX::XMFLOAT3 pos) { defaultPos = pos; };

	WorldCoordinate GetwroldTransform() { return Hand.worldCoordinate_; }

	void setPos(DirectX::XMFLOAT3 pos);

	void setScale(DirectX::XMFLOAT3 scale);

	void setRotate(DirectX::XMFLOAT3 rotate);

	void setTargetPos(DirectX::XMFLOAT3 target);

private:
	Obj3d Hand=Obj3d("Resources/3dModels/muso/muso.obj");

	//�肪�U������
	bool isAttackFlag = false;
	//�U�����I����Ė߂��Ă��鎞��
	bool isAttackReturnFlag = false;

	bool isFallTargetMoveFlag = false;
	bool isFallFallFlag = false;
	bool isFallReturnFlag = false;

	bool isPillarFallReturnFlag = false;

	bool isBeamFirstStart = false;

	bool isBeamEnd = false;

	bool isPillarRollFirstStart = false;
	

	bool isPushUpTargetMoveFlag = false;
	bool isPushUpUpFlag = false;
	bool isPushUpFallFlag = false;
	bool isPushUpReturnFlag = false;
	bool isPushUpReturnUpFlag = false;

	bool isGetTargetPos = false;

	//��𒵂˕Ԃ������̃t���O
	bool isReturnHand = false;

	//�^�C���J�E���^�[
	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;
	float ActionType4TimeCount = 0;
	float ActionType5TimeCount = 0;
	float ActionType6TimeCount = 0;

	//���쎞��
	float maxTime = 20.0f;
	float maxReturnTime = 20.0f;
	float maxReturnAttackTime = 10.0f;

	//�v���X
	float maxSetPressTime = 20.0f;
	float maxResetPressTime = 50.0f;

	//�Η��Ƃ�
	float maxTargetMoveTime = 25.0f;
	float maxFallTime = 10.0f;
	float maxFallReturnTime = 10.0f;
	float maxUpFallTime = 10.0f;

	//���i�K�p�Η��Ƃ�
	float maxUpPillarFallTime = 15.0f;

	//�r�[��
	float maxBeamTime = 25.0f;
	float maxFirstBeamTime = 20.0f;

	//��]
	float maxPillarRollTime = 30.0f;

	//�˂��グ
	float maxPushUpFallTime = 10.0f;
	float maxPushUpTargetMoveTime = 25.0f;
	float maxPushUpUpTime = 10.0f;
	float maxPushUpUpReturnTime = 20.0f;
	float maxPushUpReturnTime = 10.0f;
	float maxPushUpDefaultUpTime = 20.0f;

	//�E�G�C�g
	float waitTime = 0;

	//�҂���
	float attackWaitTime = 40;
	float returnWaitTime = 120;

	float stoneFallWaitTime = 40;
	float stoneFallReturnWaitTime = 10;

	float PushUpWaitTime = 40;
	float PushUpReturnWaitTime = 10;

	//���s�����Ă��邩
	bool isAction = false;
	bool isStoneFallAction = false;

	//
	bool isPress = false;
	bool isPressEnd = true;
	bool isStoneFall = false;
	bool isPillarFall = false;
	bool isPillarRoll = false;
	bool isBeam = false;
	bool isPillarPushUp = false;

	//�v���C���[�̈ʒu
	DirectX::XMFLOAT3 targetPos = {};

	//�U�����Ԃ��ꂽ�ʒu
	DirectX::XMFLOAT3 returnPos = {};

	DirectX::XMFLOAT3 defaultPos = {};

	DirectX::XMFLOAT3 startBeamScale = { 5,5,5 };

	
};

const DirectX::XMFLOAT3 lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const float t);

const DirectX::XMFLOAT3 operator-= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);

const DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s);

const DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v);

const DirectX::XMFLOAT3 operator+= (DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);

float length(DirectX::XMFLOAT3& a);
void normalize(DirectX::XMFLOAT3& a);
