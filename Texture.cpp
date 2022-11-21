#include "Texture.h"
#include <cassert>
#include "Util.h"
#include <DirectXTex.h>
#include "InitDirectX.h"

// �ÓI�ϐ��̎��̐���
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Texture::srvHeap_ = nullptr;
std::map<Texture::TEXTURE_KEY, Texture::TEXTURE_VALUE> Texture::textures_{};
std::map<Texture::TEX_KEY_ID, Texture::TEXTURE_KEY> Texture::mapKeys_{};
uint32_t Texture::indexNextDescHeap_{ 0 };

void Texture::Initialize(void)
{
    InitDirectX* iDX_ = InitDirectX::GetInstance();

    // �f�X�N���v�^�q�[�v�̐ݒ�
    D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_���猩����悤��
    srvDescHeapDesc.NumDescriptors = maxSRVDesc_;

    // �ݒ������SRV�p�f�X�N���v�^�q�[�v���쐬
    HRESULT r = iDX_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&srvHeap_));
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
}

void Texture::Load(const std::string& relativePath, const std::string& fileName)
{
#pragma region ���낢��m�F
    // srvDesc�̋󂫊m�F
    assert(indexNextDescHeap_ < maxSRVDesc_);
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // �ꎞobj

    tmp.CheckPath(relativePath, fileName);

    // ���ɓǂݍ��񂾃e�N�X�`���Ƃ̏d���m�F�B
    if (textures_.count(tmp.name_)) {
        // �d�����������ꍇ�C�e���[�^��Ԃ��B
        decltype(textures_)::iterator it{ textures_.find(tmp.name_) };

        // �C�e���[�^����n���h�����擾����
        handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
    }
    else {
        // �d�����Ȃ������ꍇ�͎��̃e�N�X�`���̂��߂Ƀn���h���̎w�W��1�i�߂�B
        indexNextDescHeap_++;
    }
#pragma endregion

#pragma region �e�N�X�`���̃��[�h
    //�摜�C���[�W�f�[�^�z��
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};

    // WIC�e�N�X�`���̃��[�h�Ɏg�� path�𕶎���ϊ�
    std::wstring wPath{ tmp.name_.begin(),tmp.name_.end() };
    const wchar_t* szFile{ wPath.c_str() };

    // WIC�e�N�X�`���̃��[�h
    HRESULT r = LoadFromWICFile(
        szFile,   //�uResources�v�t�H���_�́utexture.png�v
        DirectX::WIC_FLAGS_NONE,
        &metadata, scratchImg);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region �~�b�v�}�b�v
    DirectX::ScratchImage mipChain{};

    // �~�b�v�}�b�v����
    r = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(r)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }
#pragma endregion

    // �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
    metadata.format = DirectX::MakeSRGB(metadata.format);

#pragma region �q�[�v�ݒ�ƃf�X�N���v�^�ݒ�
    // �q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = static_cast<UINT>(metadata.height);
    textureResourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureResourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureResourceDesc.SampleDesc.Count = 1;
#pragma endregion

    // InitDirectX�̃C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region �e�N�X�`���o�b�t�@
    // �e�N�X�`���o�b�t�@�̐���
    r = iDX->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.buff_)); // MAP_VALUE��buff_�֏�������
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region srvCpuHandle�̕ۑ��ƃo�b�t�@�ւ̃f�[�^�]��
    // �v�m�F: static_cast���O���ƌx����
    // �f�X�N���v�^�̃T�C�Y���擾����
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUE��srvCPUHandle_�֏�������
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    tmp.info_.srvCpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // �S�~�b�v�}�b�v�ɂ���
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
        const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);

        // �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
        r = tmp.info_.buff_-> // MAP_VALUE��buff_
            WriteToSubresource(
                static_cast<UINT>(i),
                nullptr,		// �S�̈�փR�s�[
                img->pixels,	// ���f�[�^�A�h���X
                static_cast<UINT>(img->rowPitch),	// 1���C���T�C�Y
                static_cast<UINT>(img->slicePitch)	// �S�T�C�Y
            );
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }
#pragma endregion

#pragma region SRV�̐ݒ�Ɛ���
    // �V�F�[�_���\�[�X�r���[�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
#pragma endregion

#pragma region srvGpuHandle�̕ۑ���map�ɕR�Â��ĕۑ�
    // MAP_VALUE��srvGPUHandle_�֏�������
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    // �n���h����i�߂�
    tmp.info_.srvGpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // map�ւ̑}���ior ����j������̏ꍇ�A����KEY�ɑ΂���VALUE���㏑������邽�ߒ���
    textures_.insert_or_assign(tmp.name_, tmp.info_); // ������ł����Ă��S������VALUE������Ƃ͎v����B
#pragma endregion
}

void Texture::Load(const std::string& pathAndFileName)
{
#pragma region ���낢��m�F
    // srvDesc�̋󂫊m�F
    assert(indexNextDescHeap_ < maxSRVDesc_);
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // �ꎞobj

    // ���O��1�Ȃ̂Ŕ���͂������̂܂ܐݒ�
    tmp.SetMapKey(pathAndFileName);

    // ���ɓǂݍ��񂾃e�N�X�`���Ƃ̏d���m�F�B
    if (textures_.count(tmp.name_)) {
        // �d�����������ꍇ�C�e���[�^��Ԃ��B
        decltype(textures_)::iterator it{ textures_.find(tmp.name_) };

        // �C�e���[�^����n���h�����擾����
        handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
    }
    else {
        // �d�����Ȃ������ꍇ�͎��̃e�N�X�`���̂��߂Ƀn���h���̎w�W��1�i�߂�B
        indexNextDescHeap_++;
    }
#pragma endregion

#pragma region �e�N�X�`���̃��[�h
    //�摜�C���[�W�f�[�^�z��
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};

    // WIC�e�N�X�`���̃��[�h�Ɏg�� path�𕶎���ϊ�
    std::wstring wPath{ tmp.name_.begin(),tmp.name_.end() };
    const wchar_t* szFile{ wPath.c_str() };

    // WIC�e�N�X�`���̃��[�h
    HRESULT r = LoadFromWICFile(
        szFile,   //�uResources�v�t�H���_�́utexture.png�v
        DirectX::WIC_FLAGS_NONE,
        &metadata, scratchImg);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region �~�b�v�}�b�v
    DirectX::ScratchImage mipChain{};

    // �~�b�v�}�b�v����
    r = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(r)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }
#pragma endregion

    // �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
    metadata.format = DirectX::MakeSRGB(metadata.format);

#pragma region �q�[�v�ݒ�ƃf�X�N���v�^�ݒ�
    // �q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = static_cast<UINT>(metadata.height);
    textureResourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureResourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureResourceDesc.SampleDesc.Count = 1;
#pragma endregion

    // InitDirectX�̃C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region �e�N�X�`���o�b�t�@
    // �e�N�X�`���o�b�t�@�̐���
    r = iDX->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.buff_)); // MAP_VALUE��buff_�֏�������
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region srvCpuHandle�̕ۑ��ƃo�b�t�@�ւ̃f�[�^�]��
    // �v�m�F: static_cast���O���ƌx����
    // �f�X�N���v�^�̃T�C�Y���擾����
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUE��srvCPUHandle_�֏�������
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    tmp.info_.srvCpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // �S�~�b�v�}�b�v�ɂ���
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
        const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);

        // �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
        r = tmp.info_.buff_-> // MAP_VALUE��buff_
            WriteToSubresource(
                static_cast<UINT>(i),
                nullptr,		// �S�̈�փR�s�[
                img->pixels,	// ���f�[�^�A�h���X
                static_cast<UINT>(img->rowPitch),	// 1���C���T�C�Y
                static_cast<UINT>(img->slicePitch)	// �S�T�C�Y
            );
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }
#pragma endregion

#pragma region SRV�̐ݒ�Ɛ���
    // �V�F�[�_���\�[�X�r���[�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
#pragma endregion

#pragma region srvGpuHandle�̕ۑ���map�ɕR�Â��ĕۑ�
    // MAP_VALUE��srvGPUHandle_�֏�������
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    // �n���h����i�߂�
    tmp.info_.srvGpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // map�ւ̑}���ior ����j������̏ꍇ�A����KEY�ɑ΂���VALUE���㏑������邽�ߒ���
    textures_.insert_or_assign(tmp.name_, tmp.info_); // ������ł����Ă��S������VALUE������Ƃ͎v����B
#pragma endregion
}

void Texture::Load(const std::string& relativePath, const std::string& fileName, const std::string& id)
{
#pragma region ���낢��m�F
    // srvDesc�̋󂫊m�F
    assert(indexNextDescHeap_ < maxSRVDesc_);
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // �ꎞobj

    tmp.CheckPath(relativePath, fileName); // path�̊m�F

    // ���ɓǂݍ��񂾃e�N�X�`���Ƃ̏d���m�F�B
    if (textures_.count(tmp.name_)) {
        // �d�����������ꍇ�C�e���[�^��Ԃ��B
        decltype(textures_)::iterator it{ textures_.find(tmp.name_) };

        // �C�e���[�^����n���h�����擾����
        handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
    }
    else {
        // �d�����Ȃ������ꍇ�͎��̃e�N�X�`���̂��߂Ƀn���h���̎w�W��1�i�߂�B
        indexNextDescHeap_++;
    }

    // MAP_KEY�Ƃ̕R�t��
    mapKeys_.insert_or_assign(id, tmp.name_);
#pragma endregion

#pragma region �e�N�X�`���̃��[�h
    //�摜�C���[�W�f�[�^�z��
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};

    // WIC�e�N�X�`���̃��[�h�Ɏg�� path�𕶎���ϊ�
    std::wstring wPath{ tmp.name_.begin(),tmp.name_.end() };
    const wchar_t* szFile{ wPath.c_str() };

    // WIC�e�N�X�`���̃��[�h
    HRESULT r = LoadFromWICFile(
        szFile,   //�uResources�v�t�H���_�́utexture.png�v
        DirectX::WIC_FLAGS_NONE,
        &metadata, scratchImg);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region �~�b�v�}�b�v
    DirectX::ScratchImage mipChain{};

    // �~�b�v�}�b�v����
    r = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(r)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }
#pragma endregion

    // �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
    metadata.format = DirectX::MakeSRGB(metadata.format);

#pragma region �q�[�v�ݒ�ƃf�X�N���v�^�ݒ�
    // �q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = static_cast<UINT>(metadata.height);
    textureResourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureResourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureResourceDesc.SampleDesc.Count = 1;
#pragma endregion


    // InitDirectX�̃C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region �e�N�X�`���o�b�t�@
    // �e�N�X�`���o�b�t�@�̐���
    r = iDX->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.buff_)); // MAP_VALUE��buff_�֏�������
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region srvCpuHandle�̕ۑ��ƃo�b�t�@�ւ̃f�[�^�]��
    // �v�m�F: static_cast���O���ƌx����
    // �f�X�N���v�^�̃T�C�Y���擾����
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUE��srvCPUHandle_�֏�������
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    tmp.info_.srvCpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // �S�~�b�v�}�b�v�ɂ���
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
        const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);

        // �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
        r = tmp.info_.buff_-> // MAP_VALUE��buff_
            WriteToSubresource(
                static_cast<UINT>(i),
                nullptr,		// �S�̈�փR�s�[
                img->pixels,	// ���f�[�^�A�h���X
                static_cast<UINT>(img->rowPitch),	// 1���C���T�C�Y
                static_cast<UINT>(img->slicePitch)	// �S�T�C�Y
            );
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }
#pragma endregion

#pragma region SRV�̐ݒ�Ɛ���
    // �V�F�[�_���\�[�X�r���[�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
#pragma endregion

#pragma region srvGpuHandle�̕ۑ���map�ɕR�Â��ĕۑ�
    // MAP_VALUE��srvGPUHandle_�֏�������
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    // �n���h����i�߂�
    tmp.info_.srvGpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // map�ւ̑}���ior ����j������̏ꍇ�A����KEY�ɑ΂���VALUE���㏑������邽�ߒ���
    textures_.insert_or_assign(tmp.name_, tmp.info_); // ������ł����Ă��S������VALUE������Ƃ͎v����B
#pragma endregion
}

void Texture::LoadWithId(const std::string& pathAndFileName, const std::string& id)
{
#pragma region ���낢��m�F
    // srvDesc�̋󂫊m�F
    assert(indexNextDescHeap_ < maxSRVDesc_);
    uint32_t handle{ indexNextDescHeap_ };

    Texture tmp{}; // �ꎞobj

    // ���O��1�Ȃ̂Ŕ���͂������̂܂ܐݒ�
    tmp.SetMapKey(pathAndFileName);

    // ���ɓǂݍ��񂾃e�N�X�`���Ƃ̏d���m�F�B
    if (textures_.count(tmp.name_)) {
        // �d�����������ꍇ�C�e���[�^��Ԃ��B
        decltype(textures_)::iterator it{ textures_.find(tmp.name_) };

        // �C�e���[�^����n���h�����擾����
        handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
    }
    else {
        // �d�����Ȃ������ꍇ�͎��̃e�N�X�`���̂��߂Ƀn���h���̎w�W��1�i�߂�B
        indexNextDescHeap_++;
    }

    // MAP_KEY�Ƃ̕R�t��
    mapKeys_.insert_or_assign(id, tmp.name_);
#pragma endregion

#pragma region �e�N�X�`���̃��[�h
    //�摜�C���[�W�f�[�^�z��
    DirectX::TexMetadata metadata{};
    DirectX::ScratchImage scratchImg{};

    // WIC�e�N�X�`���̃��[�h�Ɏg�� path�𕶎���ϊ�
    std::wstring wPath{ tmp.name_.begin(),tmp.name_.end() };
    const wchar_t* szFile{ wPath.c_str() };

    // WIC�e�N�X�`���̃��[�h
    HRESULT r = LoadFromWICFile(
        szFile,   //�uResources�v�t�H���_�́utexture.png�v
        DirectX::WIC_FLAGS_NONE,
        &metadata, scratchImg);
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region �~�b�v�}�b�v
    DirectX::ScratchImage mipChain{};

    // �~�b�v�}�b�v����
    r = GenerateMipMaps(scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain);
    if (SUCCEEDED(r)) {
        scratchImg = std::move(mipChain);
        metadata = scratchImg.GetMetadata();
    }
#pragma endregion

    // �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
    metadata.format = DirectX::MakeSRGB(metadata.format);

#pragma region �q�[�v�ݒ�ƃf�X�N���v�^�ݒ�
    // �q�[�v�ݒ�
    D3D12_HEAP_PROPERTIES texHeapProp{};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

    // ���\�[�X�ݒ�
    D3D12_RESOURCE_DESC textureResourceDesc{};
    textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureResourceDesc.Format = metadata.format;
    textureResourceDesc.Width = metadata.width;
    textureResourceDesc.Height = static_cast<UINT>(metadata.height);
    textureResourceDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
    textureResourceDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    textureResourceDesc.SampleDesc.Count = 1;
#pragma endregion


    // InitDirectX�̃C���X�^���X�擾
    InitDirectX* iDX = InitDirectX::GetInstance();

#pragma region �e�N�X�`���o�b�t�@
    // �e�N�X�`���o�b�t�@�̐���
    r = iDX->GetDevice()->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &textureResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tmp.info_.buff_)); // MAP_VALUE��buff_�֏�������
#ifdef _DEBUG
    assert(SUCCEEDED(r));
#endif // _DEBUG
#pragma endregion

#pragma region srvCpuHandle�̕ۑ��ƃo�b�t�@�ւ̃f�[�^�]��
    // �v�m�F: static_cast���O���ƌx����
    // �f�X�N���v�^�̃T�C�Y���擾����
    uint32_t incrementSize = iDX->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    // MAP_VALUE��srvCPUHandle_�֏�������
    tmp.info_.srvCpuHandle_ = srvHeap_.Get()->GetCPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    tmp.info_.srvCpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // �S�~�b�v�}�b�v�ɂ���
    for (size_t i = 0; i < metadata.mipLevels; i++) {
        // �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
        const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);

        // �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
        r = tmp.info_.buff_-> // MAP_VALUE��buff_
            WriteToSubresource(
                static_cast<UINT>(i),
                nullptr,		// �S�̈�փR�s�[
                img->pixels,	// ���f�[�^�A�h���X
                static_cast<UINT>(img->rowPitch),	// 1���C���T�C�Y
                static_cast<UINT>(img->slicePitch)	// �S�T�C�Y
            );
#ifdef _DEBUG
        assert(SUCCEEDED(r));
#endif // _DEBUG
    }
#pragma endregion

#pragma region SRV�̐ݒ�Ɛ���
    // �V�F�[�_���\�[�X�r���[�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureResourceDesc.Format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

    // �n���h���̂����ʒu�ɃV�F�[�_�[���\�[�X�r���[�̍쐬
    iDX->GetDevice()->CreateShaderResourceView(tmp.info_.buff_.Get(), &srvDesc, tmp.info_.srvCpuHandle_);
#pragma endregion

#pragma region srvGpuHandle�̕ۑ���map�ɕR�Â��ĕۑ�
    // MAP_VALUE��srvGPUHandle_�֏�������
    tmp.info_.srvGpuHandle_ = srvHeap_.Get()->GetGPUDescriptorHandleForHeapStart(); // Desc�̃q�[�v�̈�̃X�^�[�g�ʒu���擾
    // �n���h����i�߂�
    tmp.info_.srvGpuHandle_.ptr += static_cast<size_t>(incrementSize) * static_cast<size_t>(handle);

    // map�ւ̑}���ior ����j������̏ꍇ�A����KEY�ɑ΂���VALUE���㏑������邽�ߒ���
    textures_.insert_or_assign(tmp.name_, tmp.info_); // ������ł����Ă��S������VALUE������Ƃ͎v����B
#pragma endregion
}

void Texture::CreateIdForTexPath(const std::string& relativePath, const std::string& fileName, const std::string& id)
{
    // �ꎞobj
    Texture tmp;

    // path�̊m�F�ƏC��
    tmp.CheckPath(relativePath, fileName);

    // ���ɓǂݍ���ł���e�N�X�`���̒��Ɉ�v������̂����邩�m�F
    if (textures_.count(tmp.name_)) {
        // ����ꍇ�AID�ƕR�t���ĕۑ�
        mapKeys_.insert_or_assign(id, tmp.name_);
    }
    else {
        try {
            // �Ȃ��ꍇ�A��O�𓊂���
            throw std::logic_error("Specified TEXTURE_KEY does not exist.");
        }
        catch (const std::logic_error&) {
            // �����I���B
            std::exit(1);
        }
    }
}

void Texture::CreateIdForTexPath(const std::string& pathAndFileName, const std::string& id)
{
    // ���ɓǂݍ���ł���e�N�X�`���̒��Ɉ�v������̂����邩�m�F
    if (textures_.count(pathAndFileName)) {
        // ����ꍇ�AID�ƕR�t���ĕۑ�
        mapKeys_.insert_or_assign(id, pathAndFileName);
    }
    else {
        try {
            // �Ȃ��ꍇ�A��O�𓊂���
            throw std::logic_error("Specified TEXTURE_KEY does not exist.");
        }
        catch (const std::logic_error&) {
            // �����I���B
            std::exit(1);
        }
    }
}

const Texture::TEXTURE_KEY Texture::GetTextureKey(const std::string& id)
{
    return mapKeys_.at(id);
}

const Texture Texture::GetTexture(const std::string& relativePath, const std::string& fileName)
{
    Texture tmp{}; // �ꎞobj

    tmp.CheckPath(relativePath, fileName);
    tmp.info_ = textures_.at(tmp.name_);

    return tmp;
}

const Texture Texture::GetTexture(const std::string& pathAndFileName)
{
    Texture tmp{}; // �ꎞobj

    tmp.SetMapKey(pathAndFileName);
    tmp.info_ = textures_.at(tmp.name_);

    return tmp;
}

const Texture Texture::GetTextureById(const std::string& id)
{
    Texture tmp{}; // �ꎞobj

    tmp.SetMapKey(GetTextureKey(id));
    tmp.info_ = textures_.at(tmp.name_);

    return tmp;
}

const Texture::TEXTURE_VALUE Texture::GetTextureInfo(const std::string& relativePath, const std::string& fileName)
{
    std::string name{};

    // relativePath�̖�����"/"�����邩�m�F
    if (!std::string{ relativePath.back() }.compare("/")) { // ��v���Ă���ꍇ0��Ԃ��̂�!�Ŕ�0�Ƃ��Ă���B
        // ����ꍇ�A���̂܂ܖ��O��ݒ�
        name = relativePath + fileName;
    }
    else {
        // ���Ȃ��ꍇ�A"/"�������Ė��O��ݒ�
        name = relativePath + "/" + fileName;
    }

    return textures_.at(name);
}

const Texture::TEXTURE_VALUE Texture::GetTextureInfo(const std::string& pathAndFileName)
{
    return textures_.at(pathAndFileName);
}

const Texture::TEXTURE_VALUE Texture::GetTextureInfoById(const std::string& id)
{
    return textures_.at(GetTextureKey(id));
}

void Texture::SetMapKey(const std::string keyName)
{
    name_ = keyName;
}

void Texture::CheckPath(const std::string& relativePath, const std::string& fileName)
{
    // relativePath�̖�����"/"�����邩�m�F
    if (!std::string{ relativePath.back() }.compare("/")) { // ��v���Ă���ꍇ0��Ԃ��̂�!�Ŕ�0�Ƃ��Ă���B
        // ����ꍇ�A���̂܂ܖ��O��ݒ�
        SetMapKey(relativePath + fileName);
    }
    else {
        // ���Ȃ��ꍇ�A"/"�������Ė��O��ݒ�
        SetMapKey(relativePath + "/" + fileName);
    }
}
