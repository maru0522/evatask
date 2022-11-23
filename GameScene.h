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

public:
    XAudio::SoundData soundData1 = XAudio::Load("Resources/Sounds/SE_footstep.wav");
    XAudio::SoundData soundData2 = XAudio::Load("Resources/Sounds/punpkin_mansion.wav");

    // égópÇ∑ÇÈïœêîêÈåæ

    RailCamera railcamera;

    bosstest boss;
    
    Player player;

    float x = 0;
    float z = 0;

    Particle p1_;

private:
    GameScene(void) = default;
    ~GameScene(void) = default;
    GameScene(const GameScene& ins) = delete;
    GameScene& operator=(const GameScene& ins) = delete;
};

