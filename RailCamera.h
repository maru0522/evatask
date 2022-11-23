#pragma once
#include "Obj3d.h"
#include "Camera.h"


class RailCamera
{

public:

	RailCamera();
	~RailCamera();
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="worldTransform">ワールド座標</param>
	/// <param name="Rot">回転角(ラジアン)</param>
	void Initialize(DirectX::XMFLOAT3 worldPos, DirectX::XMFLOAT3 Rot);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	DirectX::XMFLOAT3 VectorMat(DirectX::XMFLOAT3 vector, DirectX::XMMATRIX mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	DirectX::XMFLOAT3 VectorMat(DirectX::XMMATRIX mat, DirectX::XMFLOAT3 vector);

	Camera* getView();

	DirectX::XMMATRIX getMatWorld();

	DirectX::XMFLOAT3 getForwardVec();

	//ワールド座標を取得
	DirectX::XMFLOAT3 GetWorldPosition();

	void setPos(DirectX::XMFLOAT3 pos);

	void setRotate(DirectX::XMFLOAT3 rotate);

	void setWorldMat(DirectX::XMMATRIX woeldMat);

private:

	Camera viewProjection_;

	//ワールド変換データ(適当読み込み)
	WorldCoordinate worldTransform_ ;

	//ビュープロジェクション

	DirectX::XMFLOAT3 forward = { 0.0f, 0.0f, 1.0f };

};

