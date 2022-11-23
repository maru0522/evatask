#pragma once
#include "Audio.h"
#include "Camera.h"
#include "Obj3d.h"
#include "bosstest.h"
#include "RailCamera.h"
#include "Player.h"
#include "Particle.h"

class GameScene
{
public:
    static GameScene* GetInstance(void);

public:
    void Initialize(void);
    void Update(void);
    void Draw(void);
    void AudioFinalize(void);
    void AllCol();

public:
    XAudio::SoundData soundData1 = XAudio::Load("Resources/Sounds/SE_footstep.wav");
    XAudio::SoundData soundData2 = XAudio::Load("Resources/Sounds/punpkin_mansion.wav");

    // égópÇ∑ÇÈïœêîêÈåæ

    RailCamera railcamera;

    Obj3d YUKA = Obj3d{ "Resources/3dModels/cube/cube.obj" };

    bosstest boss;
    
    Player player;

    float x = 0;
    float z = 0;

private:
    GameScene(void) = default;
    ~GameScene(void) = default;
    GameScene(const GameScene& ins) = delete;
    GameScene& operator=(const GameScene& ins) = delete;

    float rotateX = 0;
    float rotateY = 0;

    float cameraRotateX = 0;
    float cameraRotateY = 9.45f;

    enum Scene
    {
        Main,
        Result,
        GameOver
    };
};

