#include "CtrlFPS.h"
#include <thread>

void CtrlFPS::Initialize(void)
{
    // ���ݎ��Ԃ��L�^����
    timeRef_ = std::chrono::steady_clock::now();
}

void CtrlFPS::Update(void)
{
    //�@���ݎ��Ԃ��擾����
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    // �O��L�^����̌o�ߎ��Ԃ��擾����
    std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - timeRef_);

    // 1/60�b�i���͂��ɒZ�����ԁj�o���Ă��Ȃ��ꍇ
    if (elapsed < kMinTime_) {
        // 1/60�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
        while (std::chrono::steady_clock::now() - timeRef_ < kMinTime_)
        {
            // 1�}�C�N���b�X���[�v
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }

    // ���݂̎��Ԃ��L�^
    timeRef_ = std::chrono::steady_clock::now();
}
