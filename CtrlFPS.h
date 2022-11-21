#pragma once
#include <chrono>

class CtrlFPS
{
public: // �ÓI�ϐ�
    static constexpr std::chrono::microseconds kMinTime_{uint64_t(1000000.0f / 60.0f)}; // 1/60�b�҂�����̎���
    static constexpr std::chrono::microseconds kMiCheckTime_{uint64_t(1000000.0f / 65.0f)}; // 1/60�b���͂��ɒZ������

public: // �֐�
    void Initialize(void);
    void Update(void);

private: // �ϐ�
    std::chrono::steady_clock::time_point timeRef_; // ���ԋL�^�iFPS�Œ�p�j
};