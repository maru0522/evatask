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
		//コンストラクタ
		Vector3(const XMVECTOR& other) :XMFLOAT3() {
			XMVECTOR temp = other;
			XMStoreFloat3(this, temp);
		}

		inline BOOL operator == (const Vector3& r) const { return x == r.x && y == r.y && z == r.z; }
		inline BOOL operator != (const Vector3& r) const { return x != r.x || y != r.y || z != r.z; }
		inline XMVECTOR operator *(const float r) const { return Vector3(x * r, y * r, z * r); }
		inline XMVECTOR operator /(const float r) const { return Vector3(x / r, y / r, z / r); }

		// ベクトルの内積
		float VDot(Vector3 In) { return x * In.x + y * In.y + z * In.z; }
		// ベクトルの外積
		Vector3 VCross(Vector3 In) { return Vector3(y * In.z - z * In.y, z * In.x - x * In.z, x * In.y - y * In.x); }
		// ベクトルのスケーリング
		Vector3 VScale(float Scale) { Vector3 Result = { x * Scale, y * Scale, z * Scale }; return Result; }

		//代入
		Vector3& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat3(this, temp);
			return *this;
		}
		//キャスト
		operator XMVECTOR() const {
			return XMLoadFloat3(this);
		}
		//長さ
		float Length() const {
			return (static_cast<Vector3>(XMVector3Length(XMVECTOR(*this)))).x;
		}
		//正規化
		void normalize() {
			*this = XMVector3Normalize(XMVECTOR(*this));
		}
	};

public:
	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Camera* camera/*, bosstest* boss*/);
	//void ResourceInitialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Update(Camera viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(Camera& viewProjection);

	void DrawUI();

	XMFLOAT2 kasu(XMFLOAT3 obj);

	bool screenLock(WorldCoordinate pos);

	/// <summary>
	/// UI描画

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack(XMFLOAT3 flont);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMFLOAT3 vector, XMMATRIX mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	XMFLOAT3 VectorMat(XMMATRIX mat, XMFLOAT3 vector);

	XMFLOAT3 PlayerMoveVec();

	//ワールド座標を取得
	XMFLOAT3 GetWorldPosition();

	WorldCoordinate GetMat();

	//XMFLOAT2 poskure() { return bosstarget->GetPosition(); }

	bool LockOn();

	void SetWorldPosition(XMFLOAT3 osimodosi);

	XMFLOAT3 Hikaku (XMFLOAT3 boss,  XMFLOAT3 hand1, XMFLOAT3 hand2);
	XMFLOAT3 Hikaku2(XMFLOAT3 hand1, XMFLOAT3 hand2);

	void EnemyArrow();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };


	//void reset();

private:

	Camera* camera{nullptr};

	//bosstest* boss = nullptr;


	static const int gunbitnum = 4;

	//ワールド変換データ
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

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t ReticleTexture = 0u;

	Keyboard* KeyInput= nullptr;
	XPad* PadInput = nullptr;

	//弾
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

