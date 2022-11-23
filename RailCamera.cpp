#include "RailCamera.h"
#include "bossHand.h"


RailCamera::RailCamera()
{

}
RailCamera::~RailCamera()
{

}

void RailCamera::Initialize(DirectX::XMFLOAT3 worldPos, DirectX::XMFLOAT3 Rot)
{
	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();

	worldTransform_.worldCoordinate_.position_ = worldPos;
	worldTransform_.worldCoordinate_.rotation_ = Rot;
	worldTransform_.worldCoordinate_.scale_ = {1.0f,1.0f,1.0f};



}

void RailCamera::Update()
{

	worldTransform_.Update();

	viewProjection_->eye_ = worldTransform_.worldCoordinate_.position_;

	//ワールド前方ベクトル
	forward = { 0.0f, 0.0f, 1.0f };

	//レールカメラの回転を反映
	forward = VectorMat(forward, worldTransform_.worldCoordinate_.matWorld_);

	//視点から前方に適当な距離進んだ位置が注視点
	viewProjection_->target_ = viewProjection_->eye_ + forward;

	//ワールド上方ベクトル
	DirectX::XMFLOAT3 up(0, 1, 0);

	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_->up_ = VectorMat(up, worldTransform_.worldCoordinate_.matWorld_);

	
	viewProjection_->Update();

	/*debugText_->SetPos(50, 110);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);*/
}

DirectX::XMFLOAT3 RailCamera::VectorMat(DirectX::XMFLOAT3 vector, DirectX::XMMATRIX mat)
{
	DirectX::XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.r[0].m128_f32[0] + vector.y * mat.r[1].m128_f32[0] + vector.z * mat.r[2].m128_f32[0] + 0.0f * mat.r[3].m128_f32[0];
	changeVector.y = vector.x * mat.r[0].m128_f32[1] + vector.y * mat.r[1].m128_f32[1] + vector.z * mat.r[2].m128_f32[1] + 0.0f * mat.r[3].m128_f32[1];
	changeVector.z = vector.x * mat.r[0].m128_f32[2] + vector.y * mat.r[1].m128_f32[2] + vector.z * mat.r[2].m128_f32[2] + 0.0f * mat.r[3].m128_f32[2];

	return changeVector;
}

DirectX::XMFLOAT3 RailCamera::VectorMat(DirectX::XMMATRIX mat, DirectX::XMFLOAT3 vector)
{
	DirectX::XMFLOAT3 changeVector = { 0,0,0 };

	changeVector.x = mat.r[0].m128_f32[0] * vector.x + mat.r[0].m128_f32[1] * vector.y + mat.r[0].m128_f32[2] * vector.z + mat.r[0].m128_f32[3] * 0.0f;
	changeVector.y = mat.r[1].m128_f32[0] * vector.x + mat.r[1].m128_f32[1] * vector.y + mat.r[1].m128_f32[2] * vector.z + mat.r[1].m128_f32[3] * 0.0f;
	changeVector.z = mat.r[2].m128_f32[0] * vector.x + mat.r[2].m128_f32[1] * vector.y + mat.r[2].m128_f32[2] * vector.z + mat.r[2].m128_f32[3] * 0.0f;

	return changeVector;
}

Camera* RailCamera::getView()
{
	return viewProjection_;
}

DirectX::XMMATRIX RailCamera::getMatWorld()
{
	return worldTransform_.worldCoordinate_.matWorld_;
}

void RailCamera::setPos(DirectX::XMFLOAT3 pos)
{
	worldTransform_.worldCoordinate_.position_ = pos;
	worldTransform_.Update();
}

void RailCamera::setRotate(DirectX::XMFLOAT3 rotate)
{
	worldTransform_.worldCoordinate_.rotation_ = rotate;
	worldTransform_.Update();
}

void RailCamera::setWorldMat(DirectX::XMMATRIX woeldMat)
{
	worldTransform_.worldCoordinate_.matWorld_ = woeldMat;
}

DirectX::XMFLOAT3 RailCamera::GetWorldPosition()
{

	//ワールド座標を入れる変数
	DirectX::XMFLOAT3 worldpos;

	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = worldTransform_.worldCoordinate_.matWorld_.r[3].m128_f32[0];
	worldpos.y = worldTransform_.worldCoordinate_.matWorld_.r[3].m128_f32[1];
	worldpos.z = worldTransform_.worldCoordinate_.matWorld_.r[3].m128_f32[2];

	return worldpos;

}

DirectX::XMFLOAT3 RailCamera::getForwardVec()
{
	normalize(forward);

	return forward;
}