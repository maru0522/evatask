#pragma once
#include"Obj3d.h"
#include"RailCamera.h"

using namespace DirectX;
class PlayerBullet
{
public:
	PlayerBullet();
	~PlayerBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	void Initlize(RailCamera* camera, const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw();

	bool IsDead()const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	XMFLOAT3 GetWorldPosition();
	XMFLOAT3 GetScale();

private:

	RailCamera* camera{ nullptr };

	//ワールド変換データ
	Obj3d bullet=Obj3d{  "Resources/3dModels/bit/bit.obj" };

	//テクスチャハンドル


	//速度
	XMFLOAT3 Velocity_;

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

};


