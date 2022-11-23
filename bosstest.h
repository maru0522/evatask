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
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(RailCamera* camera, DirectX::XMFLOAT3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(DirectX::XMFLOAT3 player);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw();

	void cubeActionDecision();

	//ワールド座標を取得
	DirectX::XMFLOAT3 GetWorldPosition();

	//ボス本体の座標を変える
	void setPos(DirectX::XMFLOAT3 pos);

	void setRotate(DirectX::XMFLOAT3 rotate);

	//handのパンチをするためのフラグを変更
	void setisAttackFlagL(bool flag, DirectX::XMFLOAT3 player);
	void setisbossPunch(bool flag);

	//プレス攻撃をするためのフラグを変更
	void setisBossPress(bool flag);

	//石落としをするためのフラグを変更
	void setisBossStoneFall(bool flag);
	void setisBossPillarFall(bool flag);
	

	void setisBossBeam(bool flag);

	void setisBossPillarRoll(bool flag);

	void setisBossPushUp(bool flag);

	//その場でパンチの攻撃を跳ね返したことにする
	void playerAttackReturnL();

	void bossPunch(DirectX::XMFLOAT3 player);

	//プレス攻撃
	void bossPress(DirectX::XMFLOAT3 player);

	//石落とし
	void bossStoneFall(DirectX::XMFLOAT3 player);
	void bossPillarFall(DirectX::XMFLOAT3 player);

	void bossBeam();

	void bossPillarRoll();

	void bossPushUp(DirectX::XMFLOAT3 player);

	//pos???????????? 全部持ってくる
	WorldCoordinate getPos() { return worldTransform.worldCoordinate_; };

	//handを持ってくる
	std::vector<bossHand*> getHand() { return hand; };

	//プレス攻撃をするときの目標座標移動用関数
	void setPressPos();

	//本体と一緒にhandが動くための関数
	void setPressHandPos();
	void setPillarHandPos();
	void setOphanimHandPos();

	void setCubeDefaultPos();
	void setPillarDefaultPos();
	void setOphanimDefaultPos();


	void setPressEnd();



	int getHP() { return bossHP; };

	//与えるダメージを入れる(ダメージが確定したら専用で関数を作ること)
	void OnCollision(int damage);

	void reset();

private:

	//RailCamera* camera{nullptr};

	//ワールドトランスフォーム
	Obj3d worldTransform=Obj3d("Resources/3dModels/core/core.obj");

	int bossHP = 1000;

	//手の座標(もしかしたら処理が同じかもしれないのでクラス化したほうがいいかも)
	/*bossHand hand[8];*/

	//クラス化した
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

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	////3Dモデル
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

	//タイムカウンター
	float timeCount = 0;
	float fallTimeCount = 0;
	float returnTimeCount = 0;
	float moveEndTimeCount = 0;

	float beamFirstMoveCount = 0;
	float beamEndMoveCount = 0;

	//動作時間
	float maxTime = 10.0f;
	float maxfallTime = 10.0f;
	float maxReturnTime = 20.0f;
	float maxMoveEndTime = 5.0f;

	float maxFirstBeamMoveTime = 20.0f;
	float maxEndBeamMoveTime = 20.0f;

	float maxBosspillarMoveTime = 30;

	float maxBosspillarRollTime = 25;
	float maxBosspillarRollEndTime = 600;

	//ウエイト
	float waitTime = 0;
	float punchWaitTime = 0;
	float bossActionSelectWaitTime = 60;
	float bossActionSelectWait = bossActionSelectWaitTime;

	//待つ時間
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

