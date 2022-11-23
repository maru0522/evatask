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
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	void Initlize(RailCamera* camera, const XMFLOAT3& position, const XMFLOAT3& rota, const XMFLOAT3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw();

	bool IsDead()const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	XMFLOAT3 GetWorldPosition();
	XMFLOAT3 GetScale();

private:

	RailCamera* camera{ nullptr };

	//���[���h�ϊ��f�[�^
	Obj3d bullet=Obj3d{  "Resources/3dModels/bit/bit.obj" };

	//�e�N�X�`���n���h��


	//���x
	XMFLOAT3 Velocity_;

	//����<frm>
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

};


