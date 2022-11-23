#pragma once
#include "Audio.h"
#include "Camera.h"
#include "Obj3d.h"
#include "bosstest.h"
#include "RailCamera.h"
#include "Player.h"

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
    XAudio::SoundData bgm = XAudio::Load("Resources/Sounds/punpkin_mansion.wav"); // bgm
    XAudio::SoundData SE_playerFoot = XAudio::Load("Resources/Sounds/SE_footstep.wav"); // プレイヤーの足音
    XAudio::SoundData SE_BOSSAttack = XAudio::Load("Resources/Sounds/SE_BOSS_1.wav"); // ボスの攻撃
    XAudio::SoundData SE_BOSSHeavy = XAudio::Load("Resources/Sounds/SE_BossAttack.wav"); // ボスの強い攻撃　音量注意
    XAudio::SoundData SE_playerAttack = XAudio::Load("Resources/Sounds/SE_gunfire.wav"); // プレイヤー攻撃　音量超注意
    XAudio::SoundData SE_wind = XAudio::Load("Resources/Sounds/SE_wind.wav"); // 風の音　音量注意
    XAudio::SoundData SE_spark = XAudio::Load("Resources/Sounds/SE_spark.wav"); // スパークの音（パチパチ）　音量超注意


    //Camera cameraT{};

    // 使用する変数宣言
   /* Obj3d objT{ "Resources/3dModels/player/Player.obj", &cameraT };
    Obj3d objT2{ "Resources/3dModels/bit/bit.obj", &cameraT };
    Obj3d muso{ "Resources/3dModels/muso/muso.obj" , &cameraT };*/

    RailCamera railcamera;

    Obj3d YUKA = Obj3d{ "Resources/3dModels/cube/cube.obj" };
    Obj3d YUKA2 = Obj3d{ "Resources/3dModels/cube/cube.obj" };
    Obj3d TitlePoll = Obj3d{ "Resources/3dModels/cube/cube.obj" };
    Obj3d TitleKanban = Obj3d{ "Resources/3dModels/Title/Title.obj" };

    Sprite TitleA{ "Resources/Abutton.png" ,CMode::PATH };
    Sprite Bscr{ "Resources/kuro.png" ,CMode::PATH };
    Sprite GameOverLogo{ "Resources/GameOver.png" ,CMode::PATH };
    Sprite GameClearLogo{ "Resources/GameClear.png" ,CMode::PATH };

    bosstest boss;
    
    Player player;

    float x = 0;
    float z = 0;

private:

    enum Scene
    {
        Title,
        Main,
        MoveScene1,
        MoveScene2,
        MoveScene3,
        Result,
        GameOver
    };
    GameScene(void) = default;
    ~GameScene(void) = default;
    GameScene(const GameScene& ins) = delete;
    GameScene& operator=(const GameScene& ins) = delete;

    float rotateX = 0;

    float cameraX = 0;
    float cameraZ = 0;
    float rotateY = 0;

    float cameraRotateX = 0;
    float cameraRotateY = 9.45f;

    float TitleCameraTimer;
    const float TitleCameraTime = 60;
    bool StartFlag = false;

    float BossCameraTimer=0;
    const float BossCameraTime = 200;
    

    bool BattleFlag = false;

    int sceneNum = GameOver;

    bool BlackCcr = false;
    float BlkScrTimer;
    const float BlkScrTime=90;

    float bscrBle;
  
};

