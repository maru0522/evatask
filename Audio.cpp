#include "Audio.h"
#include <wrl.h>
#include <fstream>
#include <cassert>

Microsoft::WRL::ComPtr<IXAudio2> XAudio::xAudio2_{ nullptr };
IXAudio2MasteringVoice* XAudio::masterVoice_{ nullptr };

void XAudio::Initialize(void)
{
    HRESULT r = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    r = xAudio2_->CreateMasteringVoice(&masterVoice_);
}

XAudio::SoundData XAudio::Load(const fsPath pathAndFileName)
{
    std::ifstream file;
    file.open(pathAndFileName, std::ios_base::binary);
    assert(file.is_open());

    RiffHeader riff;
    file.read((char*)&riff, sizeof(riff));
    if (std::strncmp(riff.chunk.id, "RIFF", 4) != 0) assert(0);
    if (std::strncmp(riff.type, "WAVE", 4) != 0) assert(0);

    FormatChunk format{};
    file.read((char*)&format, sizeof(ChunkHeader));
    if (std::strncmp(format.chunk.id, "fmt ", 4) != 0) assert(0);
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char*)&format.fmt, format.chunk.size);

    ChunkHeader data;
    file.read((char*)&data, sizeof(data));
    if (std::strncmp(data.id, "JUNK", 4) == 0) {
        file.seekg(data.size, std::ios_base::cur);
        file.read((char*)&data, sizeof(data));
    }
    if (std::strncmp(data.id, "data", 4) != 0) assert(0);
    char* pBuffer = new char[data.size];
    file.read(pBuffer, data.size);
    file.close();

    SoundData soundData{};

    soundData.wfex = format.fmt;
    soundData.pBuffer = reinterpret_cast<uint8_t*>(pBuffer);
    soundData.bufferSize = data.size;

    return soundData;
}

void XAudio::UnLoad(SoundData* soundData)
{
    delete[] soundData->pBuffer;

    soundData->pBuffer = 0;
    soundData->bufferSize = 0;
    soundData->wfex = {};
}

void XAudio::PlayWave(const SoundData& soundData, float_t volume)
{
    IXAudio2SourceVoice* pSourceVoice{ nullptr };
    HRESULT r = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
    assert(SUCCEEDED(r));

    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundData.pBuffer;
    buf.AudioBytes = soundData.bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    r = pSourceVoice->SubmitSourceBuffer(&buf);
    pSourceVoice->SetVolume(volume);
    r = pSourceVoice->Start();
}
