#pragma once
#include "Obj3d.h"
#include "Camera.h"


class RailCamera
{

public:

	RailCamera();
	~RailCamera();
	
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="worldTransform">���[���h���W</param>
	/// <param name="Rot">��]�p(���W�A��)</param>
	void Initialize(DirectX::XMFLOAT3 worldPos, DirectX::XMFLOAT3 Rot);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	DirectX::XMFLOAT3 VectorMat(DirectX::XMFLOAT3 vector, DirectX::XMMATRIX mat);

	/// <summary>
	/// �s��ƃx�N�g���̌v�Z(�������s��v�Z�̐�)
	/// </summary>
	DirectX::XMFLOAT3 VectorMat(DirectX::XMMATRIX mat, DirectX::XMFLOAT3 vector);

	Camera* getView();

	DirectX::XMMATRIX getMatWorld();

	DirectX::XMFLOAT3 getForwardVec();

	//���[���h���W���擾
	DirectX::XMFLOAT3 GetWorldPosition();

	void setPos(DirectX::XMFLOAT3 pos);

	void setRotate(DirectX::XMFLOAT3 rotate);

	void setWorldMat(DirectX::XMMATRIX woeldMat);

private:

	Camera viewProjection_;

	//���[���h�ϊ��f�[�^(�K���ǂݍ���)
	WorldCoordinate worldTransform_ ;

	//�r���[�v���W�F�N�V����

	DirectX::XMFLOAT3 forward = { 0.0f, 0.0f, 1.0f };

};

