#include "GameScene.h"
#include "Input.h"
#include"easing.h"

using namespace DirectX;

bool CollsionAABB(WorldCoordinate a, WorldCoordinate b);
bool CollsionSphere(XMFLOAT3 posA, float rA, XMFLOAT3 posB, float rB);
XMFLOAT3 CollsionBackAABB(WorldCoordinate a, WorldCoordinate b, XMFLOAT3 vec);

GameScene* GameScene::GetInstance(void)
{
    static GameScene instance;
    return &instance;
}

void GameScene::Initialize(void)
{
    railcamera.Initialize({}, {});

    YUKA = { "Resources/3dModels/cube/cube.obj" ,railcamera.getView() };
    YUKA2 = { "Resources/3dModels/cube/cube.obj" ,railcamera.getView() };
    TitlePoll = { "Resources/3dModels/cube/cube.obj" ,railcamera.getView() };
    TitleKanban = { "Resources/3dModels/Title/Title.obj" ,railcamera.getView() };

    
    YUKA.worldCoordinate_.scale_ = { 200.0f,1.0f,200.0f };
    YUKA.worldCoordinate_.position_.y = -10.0f;
    YUKA.Update();
    YUKA2.worldCoordinate_.scale_ = { 25.0f,2.0f,100.0f };
    YUKA2.worldCoordinate_.position_ = { 0,-10.0f,-300.0f };
    YUKA2.worldCoordinate_.rotation_.z = XMConvertToRadians(180.0f);
    YUKA2.Update();
    TitlePoll.worldCoordinate_.scale_ = { 0.3f,1.5f,0.3f };
    TitlePoll.worldCoordinate_.position_ = { -20.0f,-6.5f,-350.0f };
    TitlePoll.worldCoordinate_.rotation_.y = XMConvertToRadians(30.0f);
    TitlePoll.Update();
    TitleKanban.worldCoordinate_.scale_ = { 1.0f,1.0f,1.0f };
    TitleKanban.worldCoordinate_.position_ = { -20.0f,-5.0f,-350.0f };
    TitleKanban.worldCoordinate_.rotation_.y = XMConvertToRadians(30.0f);
    TitleKanban.Update();
    boss = new bosstest();
    boss->Initialize(&railcamera, {0,100.0f,0});
    player.Initialize(&railcamera, boss);
}

void GameScene::Update(void)
{
    switch (sceneNum)
    {
    case Title:
        if (BlackCcr)
        {
            bscrBle = easeInSine(bscrBle, 0, BlkScrTimer / BlkScrTime);
            BlkScrTimer++;
            if (BlkScrTimer > BlkScrTime)
            {
                BlkScrTimer = 0;
                BlackCcr = false;
            }

        }
        

        TitleA.SetAnchorPoint({ 0.5f, 0.5f});
        TitleA.SetPosition({ 960.0f,360.0f });

        TitleA.Update();
        railcamera.Update();
        railcamera.setPos(easeOutSineVec3(
            XMFLOAT3((sinf(cameraRotateY) * 20 + TitleKanban.worldCoordinate_.position_.x-5), (sinf(-cameraRotateX) * 20 + TitleKanban.worldCoordinate_.position_.y), (cosf(cameraRotateY) * 20 + TitleKanban.worldCoordinate_.position_.z)),
            XMFLOAT3((sinf(cameraRotateY) * 20 + player.GetWorldPosition().x), (sinf(-cameraRotateX) * 20 + player.GetWorldPosition().y + 5), (cosf(cameraRotateY) * 20 + player.GetWorldPosition().z)),
            TitleCameraTimer/TitleCameraTime));
        railcamera.setRotate(easeOutSineVec3({ rotateX,XMConvertToRadians(30.0f),0}, {rotateX,rotateY,0}, TitleCameraTimer / TitleCameraTime));
        if ((KEYS::IsTrigger(DIK_SPACE) || XPAD::IsTrigger(XINPUT_GAMEPAD_A))&& !BlackCcr)
        {
            StartFlag = true;
            boss->reset();

        }
        if (StartFlag)
        {
            TitleCameraTimer++;
            if (TitleCameraTime < TitleCameraTimer)
            {
                TitleCameraTimer = TitleCameraTime;
                sceneNum = Main;
            }

        }
    case Main:
        railcamera.Update();



        cameraX = railcamera.GetWorldPosition().x;
        cameraZ = railcamera.GetWorldPosition().z;
        if (StartFlag)
        {
            if (KEYS::IsDown(DIK_W))
            {
                if (cameraRotateX < 0.27f)
                {
                    cameraRotateX += 0.01f;
                    rotateX -= 0.01f;
                }

            }

            if (KEYS::IsDown(DIK_S))
            {
                if (cameraRotateX > -0.6f)
                {
                    cameraRotateX -= 0.01f;
                    rotateX += 0.01f;
                }

            }

            XMFLOAT2 inputnum = XPAD::GetRStick();
            cameraRotateY += (float)inputnum.x / SHRT_MAX * 0.02f;
            rotateY += (float)inputnum.x / SHRT_MAX * 0.02f;
            if ((cameraRotateX < 0.27f && (float)inputnum.y / SHRT_MAX>0) || (cameraRotateX > -0.6f && (float)inputnum.y / SHRT_MAX < 0))
            {
                cameraRotateX += (float)inputnum.y / SHRT_MAX * 0.02f;
                rotateX -= (float)inputnum.y / SHRT_MAX * 0.02f;
            }

            if (KEYS::IsDown(DIK_D))
            {
                cameraRotateY += 0.01f;
                rotateY += 0.01f;
            }

            if (KEYS::IsDown(DIK_A))
            {
                cameraRotateY -= 0.01f;
                rotateY -= 0.01f;
            }

#ifdef _DEBUG
            if (KEYS::IsTrigger(DIK_I))
            {
                boss.setisAttackFlagL(true, player.GetWorldPosition());
            }

            if (KEYS::IsTrigger(DIK_O))
            {
                boss.playerAttackReturnL();
            }

            if (KEYS::IsTrigger(DIK_K))
            {
                boss.setisBossPress(true);
            }

            if (KEYS::IsTrigger(DIK_J))
            {
                boss.setisBossBeam(true);
            }

            if (KEYS::IsTrigger(DIK_L))
            {
                boss.setisBossPushUp(true);
            }
#endif
            player.Update(&railcamera);
        }

       
        AllCol();
        if (!BattleFlag)
        {
            if (player.GetWorldPosition().x > 25.0f && player.GetWorldPosition().z < -200.0f)
            {
                player.SetWorldPosition({ 25.0f,player.GetWorldPosition().y,player.GetWorldPosition().z });
            }
            if (player.GetWorldPosition().x < -25.0f && player.GetWorldPosition().z < -200.0f)
            {
                player.SetWorldPosition({ -25.0f,player.GetWorldPosition().y,player.GetWorldPosition().z });
            }
            if (player.GetWorldPosition().z < -400.0f)
            {
                player.SetWorldPosition({ player.GetWorldPosition().x,player.GetWorldPosition().y,-400.0f });
            }
        }
        if (player.GetWorldPosition().z > -200.0f&&!BattleFlag)
        {
            BattleFlag = true;
            BossCameraTimer = 0;
            boss->SetStartFlag(true);
            sceneNum = MoveScene1;
        }
        if (BattleFlag)
        {
            if (player.GetWorldPosition().x > 200.0f)
            {
                player.SetWorldPosition({ 200.0f,player.GetWorldPosition().y,player.GetWorldPosition().z });
            }
            if (player.GetWorldPosition().x < -200.0f)
            {
                player.SetWorldPosition({ -200.0f,player.GetWorldPosition().y,player.GetWorldPosition().z });
            }
            if (player.GetWorldPosition().z < -200.0f)
            {
                player.SetWorldPosition({ player.GetWorldPosition().x,player.GetWorldPosition().y,-200.0f });
            }
            if (player.GetWorldPosition().z > 200.0f)
            {
                player.SetWorldPosition({ player.GetWorldPosition().x,player.GetWorldPosition().y,200.0f });
            }
            boss->Update(player.GetWorldPosition());
            if (boss->getHP() < 0)
            {
                sceneNum = Result;
            }
        }
        railcamera.setPos(XMFLOAT3((sinf(cameraRotateY) * 20 + player.GetWorldPosition().x), (sinf(-cameraRotateX) * 20 + player.GetWorldPosition().y + 5), (cosf(cameraRotateY) * 20 + player.GetWorldPosition().z)));
        railcamera.setRotate({ rotateX,rotateY,0 });

        
        YUKA.Update();
        YUKA2.Update();
        TitlePoll.Update();
        TitleKanban.Update();
        if (player.GetIsdead())
        {
            sceneNum = GameOver;
        }
        

        break;
    case MoveScene1:
        
        boss->bossStart({ 0,100.0f,0 });
        if (!boss->GetStartFlag())
        {
            sceneNum = Main;
        }


        break;
    case Result:
        GameClearLogo.SetPosition({ 1280.0f / 2.0f,720.0f / 4.0f });
        GameClearLogo.SetSize({ 800.0f,300.0f });
        GameClearLogo.SetAnchorPoint({ 0.5f,0.5f });
        GameClearLogo.Update();
        if (XPAD::IsTrigger(XINPUT_GAMEPAD_A) or KEYS::IsTrigger(DIK_SPACE))
        {
            //railcamera.Initialize({}, {0,0,0});

            BlackCcr = true;
        }
        if (BlackCcr)
        {
            bscrBle = easeInSine(bscrBle, 255, BlkScrTimer / BlkScrTime);
            BlkScrTimer++;
            if (BlkScrTimer > BlkScrTime)
            {
                BlkScrTimer = BlkScrTime;
                player.Initialize(&railcamera,boss);
                rotateX = 0;

                cameraX = 0;
                cameraZ = 0;
                rotateY = 0;

                cameraRotateX = 0;
                cameraRotateY = 9.45f;

                StartFlag = false;
                TitleCameraTimer = 0;
                BlkScrTimer = 0;
                BattleFlag = false;
                boss->reset();
                sceneNum = Title;
            }

        }

        break;
    case GameOver:
        railcamera.Update();
        cameraRotateY -= 0.01f;
        rotateY -= 0.01f;
        player.GameOverUpdate();

        GameOverLogo.SetPosition({ 1280.0f / 2.0f,720.0f / 4.0f });
        GameOverLogo.SetSize({ 800.0f,300.0f });
        GameOverLogo.SetAnchorPoint({ 0.5f,0.5f });
        GameOverLogo.Update();
        railcamera.setPos(XMFLOAT3((sinf(cameraRotateY) * 20 + player.GetWorldPosition().x), (sinf(-cameraRotateX) * 20 + player.GetWorldPosition().y + 5), (cosf(cameraRotateY) * 20 + player.GetWorldPosition().z)));
        railcamera.setRotate({ rotateX,rotateY,0 });
        if (XPAD::IsTrigger(XINPUT_GAMEPAD_A) or KEYS::IsTrigger(DIK_SPACE))
        {
            //railcamera.Initialize({}, {0,0,0});
           
            BlackCcr = true;
        }
        if (BlackCcr)
        {
            bscrBle = easeInSine(bscrBle, 255, BlkScrTimer / BlkScrTime);
            BlkScrTimer++;
            if (BlkScrTimer > BlkScrTime)
            {
                BlkScrTimer = BlkScrTime;
                player.Initialize(&railcamera, boss);
                rotateX = 0;

                cameraX = 0;
                cameraZ = 0;
                rotateY = 0;

                cameraRotateX = 0;
                cameraRotateY = 9.45f;

                StartFlag = false;
                TitleCameraTimer = 0;
                BlkScrTimer = 0;
                BattleFlag = false;
                boss->reset();
                sceneNum = Title;
            }

        }
       

        break;
    }
   // cameraT.Update();
    Bscr.SetAnchorPoint({ 0.5f,0.5f });
    Bscr.SetSize({ 1280.0f,720.0f });
    Bscr.SetPosition({ 640.0f,360.0f });
    Bscr.SetColor255(255.0f, 255.0f, 255.0f, bscrBle);
    Bscr.Update();
}

void GameScene::Draw(void)
{
   /* objT.Draw();
    objT2.Draw();
    muso.Draw();*/
    switch (sceneNum)
    {
    case Title:
        YUKA.Draw();
        YUKA2.Draw();
        TitlePoll.Draw();
        TitleKanban.Draw();
        player.Draw(&railcamera);
        if (!StartFlag)
        {
            TitleA.Draw();
        }
        break;
    case Main:
        YUKA.Draw();
        YUKA2.Draw();
        TitlePoll.Draw();
        TitleKanban.Draw();
        if(BattleFlag)boss->Draw();
        player.Draw(&railcamera);
        player.DrawUI(&railcamera);
        if(BattleFlag)boss->DrawUI();
        break;

    case MoveScene1:
        YUKA.Draw();
        YUKA2.Draw();
        TitlePoll.Draw();
        TitleKanban.Draw();
        boss->Draw();
        player.Draw(&railcamera);
        break;
    case Result:
        GameClearLogo.Draw();
        break;
    case GameOver:
        player.Draw(&railcamera);
        GameOverLogo.Draw();


        break;
    }

   
    Bscr.Draw();
   
}

void GameScene::AudioFinalize(void)
{
    player.FinalizeSound();
    boss->FinalizeSound();
    XAudio::UnLoad(&bgm);

    XAudio::UnLoad(&SE_wind);
    delete(boss);
}

void GameScene::AllCol()
{
    player.SetWorldPosition(CollsionBackAABB(player.GetHitArea(), YUKA.worldCoordinate_, player.PlayerMoveVec()));
    player.SetWorldPosition(CollsionBackAABB(player.GetHitArea(), YUKA2.worldCoordinate_, player.PlayerMoveVec()));
    

    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player.GetBullets();

    std::vector<bossHand*> bosshands = boss->getHand();
    //ボスハンドのリスト化したユニークptr変数

    for (const std::unique_ptr<PlayerBullet>& p_bullet : playerBullets)
    {
        for (int i = 0; i < bosshands.size(); i++)
        {
            if (CollsionSphere(p_bullet->GetWorldPosition(), p_bullet->GetScale().x, bosshands[i]->GetwroldTransform().position_, bosshands[i]->GetwroldTransform().scale_.x) && bosshands[i]->getisAttackFlag())
            {
                bosshands[i]->playerAttackReturn();
                p_bullet->OnCollision();
            }
        }
    }

    for (const std::unique_ptr<PlayerBullet>& p_bullet : playerBullets)
    {
        if (CollsionSphere(p_bullet->GetWorldPosition(), p_bullet->GetScale().x, boss->GetWorldPosition(), boss->getPos().scale_.y))
        {
            boss->OnCollision(2);
            p_bullet->OnCollision();
        }

    }

    for (int i = 0; i < bosshands.size(); i++)
    {

        if (CollsionSphere(boss->GetWorldPosition(), boss->getPos().scale_.x, bosshands[i]->GetwroldTransform().position_, bosshands[i]->GetwroldTransform().scale_.x) && bosshands[i]->getisReturnHand())
        {

            boss->OnCollision(50);

        }

    }

    for (int i = 0; i < bosshands.size(); i++)
    {

        if (CollsionSphere(player.GetWorldPosition(), player.GetHitArea().scale_.y, bosshands[i]->GetwroldTransform().position_, bosshands[i]->GetwroldTransform().scale_.x))
        {

            player.OnCollision();
            break;

        }

    }

}

bool CollsionAABB(WorldCoordinate a, WorldCoordinate b)
{
    float aLeft = a.position_.x - a.scale_.x;
    float aRight = a.position_.x + a.scale_.x;
    float aTop = a.position_.y + a.scale_.y;
    float aBottom = a.position_.y - a.scale_.y;
    float aFlont = a.position_.z + a.scale_.z;
    float aBack = a.position_.z - a.scale_.z;

    float bLeft = b.position_.x - b.scale_.x;
    float bRight = b.position_.x + b.scale_.x;
    float bTop = b.position_.y + b.scale_.y;
    float bBottom = b.position_.y - b.scale_.y;
    float bFlont = b.position_.z + b.scale_.z;
    float bBack = b.position_.z - b.scale_.z;

    if (
        aLeft < bRight && bLeft < aRight && aBottom < bTop && bBottom < aTop && aBack < bFlont &&
        bBack < aFlont) {
        return true;
    }
    else {
        return false;
    }
}

bool CollsionSphere(XMFLOAT3 posA, float rA, XMFLOAT3 posB, float rB)
{
   XMFLOAT3 ABrange = posB - posA;

    float range = length(ABrange);

    if (range < rA + rB)
    {
        return true;
    }
    else
    {
        return false;
    }
}

XMFLOAT3 CollsionBackAABB(WorldCoordinate a, WorldCoordinate b, XMFLOAT3 vec)
{
   XMFLOAT3 aa = a.position_;

   

    if (CollsionAABB(a, b))
    {
        if (
            vec.x > 0 && a.position_.x + a.scale_.x > b.position_.x - b.scale_.x &&
            a.position_.x <= b.position_.x - b.scale_.x)
        {
            aa.x = b.position_.x - b.scale_.x - a.scale_.x;
        }
        if (vec.x < 0 && a.position_.x - a.scale_.x < b.position_.x + b.scale_.x &&
            a.position_.x >= b.position_.x + b.scale_.x)
        {
            aa.x = b.position_.x + b.scale_.x + a.scale_.x;
        }
        if (
            vec.y > 0 && a.position_.y + a.scale_.y > b.position_.y - b.scale_.y &&
            a.position_.y <= b.position_.y - b.scale_.y)
        {
            aa.y = b.position_.y - b.scale_.y - a.scale_.y;
        }
        if (
            vec.y < 0 && a.position_.y - a.scale_.y < b.position_.y + b.scale_.y &&
            a.position_.y >= b.position_.y + b.scale_.y)
        {
            aa.y = b.position_.y + b.scale_.y + a.scale_.y;
        }
        if (vec.z > 0 && a.position_.z + a.scale_.z > b.position_.z - b.scale_.z &&
            a.position_.z <= b.position_.z - b.scale_.z)
        {
            aa.z = b.position_.z - b.scale_.z - a.scale_.z;
        }
        if (
            vec.z < 0 && a.position_.z - a.scale_.z < b.position_.z + b.scale_.z &&
            a.position_.z >= b.position_.z + b.scale_.z)
        {
            aa.z = b.position_.z + b.scale_.z + a.scale_.z;
        }
    }



    return aa;

}
