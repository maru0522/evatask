#include "Cube.h"
#include "InitDirectX.h"
#include <string>
#include "Texture.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

Cube::Cube(const std::string& pathAndFileName_or_Id)
{
    srvGpuHandleCopy_ = Texture::GetTextureInfo(pathAndFileName_or_Id).srvGpuHandle_;

    // ���_�f�[�^�̐ݒ�
#pragma region ���_���W�}��
    // �O
    vertices_.at(0) = { {-5.0f, -5.0f, -5.0f }, {}, { 0.0f, 1.0f } };   // ���� 
    vertices_.at(1) = { {-5.0f,  5.0f, -5.0f }, {}, { 0.0f, 0.0f } };   // ����
    vertices_.at(2) = { { 5.0f, -5.0f, -5.0f }, {}, { 1.0f, 1.0f } };   // �E��
    vertices_.at(3) = { { 5.0f,  5.0f, -5.0f }, {}, { 1.0f, 0.0f } };   // �E��
    // ��
    vertices_.at(4) = { {-5.0f, -5.0f,  5.0f }, {}, { 0.0f, 1.0f } };   // ���� 
    vertices_.at(5) = { {-5.0f,  5.0f,  5.0f }, {}, { 0.0f, 0.0f } };   // ����
    vertices_.at(6) = { { 5.0f, -5.0f,  5.0f }, {}, { 1.0f, 1.0f } };   // �E��
    vertices_.at(7) = { { 5.0f,  5.0f,  5.0f }, {}, { 1.0f, 0.0f } };   // �E��
    // ��
    vertices_.at(8) = { {-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };     // ���� 
    vertices_.at(9) = { {-5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f} };     // ����
    vertices_.at(10) = { {-5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f} };    // �E��
    vertices_.at(11) = { {-5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // �E��
    // �E
    vertices_.at(12) = { { 5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // ���� 
    vertices_.at(13) = { { 5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f} };    // ����
    vertices_.at(14) = { { 5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f} };    // �E��
    vertices_.at(15) = { { 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // �E��
    // ��
    vertices_.at(16) = { {-5.0f,  5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // ���� 
    vertices_.at(17) = { { 5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f} };    // ����
    vertices_.at(18) = { {-5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f} };    // �E��
    vertices_.at(19) = { { 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // �E��
    // ��
    vertices_.at(20) = { {-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // ���� 
    vertices_.at(21) = { { 5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f} };    // ����
    vertices_.at(22) = { {-5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f} };    // �E��
    vertices_.at(23) = { { 5.0f, -5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // �E��
#pragma endregion

#pragma region �C���f�b�N�X�z��ւ̒��_�ԍ��}��
    uint16_t indices[] =
    {
        // �O
            0,1,2,      //�O�p�`1��
            2,1,3,      //�O�p�`2��
        // ��
            4,6,5,      //�O�p�`3��
            6,7,5,      //�O�p�`4��
        // ��
            8,9,10,     //�O�p�`5��
            10,9,11,    //�O�p�`6��
        // �E
            12,14,13,   //�O�p�`7��
            14,15,13,   //�O�p�`8��
        // ��
            16,18,17,   //�O�p�`9��
            18,19,17,   //�O�p�`10��
        // ��
            20,21,22,   //�O�p�`11��
            22,21,23,   //�O�p�`12��
    };

    std::copy(std::begin(indices), std::end(indices), indices_.data()); // �����o�ɃR�s�[
#pragma endregion

#pragma region ���_�z���normal�֑}��
    for (int i = 0; i < indices_.size() / 3; ++i) {
        // �O�p�`1���ƂɌv�Z���Ă���
        // �O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
        uint16_t index0 = indices_[i * 3 + 0];
        uint16_t index1 = indices_[i * 3 + 1];
        uint16_t index2 = indices_[i * 3 + 2];
        // �O�p�`���\�����钸�_���W���x�N�g���ɑ��
        DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&vertices_[index0].pos_);
        DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&vertices_[index1].pos_);
        DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&vertices_[index2].pos_);
        // p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
        DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
        DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);
        // �O�ς͗������琂���ȃx�N�g��
        DirectX::XMVECTOR normal_ = DirectX::XMVector3Cross(v1, v2);
        // ���K���i������1�ɂ���j
        normal_ = DirectX::XMVector3Normalize(normal_);
        // ���߂��@���𒸓_�f�[�^�ɑ��
        DirectX::XMStoreFloat3(&vertices_[index0].normal_, normal_);
        DirectX::XMStoreFloat3(&vertices_[index1].normal_, normal_);
        DirectX::XMStoreFloat3(&vertices_[index2].normal_, normal_);
    }
#pragma endregion

#pragma region ���_�o�b�t�@�̐���
    // ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
    uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

    // ���_�o�b�t�@�̐ݒ�
    D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    InitDirectX* iDX = InitDirectX::GetInstance();

    // ���_�o�b�t�@�̐���
    HRESULT r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region ���_�o�b�t�@�̃}�b�s���O
    // GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
    VertexCube_st* vertMap{ nullptr };
    r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // �S���_�ɑ΂���
    for (int i = 0; i < vertices_.size(); i++)
    {
        vertMap[i] = vertices_[i];//���W���R�s�[
    }

    // �q���������
    vertBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region ���_�o�b�t�@�̃r���[�ݒ�
    // vbview
    // GPU���z�A�h���X
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    // ���_�o�b�t�@�̃T�C�Y
    vbView_.SizeInBytes = sizeVB;
    // ���_1���̃f�[�^�T�C�Y
    vbView_.StrideInBytes = sizeof(vertices_[0]);
#pragma endregion

#pragma region ���_�V�F�[�_�[
    ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"BasicVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }
#pragma endregion

#pragma region �s�N�Z���V�F�[�_
    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"BasicPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }
#pragma endregion

#pragma region ���_���C�A�E�g
    // ���_���C�A�E�g
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { // xyz���W(1�s�ŏ������ق������₷��)
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        { // �@���x�N�g���i1�s�������ق����킩��₷��)
            "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        { // uv���W(1�s�ŏ������ق������₷��)
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
    };
#pragma endregion

#pragma region �p�C�v���C���̐ݒ�i���L�j
    // �V�F�[�_�[�̐ݒ�
    pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

    // �T���v���}�X�N�̐ݒ�
    pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

    // ���X�^���C�U�̐ݒ�
    pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   // �w�ʂ��J�����O
    pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;  // �|���S�����h��Ԃ�
    pipelineDesc_.RasterizerState.DepthClipEnable = true;            // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�
    // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�i�u�����h�X�e�[�g�j�����ꃁ���o�ɂ������������񂶂�ˁH�H
    D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc_.BlendState.RenderTarget[0];
    blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��

    // ���ʐݒ�(�A���t�@�l)
    blendDesc.BlendEnable = true;					// �u�����h��L���ɂ���
    blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ���Z
    blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
    blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// �f�X�g�̒l��0%�g��

    // ���Z����(RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
    blendDesc.DestBlend = D3D12_BLEND_ONE;		// �f�X�g�̒l��100%�g��

    // ���Z����(RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		// �f�X�g�J���\�[�X�������Z
    blendDesc.SrcBlend = D3D12_BLEND_ONE;					// �\�[�X�̒l��100%�g��
    blendDesc.DestBlend = D3D12_BLEND_ONE;					// �f�X�g�̒l��100%�g��

    // �F���](RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;					// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		// 1.0f - �f�X�g�J���[�̒l
    blendDesc.DestBlend = D3D12_BLEND_ZERO;					// �g��Ȃ�

    // ����������
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;					// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				// �\�[�X�̃A���t�@�l
    blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		// 1.0f - �\�[�X�̃A���t�@�l
#pragma endregion

    // ���_���C�A�E�g�̐ݒ�
    pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

    // �}�`�̌`��ݒ�
    pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // ���̑��̐ݒ�
    pipelineDesc_.NumRenderTargets = 1; // �`��Ώۂ�1��
    pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
    pipelineDesc_.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

#pragma region �f�X�N���v�^�����W
    // �f�X�N���v�^�����W�̐ݒ�
    D3D12_DESCRIPTOR_RANGE descriptorRange{};
    descriptorRange.NumDescriptors = 1;
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange.BaseShaderRegister = 0;	// �e�N�X�`�����W�X�^0��
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ���[�g�p�����[�^
    // ���[�g�p�����[�^�[�̐ݒ�
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    // �萔�o�b�t�@0��
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// �萔�o�b�t�@�r���[
    rootParams[0].Descriptor.ShaderRegister = 0;					// �萔�o�b�t�@�ԍ�
    rootParams[0].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����
    // �e�N�X�`�����W�X�^0��
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// ���
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    // �萔�o�b�t�@1��
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
    rootParams[2].Descriptor.ShaderRegister = 1;					// �萔�o�b�t�@�ԍ�
    rootParams[2].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����
#pragma endregion

#pragma region �T���v���[�̐ݒ�
    //�e�N�X�`���T���v���[�̐ݒ�
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�(�^�C�����O)
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ��i�^�C�����O�j
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ��i�^�C�����O�j
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ăV�j�A���
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
    samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂ݎg�p�\
#pragma endregion

#pragma region ���[�g�V�O�l�`��
    //���[�g�V�O�l�`���̐ݒ�
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams;//���[�g�p�����[�^�̐擪�A�h���X
    rootSignatureDesc.NumParameters = _countof(rootParams);//���[�g�p�����[�^��
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    ComPtr<ID3DBlob> rootSigBlob{ nullptr }; // ���[�g�V�O�l�`���̃V���A���C�Y
    r = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(r));
    r = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(r));
#pragma endregion

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc_.pRootSignature = rootSignature_.Get();

#pragma region �[�x�ݒ�
    // �f�v�X�X�e���V���X�e�[�g�̐ݒ�
    pipelineDesc_.DepthStencilState.DepthEnable = true;  // �[�x�e�X�g���s��
    pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;     // �������݋���
    pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;          // ��������΍��i
    pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                 // �[�x�l�t�H�[�}�b�g
#pragma endregion
#pragma endregion

#pragma region �p�C�v���C���X�e�[�g�̐���
    // �p�C�v���C���ݒ�̕ۑ�
    r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
    assert(SUCCEEDED(r));
#pragma endregion

    SetColor();

    //SetCBTransform();

#pragma region �C���f�b�N�X�o�b�t�@�̐���
    // �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
    uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

    // ���\�[�X�ݒ�
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// �C���f�b�N�X��񂪓��镪�̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // �C���f�b�N�X�o�b�t�@�̐���
    r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp,	// �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuff_));
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�̃}�b�s���O
    // �C���f�b�N�X�o�b�t�@���}�b�s���O
    uint16_t* indexMap = nullptr;
    r = indexBuff_->Map(0, nullptr, (void**)&indexMap);
    // �S�C���f�b�N�X�ɑ΂���
    for (int i = 0; i < indices_.size(); i++) {
        indexMap[i] = indices_[i];	// �C���f�b�N�X���R�s�[
    }
    // �}�b�s���O����
    indexBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�̃r���[�ݒ�
    // �C���f�b�N�X�o�b�t�@�r���[�̍쐬
    ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
    ibView_.Format = DXGI_FORMAT_R16_UINT;
    ibView_.SizeInBytes = sizeIB;
#pragma endregion
}

Cube::Cube(const std::string& pathAndFileName_or_Id, Camera* pCamera)
{
    srvGpuHandleCopy_ = Texture::GetTextureInfo(pathAndFileName_or_Id).srvGpuHandle_;

    pCamera_ = pCamera;

    // ���_�f�[�^�̐ݒ�
#pragma region ���_���W�}��
    // �O
    vertices_.at(0) = { {-5.0f, -5.0f, -5.0f }, {}, { 0.0f, 1.0f } };   // ���� 
    vertices_.at(1) = { {-5.0f,  5.0f, -5.0f }, {}, { 0.0f, 0.0f } };   // ����
    vertices_.at(2) = { { 5.0f, -5.0f, -5.0f }, {}, { 1.0f, 1.0f } };   // �E��
    vertices_.at(3) = { { 5.0f,  5.0f, -5.0f }, {}, { 1.0f, 0.0f } };   // �E��
    // ��
    vertices_.at(4) = { {-5.0f, -5.0f,  5.0f }, {}, { 0.0f, 1.0f } };   // ���� 
    vertices_.at(5) = { {-5.0f,  5.0f,  5.0f }, {}, { 0.0f, 0.0f } };   // ����
    vertices_.at(6) = { { 5.0f, -5.0f,  5.0f }, {}, { 1.0f, 1.0f } };   // �E��
    vertices_.at(7) = { { 5.0f,  5.0f,  5.0f }, {}, { 1.0f, 0.0f } };   // �E��
    // ��
    vertices_.at(8) = { {-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };     // ���� 
    vertices_.at(9) = { {-5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f} };     // ����
    vertices_.at(10) = { {-5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f} };    // �E��
    vertices_.at(11) = { {-5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // �E��
    // �E
    vertices_.at(12) = { { 5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // ���� 
    vertices_.at(13) = { { 5.0f, -5.0f,  5.0f }, {}, {0.0f, 0.0f} };    // ����
    vertices_.at(14) = { { 5.0f,  5.0f, -5.0f }, {}, {1.0f, 1.0f} };    // �E��
    vertices_.at(15) = { { 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // �E��
    // ��
    vertices_.at(16) = { {-5.0f,  5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // ���� 
    vertices_.at(17) = { { 5.0f,  5.0f, -5.0f }, {}, {0.0f, 0.0f} };    // ����
    vertices_.at(18) = { {-5.0f,  5.0f,  5.0f }, {}, {1.0f, 1.0f} };    // �E��
    vertices_.at(19) = { { 5.0f,  5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // �E��
    // ��
    vertices_.at(20) = { {-5.0f, -5.0f, -5.0f }, {}, {0.0f, 1.0f} };    // ���� 
    vertices_.at(21) = { { 5.0f, -5.0f, -5.0f }, {}, {0.0f, 0.0f} };    // ����
    vertices_.at(22) = { {-5.0f, -5.0f,  5.0f }, {}, {1.0f, 1.0f} };    // �E��
    vertices_.at(23) = { { 5.0f, -5.0f,  5.0f }, {}, {1.0f, 0.0f} };    // �E��
#pragma endregion

#pragma region �C���f�b�N�X�z��ւ̒��_�ԍ��}��
    uint16_t indices[] =
    {
        // �O
            0,1,2,      //�O�p�`1��
            2,1,3,      //�O�p�`2��
        // ��
            4,6,5,      //�O�p�`3��
            6,7,5,      //�O�p�`4��
        // ��
            8,9,10,     //�O�p�`5��
            10,9,11,    //�O�p�`6��
        // �E
            12,14,13,   //�O�p�`7��
            14,15,13,   //�O�p�`8��
        // ��
            16,18,17,   //�O�p�`9��
            18,19,17,   //�O�p�`10��
        // ��
            20,21,22,   //�O�p�`11��
            22,21,23,   //�O�p�`12��
    };

    std::copy(std::begin(indices), std::end(indices), indices_.data()); // �����o�ɃR�s�[
#pragma endregion

#pragma region ���_�z���normal�֑}��
    for (int i = 0; i < indices_.size() / 3; ++i) {
        // �O�p�`1���ƂɌv�Z���Ă���
        // �O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
        uint16_t index0 = indices_[i * 3 + 0];
        uint16_t index1 = indices_[i * 3 + 1];
        uint16_t index2 = indices_[i * 3 + 2];
        // �O�p�`���\�����钸�_���W���x�N�g���ɑ��
        DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&vertices_[index0].pos_);
        DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&vertices_[index1].pos_);
        DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&vertices_[index2].pos_);
        // p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
        DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
        DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);
        // �O�ς͗������琂���ȃx�N�g��
        DirectX::XMVECTOR normal_ = DirectX::XMVector3Cross(v1, v2);
        // ���K���i������1�ɂ���j
        normal_ = DirectX::XMVector3Normalize(normal_);
        // ���߂��@���𒸓_�f�[�^�ɑ��
        DirectX::XMStoreFloat3(&vertices_[index0].normal_, normal_);
        DirectX::XMStoreFloat3(&vertices_[index1].normal_, normal_);
        DirectX::XMStoreFloat3(&vertices_[index2].normal_, normal_);
    }
#pragma endregion

#pragma region ���_�o�b�t�@�̐���
    // ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
    uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

    // ���_�o�b�t�@�̐ݒ�
    D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC resDesc{};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    InitDirectX* iDX = InitDirectX::GetInstance();

    // ���_�o�b�t�@�̐���
    HRESULT r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp, // �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc, // ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region ���_�o�b�t�@�̃}�b�s���O
    // GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
    VertexCube_st* vertMap{ nullptr };
    r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // �S���_�ɑ΂���
    for (int i = 0; i < vertices_.size(); i++)
    {
        vertMap[i] = vertices_[i];//���W���R�s�[
    }

    // �q���������
    vertBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region ���_�o�b�t�@�̃r���[�ݒ�
    // vbview
    // GPU���z�A�h���X
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    // ���_�o�b�t�@�̃T�C�Y
    vbView_.SizeInBytes = sizeVB;
    // ���_1���̃f�[�^�T�C�Y
    vbView_.StrideInBytes = sizeof(vertices_[0]);
#pragma endregion

#pragma region ���_�V�F�[�_�[
    ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"BasicVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }
#pragma endregion

#pragma region �s�N�Z���V�F�[�_
    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"BasicPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }
#pragma endregion

#pragma region ���_���C�A�E�g
    // ���_���C�A�E�g
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { // xyz���W(1�s�ŏ������ق������₷��)
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        { // �@���x�N�g���i1�s�������ق����킩��₷��)
            "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        { // uv���W(1�s�ŏ������ق������₷��)
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
    };
#pragma endregion

#pragma region �p�C�v���C���̐ݒ�i���L�j
    // �V�F�[�_�[�̐ݒ�
    pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

    // �T���v���}�X�N�̐ݒ�
    pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

    // ���X�^���C�U�̐ݒ�
    pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   // �w�ʂ��J�����O
    pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;  // �|���S�����h��Ԃ�
    pipelineDesc_.RasterizerState.DepthClipEnable = true;            // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�
    // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�i�u�����h�X�e�[�g�j�����ꃁ���o�ɂ������������񂶂�ˁH�H
    D3D12_RENDER_TARGET_BLEND_DESC& blendDesc = pipelineDesc_.BlendState.RenderTarget[0];
    blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��

    // ���ʐݒ�(�A���t�@�l)
    blendDesc.BlendEnable = true;					// �u�����h��L���ɂ���
    blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ���Z
    blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
    blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// �f�X�g�̒l��0%�g��

    // ���Z����(RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
    blendDesc.DestBlend = D3D12_BLEND_ONE;		// �f�X�g�̒l��100%�g��

    // ���Z����(RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		// �f�X�g�J���\�[�X�������Z
    blendDesc.SrcBlend = D3D12_BLEND_ONE;					// �\�[�X�̒l��100%�g��
    blendDesc.DestBlend = D3D12_BLEND_ONE;					// �f�X�g�̒l��100%�g��

    // �F���](RGB�l)
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;					// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		// 1.0f - �f�X�g�J���[�̒l
    blendDesc.DestBlend = D3D12_BLEND_ZERO;					// �g��Ȃ�

    // ����������
    blendDesc.BlendOp = D3D12_BLEND_OP_ADD;					// ���Z
    blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				// �\�[�X�̃A���t�@�l
    blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		// 1.0f - �\�[�X�̃A���t�@�l
#pragma endregion

    // ���_���C�A�E�g�̐ݒ�
    pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

    // �}�`�̌`��ݒ�
    pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // ���̑��̐ݒ�
    pipelineDesc_.NumRenderTargets = 1; // �`��Ώۂ�1��
    pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
    pipelineDesc_.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

#pragma region �f�X�N���v�^�����W
    // �f�X�N���v�^�����W�̐ݒ�
    D3D12_DESCRIPTOR_RANGE descriptorRange{};
    descriptorRange.NumDescriptors = 1;
    descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRange.BaseShaderRegister = 0;	// �e�N�X�`�����W�X�^0��
    descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ���[�g�p�����[�^
    // ���[�g�p�����[�^�[�̐ݒ�
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    // �萔�o�b�t�@0��
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// �萔�o�b�t�@�r���[
    rootParams[0].Descriptor.ShaderRegister = 0;					// �萔�o�b�t�@�ԍ�
    rootParams[0].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����
    // �e�N�X�`�����W�X�^0��
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// ���
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    // �萔�o�b�t�@1��
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
    rootParams[2].Descriptor.ShaderRegister = 1;					// �萔�o�b�t�@�ԍ�
    rootParams[2].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����
#pragma endregion

#pragma region �T���v���[�̐ݒ�
    //�e�N�X�`���T���v���[�̐ݒ�
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�(�^�C�����O)
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ��i�^�C�����O�j
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ��i�^�C�����O�j
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ăV�j�A���
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
    samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂ݎg�p�\
#pragma endregion

#pragma region ���[�g�V�O�l�`��
    //���[�g�V�O�l�`���̐ݒ�
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams;//���[�g�p�����[�^�̐擪�A�h���X
    rootSignatureDesc.NumParameters = _countof(rootParams);//���[�g�p�����[�^��
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    ComPtr<ID3DBlob> rootSigBlob{ nullptr }; // ���[�g�V�O�l�`���̃V���A���C�Y
    r = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);
    assert(SUCCEEDED(r));
    r = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature_));
    assert(SUCCEEDED(r));
#pragma endregion

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc_.pRootSignature = rootSignature_.Get();

#pragma region �[�x�ݒ�
    // �f�v�X�X�e���V���X�e�[�g�̐ݒ�
    pipelineDesc_.DepthStencilState.DepthEnable = true;  // �[�x�e�X�g���s��
    pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;     // �������݋���
    pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;          // ��������΍��i
    pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;                                 // �[�x�l�t�H�[�}�b�g
#pragma endregion
#pragma endregion

#pragma region �p�C�v���C���X�e�[�g�̐���
    // �p�C�v���C���ݒ�̕ۑ�
    r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
    assert(SUCCEEDED(r));
#pragma endregion

    SetColor();

    //SetCBTransform();

#pragma region �C���f�b�N�X�o�b�t�@�̐���
    // �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
    uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

    // ���\�[�X�ݒ�
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeIB;	// �C���f�b�N�X��񂪓��镪�̃T�C�Y
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    // �C���f�b�N�X�o�b�t�@�̐���
    r = iDX->GetDevice()->CreateCommittedResource(
        &heapProp,	// �q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resDesc,	// ���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&indexBuff_));
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�̃}�b�s���O
    // �C���f�b�N�X�o�b�t�@���}�b�s���O
    uint16_t* indexMap = nullptr;
    r = indexBuff_->Map(0, nullptr, (void**)&indexMap);
    // �S�C���f�b�N�X�ɑ΂���
    for (int i = 0; i < indices_.size(); i++) {
        indexMap[i] = indices_[i];	// �C���f�b�N�X���R�s�[
    }
    // �}�b�s���O����
    indexBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�̃r���[�ݒ�
    // �C���f�b�N�X�o�b�t�@�r���[�̍쐬
    ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
    ibView_.Format = DXGI_FORMAT_R16_UINT;
    ibView_.SizeInBytes = sizeIB;
#pragma endregion
}

void Cube::Update(void)
{
    // �e�s��̐錾�Ə�����
    DirectX::XMMATRIX matScale{ DirectX::XMMatrixIdentity() }; // �X�P�[���s��
    DirectX::XMMATRIX matRot{ DirectX::XMMatrixIdentity() }; // ��]�s��
    DirectX::XMMATRIX matTrans{ DirectX::XMMatrixIdentity() }; // ���s�ړ��s��

    // �X�P�[���s��̌v�Z
    matScale = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);

    // ��]�s��̌v�Z
    matRot = DirectX::XMMatrixIdentity();
    matRot *= DirectX::XMMatrixRotationZ(rotation_.z);
    matRot *= DirectX::XMMatrixRotationX(rotation_.x);
    matRot *= DirectX::XMMatrixRotationY(rotation_.y);

    // ���s�ړ��s��̌v�Z
    matTrans = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);

    // ���[���h�s��̍���
    matWorld_ = DirectX::XMMatrixIdentity();    // �ό`�����Z�b�g
    matWorld_ *= matScale;                      // ���[���h�s��ɃX�P�[�����O�𔽉f
    matWorld_ *= matRot;                        // ���[���h�s��ɉ�]�𔽉f
    matWorld_ *= matTrans;                      // ���[���h�s��ɕ��s�ړ��𔽉f

    // �e�I�u�W�F�N�g�������
    if (parent_) {
        //�e�I�u�W�F�N�g�̃��[���h�s���������
        matWorld_ *= parent_->matWorld_;
    }

    // �萔�o�b�t�@�փf�[�^�]��
    TransferMatrix();
}

void Cube::Draw(void)
{
    SetCommandsBeforeDraw();

    // �C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region ���_�o�b�t�@
    // ���_�o�b�t�@�̐ݒ�
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@
    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    iDX->GetCommandList()->IASetIndexBuffer(&ibView_);
#pragma endregion

#pragma region ConstBuffer
    // �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
#pragma endregion

#pragma region SRV
    //�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandleCopy_);
#pragma endregion

    // �`��R�}���h���X�g
    iDX->GetCommandList()->DrawIndexedInstanced(static_cast<uint32_t>(indices_.size()), 1, 0, 0, 0);
}

void Cube::SetColor(DirectX::XMFLOAT4 rgba)
{
    // �l��0.0f��菬������0.0f�ɂ���
    rgba.x = (std::max)(rgba.x, 0.0f);
    rgba.y = (std::max)(rgba.y, 0.0f);
    rgba.z = (std::max)(rgba.z, 0.0f);
    rgba.w = (std::max)(rgba.w, 0.0f);

    // �l��1.0f���傫����1.0f�ɂ���
    rgba.x = (std::min)(1.0f, rgba.x);
    rgba.y = (std::min)(1.0f, rgba.y);
    rgba.z = (std::min)(1.0f, rgba.z);
    rgba.w = (std::min)(1.0f, rgba.w);

    // �l���������ނƎ����I�ɓ]�������
    cbMaterial_.GetConstBuffMap()->color_ = rgba;
}

void Cube::TransferMatrix(void)
{
    if (pCamera_) {
        matView_ = pCamera_->GetView();
        matProjection_ = pCamera_->GetProjection();
    }

    cbTransform_.GetConstBuffMap()->mat_ = matWorld_ * matView_ * matProjection_;
}

void Cube::SetCommandsBeforeDraw(void)
{
    // �C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

    // �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
    iDX->GetCommandList()->SetPipelineState(pipelineState_.Get());
    iDX->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());

    // �v���~�e�B�u�`��̐ݒ�R�}���h
    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
    // SRV�q�[�v�̐ݒ�R�}���h
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}
