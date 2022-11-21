#pragma once
#include <fstream>
#include <wrl.h>
#if _MSC_VER > 1922 && !defined(_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif
#include <experimental/filesystem>
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

class XAudio
{
public: // ��`
    using AUDIO_KEY = std::string;
    struct AUDIO_VALUE {
        WAVEFORMATEX wfex;
        uint8_t* pBuffer;
        uint32_t bufferSize;
    };

    struct SoundData
    {
        WAVEFORMATEX wfex;
        uint8_t* pBuffer;
        uint32_t bufferSize;
    };

private: // ��`
    // �G�C���A�X�e���v���[�g
    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    using fsPath = std::experimental::filesystem::path;

    struct ChunkHeader
    {
        char id[4];
        int32_t size;
    };

    struct RiffHeader
    {
        ChunkHeader chunk;
        char type[4];
    };

    struct FormatChunk
    {
        ChunkHeader chunk;
        WAVEFORMATEX fmt;
    };

public: // �֐�
    static void Initialize(void);
    static SoundData Load(const fsPath pathAndFileName);
    static void UnLoad(SoundData* soundData);
    static void PlayWave(const SoundData& soundData, float_t volume);
    static void Reset(void) { xAudio2_.Reset(); }

private: // �ϐ�
    static ComPtr<IXAudio2> xAudio2_;
    static IXAudio2MasteringVoice* masterVoice_;
};