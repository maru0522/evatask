#pragma once
#include "WorldCoordinate.h"
#include "Obj3d.h"
#include "bossHand.h"
#include "RailCamera.h"
#include <memory>
#include <vector>

class bosstest
{
public:
	bosstest();
	~bosstest();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initialize(RailCamera* camera, DirectX::XMFLOAT3 pos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(DirectX::XMFLOAT3 player);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw();

	void cubeActionDecision();

	//���[���h���W���擾
	DirectX::XMFLOAT3 GetWorldPosition();

	//�{�X�{�̂̍��W��ς���
	void setPos(DirectX::XMFLOAT3 pos);

	void setRotate(DirectX::XMFLOAT3 rotate);

	//hand�̃p���`�����邽�߂̃t���O��ύX
	void setisAttackFlagL(bool flag, DirectX::XMFLOAT3 player);
	void setisbossPunch(bool flag);

	//�v���X�U�������邽�߂̃t���O��ύX
	void setisBossPress(bool flag);

	//�Η��Ƃ������邽�߂̃t���O��ύX
	void setisBossStoneFall(bool flag);
	void setisBossPillarFall(bool flag);
	

	void setisBossBeam(bool flag);

	void setisBossPillarRoll(bool flag);

	void setisBossPushUp(bool flag);

	//���̏�Ńp���`�̍U���𒵂˕Ԃ������Ƃɂ���
	void playerAttackReturnL();

	void bossPunch(DirectX::XMFLOAT3 player);

	//�v���X�U��
	void bossPress(DirectX::XMFLOAT3 player);

	//�Η��Ƃ�
	void bossStoneFall(DirectX::XMFLOAT3 player);
	void bossPillarFall(DirectX::XMFLOAT3 player);

	void bossBeam();

	void bossPillarRoll();

	void bossPushUp(DirectX::XMFLOAT3 player);

	//pos???????????? �S�������Ă���
	WorldCoordinate getPos() { return worldTransform.worldCoordinate_; };

	//hand�������Ă���
	std::vector<bossHand*> getHand() { return hand; };

	//�v���X�U��������Ƃ��̖ڕW���W�ړ��p�֐�
	void setPressPos();

	//�{�̂ƈꏏ��hand���������߂̊֐�
	void setPressHandPos();
	void setPillarHandPos();
	void setOphanimHandPos();

	void setCubeDefaultPos();
	void setPillarDefaultPos();
	void setOphanimDefaultPos();


	void setPressEnd();



	int getHP() { return bossHP; };

	//�^����_���[�W������(�_���[�W���m�肵�����p�Ŋ֐�����邱��)
	void OnCollision(int damage);

	void reset();

private:

	//RailCamera* camera{nullptr};

	//���[���h�g�����X�t�H�[��
	Obj3d worldTransform=Obj3d("Resources/3dModels/core/core.obj");

	int bossHP = 1000;

	//��̍��W(�����������珈����������������Ȃ��̂ŃN���X�������ق�����������)
	/*bossHand hand[8];*/

	//�N���X������
	std::vector<bossHand*> hand;

	enum phase
	{
		Cube,
		pillar,
		ophanim
	};
	
	int state = Cube;

	float setbossCubeDistance = 6.0f;
	float setbossCubePressDistance = 4.8f;

	float setBossPillarRollDistance = setbossCubeDistance;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;

	////3D���f��
	//Model* model_ = nullptr;

	bool isHandMove = false;

	bool isbossPunch = false;
	bool isBossPress = false;
	bool isBossStoneFall = false;
	bool isBossPillarFall = false;
	bool isBossPillarRoll = false;
	bool isBossBeam = false;
	bool isBossPushUp = false;

	bool isPressStart = false;
	bool isPressFall = false;
	bool isPressReturn = false;
	bool pressFirstStart = false;
	bool pillarRollFirstStart = false;
	bool isPillarRollEnd = false;
	bool pillarPushUpFirstStart = false;
	bool beamFirstStart = false;
	bool beamEndStart = false;

	int punchCount = 0;
	int pressCount = 0;
	int bossStoneFallCount = 0;
	int bossPillarFallCount = 0;
	int bossBeamCount = 0;

	float pressPosY = 20;

	DirectX::XMFLOAT3 returnPos = {};
	DirectX::XMFLOAT3 targetPos = {};
	DirectX::XMFLOAT3 beamStartPos = {};

	//�^�C���J�E���^�[
	float timeCount = 0;
	float fallTimeCount = 0;
	float returnTimeCount = 0;
	float moveEndTimeCount = 0;

	float beamFirstMoveCount = 0;
	float beamEndMoveCount = 0;

	//���쎞��
	float maxTime = 10.0f;
	float maxfallTime = 10.0f;
	float maxReturnTime = 20.0f;
	float maxMoveEndTime = 5.0f;

	float maxFirstBeamMoveTime = 20.0f;
	float maxEndBeamMoveTime = 20.0f;

	float maxBosspillarMoveTime = 30;

	float maxBosspillarRollTime = 25;
	float maxBosspillarRollEndTime = 600;

	//�E�G�C�g
	float waitTime = 0;
	float punchWaitTime = 0;
	float bossActionSelectWaitTime = 60;
	float bossActionSelectWait = bossActionSelectWaitTime;

	//�҂���
	float fallWaitTime = 2;
	float returnWaitTime = 20;

	float bossPunchWaitTime = 20;

	float bossStoneFallWaitTime = 20;

	float bossPillarFallWaitTime = 20;

	float bossBeamWaitTime = 40;

	float bosspillarMoveTime = 0;

	float bosspillarRollTime = 0;
	float bosspillarRollEndTime = 0;
	
	int bosspillarDefaultPosCount = 0;

	DirectX::XMFLOAT3 pillarDefaultPosRotate[8] =
	{
		{ 0,0,1 },
		{ 1,0,1 },
		{ 1,0,0 },
		{ 1,0,-1 },
		{ 0,0,-1 },
		{ -1,0,-1 },
		{ -1,0,0 },
		{ -1,0,1 },
	};

	float bossOphanimMoveTime = 0;
	float maxBossOphanimMoveTime = 10;
	int bossOphanimDefaultPosCount = 0;

	DirectX::XMFLOAT3 ophanimDefaultPosRotate[8] =
	{
		{ 0,1,0 },
		{ -1,1,0 },
		{ -1,0,0 },
		{ -1,-1,0 },
		{ 0,-1,0 },
		{ 1,-1,0 },
		{ 1,0,0 },
		{ 1,1,0 },
	};

};

