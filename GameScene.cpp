#include "GameScene.h"
#include "Input.h"

GameScene* GameScene::GetInstance(void)
{
    static GameScene instance;
    return &instance;
}

void GameScene::Initialize(void)
{
    objT2.worldCoordinate_.position_ = { 15,0,0 };
    muso.worldCoordinate_.position_ = { -15,0,0 };
}

void GameScene::Update(void)
{
    cameraT.Update();

    if (KEYS::IsDown(DIK_W)) {
        cameraT.eye_.z += 5;
    }
    if (KEYS::IsDown(DIK_S)) {
        cameraT.eye_.z -= 5;
    }
    if (KEYS::IsDown(DIK_A)) {
        cameraT.eye_.x -= 5;
    }
    if (KEYS::IsDown(DIK_D)) {
        cameraT.eye_.x += 5;
    }

    if (KEYS::IsDown(DIK_LEFTARROW)) {
        objT.worldCoordinate_.position_.x -= 2;
    }
    if (KEYS::IsDown(DIK_RIGHTARROW)) {
        objT.worldCoordinate_.position_.x += 2;
    }

    if (KEYS::IsTrigger(DIK_SPACE)) {
        XAudio::PlayWave(soundData2, 0.03f);
    }
    if (KEYS::IsTrigger(DIK_RETURN)) {
        XAudio::PlayWave(soundData1, 0.1f);
    }

    objT.Update();
    objT2.Update();
    muso.Update();
}

void GameScene::Draw(void)
{
    objT.Draw();
    objT2.Draw();
    muso.Draw();
}

void GameScene::AudioFinalize(void)
{
    XAudio::UnLoad(&soundData1);
    XAudio::UnLoad(&soundData2);
}
