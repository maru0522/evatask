#pragma once
#include "Audio.h"
#include "Camera.h"
#include "Obj3d.h"

class GameScene
{
public:
    static GameScene* GetInstance(void);

public:
    void Initialize(void);
    void Update(void);
    void Draw(void);
    void AudioFinalize(void);

public:
    XAudio::SoundData soundData1 = XAudio::Load("Resources/Sounds/SE_footstep.wav");
    XAudio::SoundData soundData2 = XAudio::Load("Resources/Sounds/punpkin_mansion.wav");

    Camera cameraT{};

    // égópÇ∑ÇÈïœêîêÈåæ
    //Obj3d objT{ "Resources/3dModels/cube/cube.obj", &cameraT };
    //Obj3d objT2{ "Resources/3dModels/cube/cube.obj", &cameraT };
    //Obj3d muso{ "Resources/3dModels/muso/muso.obj" , &cameraT };

private:
    GameScene(void) = default;
    ~GameScene(void) = default;
    GameScene(const GameScene& ins) = delete;
    GameScene& operator=(const GameScene& ins) = delete;
};

