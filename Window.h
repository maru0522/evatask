#pragma once
#include <Windows.h>

class Window
{
public: // �ÓI�֐�
    static Window* GetInstance(); // �C���X�^���X���擾
    static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // �ÓI�ϐ�
    // �E�B���h�E�T�C�Y
    static constexpr size_t width_ = 1280;
    static constexpr size_t height_ = 720;

    // �E�B���h�E�N���X��
    static const wchar_t windowClassName_[];

public: // �֐�
    Window();
    Window(const char* title);
    void DelWindow();

    // �E�B���h�E�̕\����Ԃ�ύX����֐�
    // �f�t�H���g:nCmdShow = SW_SHOW
    void SetDisplay(int nCmdShow);

    // �E�B���h�E�Œ�p�̃��b�Z�[�W���[�v����
    // WM_QUIT�Ȃ�FALSE��Ԃ�
    bool GetKeepWindow();

    // �E�B���h�E�n���h���̎擾
    // const: �����n�����郁���o�ϐ��̃����o�֐������ɂ�����ύX�̋֎~
    HWND GetHwnd() const { return hwnd_; }

    HINSTANCE GetHInstance() const { return w_.hInstance; }

private: // �ϐ�
    // �E�B���h�E�N���X�̐���
    WNDCLASSEX w_{};

    // �E�B���h�E�n���h���̐���
    HWND hwnd_;
};