#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<d3dcompiler.h>
#include<DirectXMath.h>
#include<math.h>
using namespace DirectX;
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include<vector>
#include<string>
#include<DirectXTex.h>
#include<wrl.h>

#include "Window.h"
#include "Input.h"
#include "InitDirectX.h"
#include "Texture.h"
#include "Sprite.h"
#include "Cube.h"
#include "Camera.h"
#include "Model.h"
#include "Obj3d.h"
#include "Audio.h"
#include "GameScene.h"

using namespace Microsoft::WRL;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // WinAPI初期化
    Window* wnd = Window::GetInstance();

    // DirectX初期化
    InitDirectX* iDX = InitDirectX::GetInstance();
    iDX->Initialize();

    // Input初期化
    KEYS::Initialize();
    DPAD::Initialize();
    XPAD::Initialize();

#pragma region 描画初期化処理
    // graphicsPipeline初期化
    GraphicsPipeline::Initialize();

    Obj3d::Initialize();
#pragma endregion

#pragma region リソース読み込み
    // テクスチャ読み込み
    Texture::Initialize();
    Texture::Load("Resources/reimu.png");
    Texture::Load("Resources/mario.jpg");
    Texture::Load("Resources/Reticle.png");
    Texture::Load("Resources/mark.png");
    Texture::Load("Resources/Abutton.png");
    Texture::Load("Resources/kuro.png");
    Texture::Load("Resources/Helth.png");
    Texture::Load("Resources/Shield.png");
    Texture::Load("Resources/GameClear.png");
    Texture::Load("Resources/GameOver.png");
    Texture::Load("Resources/PadUiAButton.png");
    

    // 3d.obj読み込み
    Model::Load("Resources/3dModels/cube/cube.obj");
    Model::Load("Resources/3dModels/muso/muso.obj");
    Model::Load("Resources/3dModels/player/Player.obj");
    Model::Load("Resources/3dModels/core/core.obj");
    Model::Load("Resources/3dModels/bit/bit.obj");
    Model::Load("Resources/3dModels/arrow/arrow.obj");
    Model::Load("Resources/3dModels/Title/Title.obj");

    XAudio::Initialize();

    GameScene* gameS = GameScene::GetInstance();
    gameS->Initialize();

#pragma endregion

    // ゲームループ
    while (true) {
        // ウィンドウメッセージ処理
        if (!wnd->GetKeepWindow()) {
            break;
        }

#pragma region DirextX毎フレーム処理
        // キーボード情報の取得
        KEYS::Update();
        DPAD::Update();
        XPAD::Update();
        // 更新処理　ここから

        gameS->Update();

        // 更新処理　ここまで
#pragma endregion

#pragma region 描画開始
        iDX->PreDraw();
#pragma endregion
        Obj3d::PreDraw();
        // 描画処理　ここから

        gameS->Draw();

        // 描画処理　ここまで
#pragma region 描画終了
        iDX->PostDraw();
#pragma endregion
    }

    XAudio::Reset();
    gameS->AudioFinalize();

    // ウィンドウクラスを登録解除
    wnd->DelWindow();

    return 0;
}