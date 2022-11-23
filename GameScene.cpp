#include "GameScene.h"
#include "Input.h"

GameScene* GameScene::GetInstance(void)
{
    static GameScene instance;
    return &instance;
}

void GameScene::Initialize(void)
{
    railcamera.Initialize({0,0,-30}, {});

    boss.Initialize(&railcamera, {0,0,50});
    player.Initialize(&railcamera, &boss);

    
}

void GameScene::Update(void)
{
    railcamera.Update();

    if (KEYS::IsTrigger(DIK_SPACE)) {
        XAudio::PlayWave(soundData2, 0.03f);
        p1_.Activate(&railcamera, { 0,0,3 });
    }
    if (KEYS::IsTrigger(DIK_RETURN)) {
        XAudio::PlayWave(soundData1, 0.1f);
    }

    if (XPAD::IsTrigger(XINPUT_GAMEPAD_A)) {
        XAudio::PlayWave(soundData2, 0.03f);
    }

    if (KEYS::IsDown(DIK_W)) {
        railcamera.getView()->eye_.z += 2;
    }
    if (KEYS::IsDown(DIK_S)) {
        railcamera.getView()->eye_.z -= 2;
    }

    boss.Update({});
    player.Update(&railcamera);

    p1_.Update();

    railcamera.setPos({ x,0,z });

}

void GameScene::Draw(void)
{
    //boss.Draw();
    player.Draw(&railcamera);
    //player.DrawUI(&railcamera);

    p1_.Draw();
}

void GameScene::AudioFinalize(void)
{
    XAudio::UnLoad(&soundData1);
    XAudio::UnLoad(&soundData2);
}
