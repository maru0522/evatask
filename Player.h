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
#include"bosstest.h"
#include"RailCamera.h"
#include "Audio.h"


using namespace DirectX;
using namespace Input;

class Player
{
	
public:
	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(RailCamera* Rcamera, bosstest* boss);
	//void ResourceInitialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Update(RailCamera* camera);

	void GameOverUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(RailCamera* camera);

	void DrawUI(RailCamera* camera);

	XMFLOAT2 kasu(XMFLOAT3 obj,RailCamera* camera);

	bool screenLock(WorldCoordinate pos, RailCamera* camera);

	/// <summary>
	/// UI描画

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack(XMFLOAT3 flont, RailCamera* camera);

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

	bool LockOn(RailCamera* camera);

	void SetWorldPosition(XMFLOAT3 osimodosi);

	XMFLOAT3 Hikaku (XMFLOAT3 boss,  XMFLOAT3 hand1, XMFLOAT3 hand2);
	XMFLOAT3 Hikaku2(XMFLOAT3 hand1, XMFLOAT3 hand2);

	WorldCoordinate GetHitArea();
	void EnemyArrow();

	//衝突を検出したら呼び出されるコールバック関数
	int SetDamege(int attacknum);

	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; };

	bool GetIsdead();

	void reset();

    void FinalizeSound(void);

private:
	RailCamera* Rcamera{nullptr};

	bosstest* boss = nullptr;

	XAudio::SoundData exampleSound = XAudio::Load("Resources/Sounds/SE_gunfire.wav");

	static const int gunbitnum = 4;

	//ワールド変換データ
	Obj3d player=Obj3d{ "Resources/3dModels/player/Player.obj" };
	Obj3d arrow =Obj3d{ "Resources/3dModels/bit/bit.obj" };
	Obj3d gunbit[gunbitnum] =
	{
		Obj3d{ "Resources/3dModels/bit/bit.obj" },
		Obj3d{ "Resources/3dModels/bit/bit.obj" },
		Obj3d{ "Resources/3dModels/bit/bit.obj" },
		Obj3d{ "Resources/3dModels/bit/bit.obj" }
	};
	Sprite Reticle{"Resources/Reticle.png",CMode::PATH};
	Sprite bosstarget{ "Resources/mark.png",CMode::PATH };

	Sprite HelthBar{ "Resources/Helth.png",CMode::PATH };
	Sprite ShieldBar{ "Resources/Shield.png",CMode::PATH };

	Sprite PadRst{ "Resources/PadUiRStickButton.png",CMode::PATH };
	Sprite PadLst{ "Resources/PadUiLStickButton.png",CMode::PATH };
	Sprite PadA	 { "Resources/PadUiAButton.png",CMode::PATH };
	Sprite PadRB { "Resources/PadUiRBButton.png",CMode::PATH };

	WorldCoordinate kyozou;

	XMFLOAT3 nannka[gunbitnum];

	
	int Helth;
	int Shield;
	int MaxShield;

	int Shieldregencool;

	int MutekiTimer;

	
	bool isDead;

	Camera viewProjection_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t ReticleTexture = 0u;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;


	int bulletCT = 0;

	const int MAXBulletCT = 5;

	XMFLOAT3 moveVec;

	float move_speed;

	bool hopper_dash;

	int cooltime;

	float hopper_speed;

	bool cooldown;

	XMFLOAT3 dash_vec;

	const int firelate = 15;

	int latetime;

	size_t width;
	size_t height;

	float timer;

	float bitmovetimer;

	float NormalTimer;

	const float BitNormalAttakTime = 20;

	bool lockmove;

	float hoppertimer;

	const float hoppertime = 10;

	float hozonY, hozonX;

	float arrowrange;
	float arrowvec;

	XMFLOAT3 hozon;
	XMFLOAT3 BitVec;

    XAudio::SoundData SE_playerFoot = XAudio::Load("Resources/Sounds/SE_footstep.wav"); // プレイヤーの足音
    XAudio::SoundData SE_playerAttack = XAudio::Load("Resources/Sounds/SE_gunfire.wav"); // プレイヤー攻撃　音量超注意
};

