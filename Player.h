#pragma once
#include"Window.h"
#include<DirectXMath.h>
#include"Obj3d.h"
#include"Input.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>
#include"Camera.h"
#include"WorldCoordinate.h"
#include"Sprite.h"

using namespace DirectX;
using namespace Input;

class Player
{
	struct Vector3 : public XMFLOAT3 {

		Vector3() = default;
		Vector3(float x, float y, float z)
		{
			this->x = x; this->y = y; this->z = z;
		}
		//�R���X�g���N�^
		Vector3(const XMVECTOR& other) :XMFLOAT3() {
			XMVECTOR temp = other;
			XMStoreFloat3(this, temp);
		}

		inline BOOL operator == (const Vector3& r) const { return x == r.x && y == r.y && z == r.z; }
		inline BOOL operator != (const Vector3& r) const { return x != r.x || y != r.y || z != r.z; }
		inline XMVECTOR operator *(const float r) const { return Vector3(x * r, y * r, z * r); }
		inline XMVECTOR operator /(const float r) const { return Vector3(x / r, y / r, z / r); }

		// �x�N�g���̓���
		float VDot(Vector3 In) { return x * In.x + y * In.y + z * In.z; }
		// �x�N�g���̊O��
		Vector3 VCross(Vector3 In) { return Vector3(y * In.z - z * In.y, z * In.x - x * In.z, x * In.y - y * In.x); }
		// �x�N�g���̃X�P�[�����O
		Vector3 VScale(float Scale) { Vector3 Result = { x * Scale, y * Scale, z * Scale }; return Result; }

		//���
		Vector3& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat3(this, temp);
			return *this;
		}
		//�L���X�g
		operator XMVECTOR() const {
			return XMLoadFloat3(this);
		}
		//����
		float Length() const {
			return (static_cast<Vector3>(XMVector3Length(XMVECTOR(*this)))).x;
		}
		//���K��
		void normalize() {
			*this = XMVector3Normalize(XMVECTOR(*this));
		}
	};

public:
	Player();
	~Player();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Camera* camera/*, bosstest* boss*/);
	//void ResourceInitialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Update(Camera viewProjection);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(Camera& viewProjection);

	void DrawUI();

	XMFLOAT2 kasu(XMFLOAT3 obj);

	bool screenLock(WorldCoordinate pos);

	/// <summary>
	/// UI�`��

	/// <summary>
	/// �U��
	/// </summary>
	void Attack(XMFLOAT3 flont);

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	XMFLOAT3 VectorMat(XMFLOAT3 vector, XMMATRIX mat);

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	XMFLOAT3 VectorMat(XMMATRIX mat, XMFLOAT3 vector);

	XMFLOAT3 PlayerMoveVec();

	//���[���h���W���擾
	XMFLOAT3 GetWorldPosition();

	WorldCoordinate GetMat();

	//XMFLOAT2 poskure() { return bosstarget->GetPosition(); }

	bool LockOn();

	void SetWorldPosition(XMFLOAT3 osimodosi);

	XMFLOAT3 Hikaku (XMFLOAT3 boss,  XMFLOAT3 hand1, XMFLOAT3 hand2);
	XMFLOAT3 Hikaku2(XMFLOAT3 hand1, XMFLOAT3 hand2);

	void EnemyArrow();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };


	//void reset();

private:

	Camera* camera{nullptr};

	//bosstest* boss = nullptr;


	static const int gunbitnum = 4;

	//���[���h�ϊ��f�[�^
	Obj3d player{"Resources/3dModels/player/player.obj", camera};
	Obj3d arrow{ "Resources/3dModels/bit/bit.obj", camera };
	Obj3d gunbit[gunbitnum]
	{ 
		Obj3d{"Resources/3dModels/cube/cube.obj", camera},
		Obj3d{"Resources/3dModels/cube/cube.obj", camera},
		Obj3d{"Resources/3dModels/cube/cube.obj", camera},
		Obj3d{"Resources/3dModels/cube/cube.obj", camera}
	};

	WorldCoordinate kyozou{camera};

	XMFLOAT3 nannka[gunbitnum];


	Camera viewProjection_;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	uint32_t ReticleTexture = 0u;

	Keyboard* KeyInput= nullptr;
	XPad* PadInput = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	std::unique_ptr<Sprite> Reticle;
	std::unique_ptr<Sprite> bosstarget;

	int bulletCT = 0;

	const int MAXBulletCT = 5;

	XMFLOAT3 moveVec;

	float move_speed;

	bool hopper_dash;

	int cooltime;

	float hopper_speed;

	bool cooldown;

	XMFLOAT3 dash_vec;

	bool B_bottan;
	bool old_B_bottan;

	const int firelate = 15;

	int latetime;

	int width;
	int height;

	float timer;

	float bitmovetimer;

	float NormalTimer;

	const float BitNormalAttakTime = 20;

	bool lockmove;

	float hoppertimer;

	const float hoppertime = 20;

	float hozonY, hozonX;

	XMFLOAT3 hozon;
	XMFLOAT3 BitVec;

};

