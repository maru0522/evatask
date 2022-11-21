#pragma once
#include <vector>
#include <DirectXMath.h>
#include <string>
#include "Texture.h"
#include <d3d12.h>

#pragma comment(lib,"d3d12.lib")

struct CBDataB0 {
    DirectX::XMMATRIX mat_;     // 3D�ϊ��s��
    //DirectX::XMFLOAT4 color_;   // �F�iRGBA�j
};

struct CBDataB1 {
    DirectX::XMFLOAT3 ambient;  // ambient
    float_t pad1;               // padding
    DirectX::XMFLOAT3 diffuse;  // diffuse
    float_t pad2;               // padding
    DirectX::XMFLOAT3 specular; // specular
    float_t alpha;              // alpha
};

struct VertexModel3d_st {
    DirectX::XMFLOAT3 pos_;     // xyz���W
    DirectX::XMFLOAT3 normal_;  // �@���x�N�g��
    DirectX::XMFLOAT2 uv_;      // uv���W
};

struct MaterialModel3d_st {
    std::string name;            // �}�e���A���̖��O
    DirectX::XMFLOAT3 ambient{};   // ambient�i���j�e���x
    DirectX::XMFLOAT3 diffuse{};   // diffuse�i�L�āj�e���x
    DirectX::XMFLOAT3 specular{};  // specular�i���ʔ��ˁj�e���x
    float_t alpha{};               // �A���t�@
    Texture::TEXTURE_KEY texKey; // �e�N�X�`���t�@�C����

    MaterialModel3d_st() {
        ambient = { 0.3f,0.3f,0.3f };
        diffuse = { 0.0f,0.0f,0.0f };
        specular = { 0.0f,0.0f,0.0f };
        alpha = 1.0f;
    }
};