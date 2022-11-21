#include "Sprite.h"
#include "InitDirectX.h"
#include <string>
#include "Texture.h"
#include "Window.h"
#include "Util.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

Sprite::Sprite(const std::string& relativePath, const std::string& fileName)
{
#pragma region gpuHandle�̎󂯎��
    srvGpuHandleCopy_ = Texture::GetTextureInfo(relativePath, fileName).srvGpuHandle_;
#pragma endregion

#pragma region ���_��񏈗�
    // ���_�f�[�^�̐ݒ�
    vertices_.at(0) = { {   0.0f, 100.0f, 0.0f }, {0.0f, 1.0f} };    // ����
    vertices_.at(1) = { {   0.0f,   0.0f, 0.0f }, {0.0f, 0.0f} };    // ����
    vertices_.at(2) = { { 100.0f, 100.0f, 0.0f }, {1.0f, 1.0f} };    // �E��
    vertices_.at(3) = { { 100.0f,   0.0f, 0.0f }, {1.0f, 0.0f} };    // �E��

    // ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
    uint32_t sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

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

    // GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
    VertexSprite_st* vertMap{ nullptr };
    r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // �S���_�ɑ΂���
    for (int i = 0; i < vertices_.size(); i++)
    {
        vertMap[i] = vertices_[i]; // ���W���R�s�[
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

#pragma region �V�F�[�_�[�̐ݒ�
    ComPtr<ID3DBlob> vsBlob{ nullptr }; // ���_�V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> psBlob{ nullptr }; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> errorBlob{ nullptr }; // �G���[�I�u�W�F�N�g
    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"Resources/Shaders/SpriteVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r))
    {
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

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"Resources/Shaders/SpritePS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r))
    {
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

#pragma region �p�C�v���C���̐ݒ�i���L�j
    // �V�F�[�_�[�̐ݒ�
    pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

    // �T���v���}�X�N�̐ݒ�
    pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

    // ���X�^���C�U�̐ݒ�
    pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
    pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
    pipelineDesc_.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�
    // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�i�u�����h�X�e�[�g�j�����ꃁ���o�ɂ������������񂶂�ˁH�H
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��

    // ���ʐݒ�i�A���t�@�l�j
    blenddesc.BlendEnable = true;                           // �u�����h��L���ɂ���
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;            // ���Z
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;              // �\�[�X�̒l��100%�g��
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;            // �f�X�g�̒l��0%�g��

    // ���Z����
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;             // �\�[�X�̒l��100%�g��
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // �f�X�g�̒l��100%�g��

    // ���Z����(RGB�l)
    blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;        // �f�X�g����\�[�X�����Z
    blenddesc.SrcBlend = D3D12_BLEND_ONE;                   // �\�[�X�̒l��100%�g��
    blenddesc.DestBlend = D3D12_BLEND_ONE;                  // �f�X�g�̒l��100%�g��

    // �F���](RGB�l)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;        // 1.0f - �f�X�g�J���[�̒l
    blenddesc.DestBlend = D3D12_BLEND_ZERO;                 // �g��Ȃ�

    // ����������
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;             // �\�[�X�̃A���t�@�l
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // 1.0f - �\�[�X�̃A���t�@�l
#pragma endregion

    // ���_���C�A�E�g
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {   // xyz���W(1�s�ŏ������ق������₷��)
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        { // uv���W(1�s�ŏ������ق������₷��)
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
    };

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
    // �f�X�N���v�^�����W�̕ϐ��錾
    D3D12_DESCRIPTOR_RANGE descriporRange{};
    // �f�X�N���v�^�����W�̐ݒ�
    descriporRange.NumDescriptors = 1;
    descriporRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriporRange.BaseShaderRegister = 0; // �e�N�X�`�����W�X�^0��
    descriporRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ���[�g�p�����[�^
    // ���[�g�p�����[�^�[�̐ݒ�
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // �萔�o�b�t�@�r���[
    rootParams[0].Descriptor.ShaderRegister = 0;                    // �萔�o�b�t�@�ԍ�
    rootParams[0].Descriptor.RegisterSpace = 0;                     // �f�t�H���g�l
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // �S�ẴV�F�[�_���猩����
    // �e�N�X�`�����W�X�^0��
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // ���
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriporRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    // �萔�o�b�t�@1��
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // ���
    rootParams[2].Descriptor.ShaderRegister = 1;                    // �萔�o�b�t�@�ԍ�
    rootParams[2].Descriptor.RegisterSpace = 0;                     // �f�t�H���g�l
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // �S�ẴV�F�[�_���猩����
#pragma endregion

#pragma region �T���v���[�̐ݒ�
    //�e�N�X�`���T���v���[�̕ϐ��錾
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    //�e�N�X�`���T���v���[�̐ݒ�
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
    // ���[�g�V�O�l�`���̕ϐ��錾
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    // ���[�g�V�O�l�`���̐ݒ�
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
    rootSignatureDesc.NumParameters = static_cast<uint32_t>(std::size(rootParams));
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    ComPtr<ID3DBlob> rootSigBlob{ nullptr }; // ���[�g�V�O�l�`���̃V���A���C�Y

    r = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    r = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    rootSigBlob->Release();
#pragma endregion

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc_.pRootSignature = rootSignature_.Get();
#pragma endregion �p�C�v���C���̐ݒ�i��L�j

    // �p�C�v���C���ݒ��ۑ�
    r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

#pragma region cbTransform
    SetCBTransform();
#pragma endregion

    SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

Sprite::Sprite(const std::string& pathAndFileName_or_Id, CMode mode)
{
#pragma region gpuHandle�̎󂯎��
    if (!static_cast<int>(CMode::PATH)) {
        // �R���X�g���N�^���[�h��PATH�̏ꍇ
        srvGpuHandleCopy_ = Texture::GetTextureInfo(pathAndFileName_or_Id).srvGpuHandle_;
    }
    else {
        // �R���X�g���N�^���[�h��ID�̏ꍇ
        srvGpuHandleCopy_ = Texture::GetTextureInfoById(pathAndFileName_or_Id).srvGpuHandle_;
    }
#pragma endregion

#pragma region ���_��񏈗�
    // ���_�f�[�^�̐ݒ�
    vertices_.at(0) = { {   0.0f, 100.0f, 0.0f }, {0.0f, 1.0f} };    // ����
    vertices_.at(1) = { {   0.0f,   0.0f, 0.0f }, {0.0f, 0.0f} };    // ����
    vertices_.at(2) = { { 100.0f, 100.0f, 0.0f }, {1.0f, 1.0f} };    // �E��
    vertices_.at(3) = { { 100.0f,   0.0f, 0.0f }, {1.0f, 0.0f} };    // �E��

    // ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
    uint32_t sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size());

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

    // �C���X�^���X�擾
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

    // GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
    VertexSprite_st* vertMap{ nullptr };
    r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
    // �S���_�ɑ΂���
    for (int i = 0; i < vertices_.size(); i++)
    {
        vertMap[i] = vertices_[i]; // ���W���R�s�[
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

#pragma region �V�F�[�_�[�̐ݒ�
    ComPtr<ID3DBlob> vsBlob{ nullptr }; // ���_�V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> psBlob{ nullptr }; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
    ComPtr<ID3DBlob> errorBlob{ nullptr }; // �G���[�I�u�W�F�N�g
    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"Resources/Shaders/SpriteVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r))
    {
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

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    r = D3DCompileFromFile(L"Resources/Shaders/SpritePS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(r))
    {
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

#pragma region �p�C�v���C���̐ݒ�i���L�j
    // �V�F�[�_�[�̐ݒ�
    pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

    // �T���v���}�X�N�̐ݒ�
    pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

    // ���X�^���C�U�̐ݒ�
    pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
    pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
    pipelineDesc_.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

#pragma region �u�����h�ݒ�
    // �����_�[�^�[�Q�b�g�̃u�����h�ݒ�i�u�����h�X�e�[�g�j�����ꃁ���o�ɂ������������񂶂�ˁH�H
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��

    // ���ʐݒ�i�A���t�@�l�j
    blenddesc.BlendEnable = true;                           // �u�����h��L���ɂ���
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;            // ���Z
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;              // �\�[�X�̒l��100%�g��
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;            // �f�X�g�̒l��0%�g��

    // ���Z����
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;             // �\�[�X�̒l��100%�g��
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // �f�X�g�̒l��100%�g��

    // ���Z����(RGB�l)
    blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;        // �f�X�g����\�[�X�����Z
    blenddesc.SrcBlend = D3D12_BLEND_ONE;                   // �\�[�X�̒l��100%�g��
    blenddesc.DestBlend = D3D12_BLEND_ONE;                  // �f�X�g�̒l��100%�g��

    // �F���](RGB�l)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;        // 1.0f - �f�X�g�J���[�̒l
    blenddesc.DestBlend = D3D12_BLEND_ZERO;                 // �g��Ȃ�

    // ����������
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;                 // ���Z
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;             // �\�[�X�̃A���t�@�l
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;        // 1.0f - �\�[�X�̃A���t�@�l
#pragma endregion

#pragma region ���_���C�A�E�g�̐ݒ�
    // ���_���C�A�E�g
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {   // xyz���W(1�s�ŏ������ق������₷��)
            "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
        { // uv���W(1�s�ŏ������ق������₷��)
            "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
            D3D12_APPEND_ALIGNED_ELEMENT,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
        },
    };
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
    // �f�X�N���v�^�����W�̕ϐ��錾
    D3D12_DESCRIPTOR_RANGE descriporRange{};
    // �f�X�N���v�^�����W�̐ݒ�
    descriporRange.NumDescriptors = 1;
    descriporRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriporRange.BaseShaderRegister = 0; // �e�N�X�`�����W�X�^0��
    descriporRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
#pragma endregion

#pragma region ���[�g�p�����[�^
    // ���[�g�p�����[�^�[�̐ݒ�
    D3D12_ROOT_PARAMETER rootParams[3] = {};
    rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // �萔�o�b�t�@�r���[
    rootParams[0].Descriptor.ShaderRegister = 0;                    // �萔�o�b�t�@�ԍ�
    rootParams[0].Descriptor.RegisterSpace = 0;                     // �f�t�H���g�l
    rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // �S�ẴV�F�[�_���猩����
    // �e�N�X�`�����W�X�^0��
    rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// ���
    rootParams[1].DescriptorTable.pDescriptorRanges = &descriporRange;
    rootParams[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    // �萔�o�b�t�@1��
    rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // ���
    rootParams[2].Descriptor.ShaderRegister = 1;                    // �萔�o�b�t�@�ԍ�
    rootParams[2].Descriptor.RegisterSpace = 0;                     // �f�t�H���g�l
    rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;   // �S�ẴV�F�[�_���猩����
#pragma endregion

#pragma region �T���v���[�̐ݒ�
    //�e�N�X�`���T���v���[�̕ϐ��錾
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};
    //�e�N�X�`���T���v���[�̐ݒ�
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
    // ���[�g�V�O�l�`���̕ϐ��錾
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    // ���[�g�V�O�l�`���̐ݒ�
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
    rootSignatureDesc.NumParameters = static_cast<uint32_t>(std::size(rootParams));
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
    rootSignatureDesc.NumStaticSamplers = 1;

    ComPtr<ID3DBlob> rootSigBlob{ nullptr }; // ���[�g�V�O�l�`���̃V���A���C�Y

    r = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
        &rootSigBlob, &errorBlob);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    r = iDX->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
        IID_PPV_ARGS(&rootSignature_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc_.pRootSignature = rootSignature_.Get();
#pragma endregion �p�C�v���C���̐ݒ�i��L�j

    // �p�C�v���C���ݒ��ۑ�
    r = iDX->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

#pragma region cbTransform
    SetCBTransform();
#pragma endregion

    SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

void Sprite::Update(void)
{
    TransferVertex();

    // ���[���h�s��̏�����
    matWorld_ = DirectX::XMMatrixIdentity();

    // ��]�s��̐錾�Ə�����
    DirectX::XMMATRIX matRot{ DirectX::XMMatrixIdentity() };
    matRot *= DirectX::XMMatrixRotationZ(rotation_); // z�x��]
    matWorld_ *= matRot; // ���[���h�s��ɉ�]�𔽉f

    // ���s�ړ��s��̐錾�Ə�����
    DirectX::XMMATRIX matTrans{ DirectX::XMMatrixIdentity() };
    matTrans = DirectX::XMMatrixTranslation(position_.x, position_.y, 0.0f); // ���s�ړ�
    matWorld_ *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

    // �e��ݒ肵�Ă���ꍇ�͐e�̃��[���h�s�������
    if (parent_) {
        matWorld_ *= parent_->matWorld_;
    } // ��̐ݒ�̓��[���h�s��̂��̂ł͂Ȃ����[�J���s��ׂ̈̐ݒ�ƂȂ�

    // �萔�o�b�t�@�֓]��
    TransferMatrix();
}

void Sprite::Draw(void)
{
    if (isInvisible_) {
        return;
    }

    SetCommandsBeforeDraw();

    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region ���_�o�b�t�@
    // ���_�o�b�t�@�r���[�̐ݒ�R�}���h
    iDX->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
#pragma endregion

#pragma region ConstBuffer
    // �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(0, cbMaterial_.GetBuffer()->GetGPUVirtualAddress());
    iDX->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTransform_.GetBuffer()->GetGPUVirtualAddress());
#pragma endregion

#pragma region SRV
    // SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
    iDX->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandleCopy_);
#pragma endregion

    // �`��R�}���h
    iDX->GetCommandList()->DrawInstanced(static_cast<uint32_t>(vertices_.size()), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
}

void Sprite::SetColor(DirectX::XMFLOAT4 rgba)
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

void Sprite::SetColor(float_t r, float_t g, float_t b, float_t a)
{
    // �l��0.0f��菬������0.0f�ɂ���
    r = (std::max)(r, 0.0f);
    g = (std::max)(g, 0.0f);
    b = (std::max)(b, 0.0f);
    a = (std::max)(a, 0.0f);

    // �l��1.0f���傫����1.0f�ɂ���
    r = (std::min)(1.0f, r);
    g = (std::min)(1.0f, g);
    b = (std::min)(1.0f, b);
    a = (std::min)(1.0f, a);

    // �l���������ނƎ����I�ɓ]�������
    cbMaterial_.GetConstBuffMap()->color_ = { r,g,b,a };
}

void Sprite::SetColor255(DirectX::XMFLOAT4 rgba)
{
    // �l��0.0f��菬������0.0f�ɂ���
    rgba.x = (std::max)(rgba.x, 0.0f);
    rgba.y = (std::max)(rgba.y, 0.0f);
    rgba.z = (std::max)(rgba.z, 0.0f);
    rgba.w = (std::max)(rgba.w, 0.0f);

    // �l��255.0f���傫����255.0f�ɂ���
    rgba.x = (std::min)(255.0f, rgba.x);
    rgba.y = (std::min)(255.0f, rgba.y);
    rgba.z = (std::min)(255.0f, rgba.z);
    rgba.w = (std::min)(255.0f, rgba.w);

    // ���K��
    rgba = {
        static_cast<float>(rgba.x / 255.0f),
        static_cast<float>(rgba.y / 255.0f),
        static_cast<float>(rgba.z / 255.0f),
        static_cast<float>(rgba.w / 255.0f)
    };

    // �l���������ނƎ����I�ɓ]�������
    cbMaterial_.GetConstBuffMap()->color_ = rgba;
}

void Sprite::SetColor255(float_t r, float_t g, float_t b, float_t a)
{
    // �l��0.0f��菬������0.0f�ɂ���
    r = (std::max)(r, 0.0f);
    g = (std::max)(g, 0.0f);
    b = (std::max)(b, 0.0f);
    a = (std::max)(a, 0.0f);

    // �l��255.0f���傫����255.0f�ɂ���
    r = (std::min)(255.0f, r);
    g = (std::min)(255.0f, g);
    b = (std::min)(255.0f, b);
    a = (std::min)(255.0f, a);

    // ���K��
    DirectX::XMFLOAT4 rgba{
        static_cast<float>(r / 255.0f),
        static_cast<float>(g / 255.0f),
        static_cast<float>(b / 255.0f),
        static_cast<float>(a / 255.0f)
    };

    // �l���������ނƎ����I�ɓ]�������
    cbMaterial_.GetConstBuffMap()->color_ = rgba;
}

void Sprite::SetParent(Sprite* parent)
{
    parent_ = parent;
}

void Sprite::TransferVertex(void)
{
#pragma region �A���J�[�|�C���g�̕ύX
    float_t left{ (0.0f - anchorPoint_.x) * size_.x };
    float_t right{ (1.0f - anchorPoint_.x) * size_.x };
    float_t top{ (0.0f - anchorPoint_.y) * size_.y };
    float_t bottom{ (1.0f - anchorPoint_.y) * size_.y };
#pragma endregion

#pragma region �t���b�v�̕ύX
    // �O���t�B�b�N�X�p�C�v���C���̔w�ʃJ�����O���I�t�ɂ��Ă��Ȃ��ƕ`�悳��Ȃ��Ȃ�

    // ���E�t���b�v
    if (isFlipX_) {
        left = -left; // ��
        right = -right; // �E
    }

    // �㉺�t���b�v
    if (isFlipY_) {
        top = -top; // ��
        bottom = -bottom; // ��
    }
#pragma endregion

#pragma region ���_���W�̕ύX
    vertices_.at(static_cast<int>(VertNum::LeftBottom)).pos_ = { left, bottom, 0.0f };      // ����
    vertices_.at(static_cast<int>(VertNum::LeftTop)).pos_ = { left, top, 0.0f };            // ����
    vertices_.at(static_cast<int>(VertNum::RightBottom)).pos_ = { right, bottom, 0.0f };    // �E��
    vertices_.at(static_cast<int>(VertNum::RightTop)).pos_ = { right, top, 0.0f };          // �E��
#pragma endregion

#pragma region ���_�o�b�t�@�r���[
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress(); // GPU���Q�Ƃ���A�h���X��n��
    vbView_.SizeInBytes = static_cast<UINT>(sizeof(vertices_[0]) * vertices_.size()); // ���_�o�b�t�@�̃T�C�Y = ���_�f�[�^�S�̂̃T�C�Y
    vbView_.StrideInBytes = sizeof(vertices_[0]); // ���_1���̃f�[�^�T�C�Y
#pragma endregion

#pragma region ���W�̍X�V
    // GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
    VertexSprite_st* vertMap{ nullptr };
    // �}�b�s���O
    HRESULT r = vertBuff_->Map(0, nullptr, (void**)&vertMap);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG

    // ���W�̃R�s�[
    std::copy(vertices_.begin(), vertices_.end(), vertMap);

    // �}�b�s���O����
    vertBuff_->Unmap(0, nullptr);
#pragma endregion
}

void Sprite::TransferMatrix(void)
{
    cbTransform_.GetConstBuffMap()->mat_ = matWorld_ * matProjection_;
}

void Sprite::SetCBTransform(void)
{
#pragma region cbTransform
    cbTransform_.GetConstBuffMap()->mat_ = DirectX::XMMatrixIdentity();
    cbTransform_.GetConstBuffMap()->mat_.r[0].m128_f32[0] = 2.0f / Window::width_;
    cbTransform_.GetConstBuffMap()->mat_.r[1].m128_f32[1] = -2.0f / Window::height_;
    cbTransform_.GetConstBuffMap()->mat_.r[3].m128_f32[0] = -1.0f;
    cbTransform_.GetConstBuffMap()->mat_.r[3].m128_f32[1] = 1.0f;
#pragma endregion
}

void Sprite::SetCommandsBeforeDraw(void)
{
    InitDirectX* iDX = InitDirectX::GetInstance();

# pragma region ����
    // �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
    iDX->GetCommandList()->SetPipelineState(pipelineState_.Get());
    iDX->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
    // �v���~�e�B�u�`��̐ݒ�R�}���h
    iDX->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g

    ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
    // SRV�q�[�v�̐ݒ�R�}���h
    iDX->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
#pragma endregion
}
