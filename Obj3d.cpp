#include "Obj3d.h"
#include "Model.h"
#include "InitDirectX.h"

GraphicsPipeline* Obj3d::graphicsPipeline_;

Obj3d::Obj3d(const fsPath& pathAndObjName)
{
    model_.SetMODEL_KEY(pathAndObjName);
    model_.SetMODEL_VALUE(*Model::GetMODEL_VALUE(pathAndObjName)); // �Ȃ񂩂��ꐦ���댯�ȓn�����ȋC������

    // �����Ŏ~�܂��Ă�Ȃ�.mtl���Ő������摜���L����Ă��Ȃ��\���B
    srvGpuHandleCopy_ = Texture::GetTextureInfo(model_.GetModelValueInfo()->material_.texKey).srvGpuHandle_;
}

Obj3d::Obj3d(const fsPath& pathAndObjName, Camera* pCamera)
{
    pCamera_ = pCamera;

    worldCoordinate_.SetCamera(pCamera_);

    model_.SetMODEL_KEY(pathAndObjName);
    model_.SetMODEL_VALUE(*Model::GetMODEL_VALUE(pathAndObjName)); // �Ȃ񂩂��ꐦ���댯�ȓn�����ȋC������

    // �����Ŏ~�܂��Ă�Ȃ�.mtl���Ő������摜���L����Ă��Ȃ��\���B
    srvGpuHandleCopy_ = Texture::GetTextureInfo(model_.GetModelValueInfo()->material_.texKey).srvGpuHandle_;
}

void Obj3d::Update(void)
{
    model_.Update();
    worldCoordinate_.Update();
}

void Obj3d::Initialize(void)
{
    graphicsPipeline_ = GraphicsPipeline::GetGraphicsPipeLine3d();
}

void Obj3d::PreDraw(void)
{
    // �C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

    // �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
    iDX->GetCommandList()->SetPipelineState(graphicsPipeline_->GetPipelineState());
    iDX->GetCommandList()->SetGraphicsRootSignature(graphicsPipeline_->GetRootSignature());

    // �v���~�e�B�u�`��̐ݒ�R�}���h
    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
    // SRV�q�[�v�̐ݒ�R�}���h
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Obj3d::Draw(void)
{
    // �C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region ���_�o�b�t�@
    // ���_�o�b�t�@�̐ݒ�
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &model_.GetModelValueInfo()->vbView_);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@
    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    iDX->GetCommandList()->IASetIndexBuffer(&model_.GetModelValueInfo()->ibView_);
#pragma endregion

#pragma region ConstBuffer
    // �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, worldCoordinate_.GetCBData()->GetBuffer()->GetGPUVirtualAddress());
    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(1, model_.GetModelValueInfo()->constBuffB1_.GetBuffer()->GetGPUVirtualAddress());
#pragma endregion

#pragma region SRV
    //�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvGpuHandleCopy_);
#pragma endregion

    // �`��R�}���h���X�g
    iDX->GetCommandList()->DrawIndexedInstanced((uint32_t)model_.GetModelValueInfo()->indices_.size(), 1, 0, 0, 0);
}