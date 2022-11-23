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
	void Initialize(RailCamera* camera, bosstest* boss);
	//void ResourceInitialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw();

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


	void reset();

private:
	RailCamera* Rcamera = nullptr;

	bosstest* boss = nullptr;

	static const int gunbitnum = 4;

	//ワールド変換データ
	Obj3d player{"Resources/3dModels/player/player.obj", Rcamera->getView()};
	Obj3d arrow{ "Resources/3dModels/bit/bit.obj", Rcamera->getView() };
	Obj3d gunbit[gunbitnum]
	{ 
		Obj3d{"Resources/3dModels/cube/cube.obj", Rcamera->getView()},
		Obj3d{"Resources/3dModels/cube/cube.obj", Rcamera->getView()},
		Obj3d{"Resources/3dModels/cube/cube.obj", Rcamera->getView()},
		Obj3d{"Resources/3dModels/cube/cube.obj", Rcamera->getView()}
	};											  
	Sprite Reticle{"Resources/Reticle.png",CMode::PATH};
	Sprite bosstarget{ "Resources/mark.png",CMode::PATH };

	WorldCoordinate kyozou{ Rcamera->getView() };

	XMFLOAT3 nannka[gunbitnum];


	Camera viewProjection_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t ReticleTexture = 0u;

	Keyboard* KeyInput= nullptr;
	XPad* PadInput = nullptr;

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

