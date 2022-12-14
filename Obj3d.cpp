#include "Obj3d.h"
#include "Model.h"
#include "InitDirectX.h"

GraphicsPipeline* Obj3d::graphicsPipeline_;

Obj3d::Obj3d(const fsPath& pathAndObjName)
{
    model_.SetMODEL_KEY(pathAndObjName);
    model_.SetMODEL_VALUE(*Model::GetMODEL_VALUE(pathAndObjName)); // なんかこれ凄く危険な渡し方な気がする

    // ここで止まってるなら.mtl内で正しい画像が記されていない可能性。
    srvGpuHandleCopy_ = Texture::GetTextureInfo(model_.GetModelValueInfo()->material_.texKey).srvGpuHandle_;
}

Obj3d::Obj3d(const fsPath& pathAndObjName, Camera* pCamera)
{
    pCamera_ = pCamera;

    worldCoordinate_.SetCamera(pCamera_);

    model_.SetMODEL_KEY(pathAndObjName);
    model_.SetMODEL_VALUE(*Model::GetMODEL_VALUE(pathAndObjName)); // なんかこれ凄く危険な渡し方な気がする

    // ここで止まってるなら.mtl内で正しい画像が記されていない可能性。
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
    // インスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

    // パイプラインステートとルートシグネチャの設定コマンド
    iDX->GetCommandList()->SetPipelineState(graphicsPipeline_->GetPipelineState());
    iDX->GetCommandList()->SetGraphicsRootSignature(graphicsPipeline_->GetRootSignature());

    // プリミティブ形状の設定コマンド
    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
    // SRVヒープの設定コマンド
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void Obj3d::Draw(void)
{
    // インスタンス取得
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region 頂点バッファ
    // 頂点バッファの設定
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &model_.GetModelValueInfo()->vbView_);
#pragma endregion

#pragma region インデックスバッファ
    // インデックスバッファの設定
    iDX->GetCommandList()->IASetIndexBuffer(&model_.GetModelValueInfo()->ibView_);
#pragma endregion

#pragma region ConstBuffer
    // 定数バッファビュー（CBV）の設定コマンド
    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, worldCoordinate_.GetCBData()->GetBuffer()->GetGPUVirtualAddress());
    //iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(1, model_.GetModelValueInfo()->constBuffB1_.GetBuffer()->GetGPUVirtualAddress());
#pragma endregion

#pragma region SRV
    //指定のヒープにあるSRVをルートパラメータ1番に設定
    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(2, srvGpuHandleCopy_);
#pragma endregion

    // 描画コマンドリスト
    iDX->GetCommandList()->DrawIndexedInstanced((uint32_t)model_.GetModelValueInfo()->indices_.size(), 1, 0, 0, 0);
}

void Obj3d::SetCamera(Camera* pCamera)
{
    pCamera_ = pCamera;
    worldCoordinate_.SetCamera(pCamera_);
}

DirectX::XMFLOAT3 Obj3d::getWorldPos()
{
    DirectX::XMFLOAT3 ans;

    ans.x = worldCoordinate_.matWorld_.r[3].m128_f32[0];
    ans.y = worldCoordinate_.matWorld_.r[3].m128_f32[1];
    ans.z = worldCoordinate_.matWorld_.r[3].m128_f32[2];

    return ans;

}