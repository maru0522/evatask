#include "GameScene.h"
#include "Input.h"

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
    objT2.worldCoordinate_.position_ = { 15,0,0 };
    muso.worldCoordinate_.position_ = { -15,0,0 };

    railcamera.Initialize({}, {});

    YUKA = { "Resources/3dModels/cube/cube.obj" ,railcamera.getView() };
    
    YUKA.worldCoordinate_.scale_ = { 200.0f,1.0f,200.0f };
    YUKA.worldCoordinate_.position_.y = -10.0f;
    YUKA.Update();
    boss.Initialize(&railcamera, {0,0,50});
    player.Initialize(&railcamera, &boss);
}

void GameScene::Update(void)
{
    cameraT.Update();
    railcamera.Update();



    float cameraX = railcamera.GetWorldPosition().x;
    float cameraZ = railcamera.GetWorldPosition().z;

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
     cameraRotateY += (float)inputnum.x/ SHRT_MAX * 0.01f;
     rotateY += (float)inputnum.x / SHRT_MAX * 0.01f;
     if ((cameraRotateX < 0.27f && (float)inputnum.y / SHRT_MAX>0) || (cameraRotateX > -0.6f && (float)inputnum.y / SHRT_MAX < 0))
     {
         cameraRotateX += (float)inputnum.y / SHRT_MAX * 0.01f;
         rotateX -= (float)inputnum.y / SHRT_MAX * 0.01f;
     }

     //if (cameraRotateX < 0.27f)
     //{
     //	cameraRotateX = 0.27f;
     //	//rotateX = -0.27f;
     //}
     //if (cameraRotateX > -0.6f)
     //{
     //	cameraRotateX = -0.6f;
     //	//rotateX = 0.6f;
     //}
    

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
    
    if (KEYS::IsTrigger(DIK_SPACE)) {
        XAudio::PlayWave(soundData2, 0.03f);
    }
    if (KEYS::IsTrigger(DIK_RETURN)) {
        XAudio::PlayWave(soundData1, 0.1f);
    }

    objT.Update();
    objT2.Update();
    muso.Update();

    boss.Update({});
    objT2.Update();
    muso.Update();
    railcamera.setPos(XMFLOAT3((sinf(cameraRotateY) * 20 + player.GetWorldPosition().x), (sinf(-cameraRotateX) * 20 + player.GetWorldPosition().y + 5), (cosf(cameraRotateY) * 20 + player.GetWorldPosition().z)));
    railcamera.setRotate({ rotateX,rotateY,0 });
    boss.Update({});
    boss.Update(player.GetWorldPosition());
    YUKA.Update();

    AllCol();
   /* objT.Draw();
    objT2.Draw();
    muso.Draw();*/
    YUKA.Draw();
    boss.Draw();
}
    player.DrawUI(&railcamera);
    objT.Draw();
    objT2.Draw();
    muso.Draw();
    boss.Draw();
    player.Draw(&railcamera);
    //player.DrawUI(&railcamera);

    p1_.Draw();
}

void GameScene::AudioFinalize(void)
{
    XAudio::UnLoad(&soundData1);
    XAudio::UnLoad(&soundData2);
}

void GameScene::AllCol()
{
    player.SetWorldPosition(CollsionBackAABB(player.GetHitArea(), YUKA.worldCoordinate_, player.PlayerMoveVec()));

    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player.GetBullets();

    std::vector<bossHand*> bosshands = boss.getHand();
    //ボスハンドのリスト化したユニークptr変数

    for (const std::unique_ptr<PlayerBullet>& p_bullet : playerBullets)
    {
        for (int i = 0; i < bosshands.size(); i++)
        {
            if (CollsionSphere(p_bullet->GetWorldPosition(), p_bullet->GetScale().x, bosshands[i]->GetwroldTransform().position_, bosshands[i]->GetwroldTransform().scale_.x) && bosshands[i]->getisAttackFlag())
            {
                bosshands[i]->playerAttackReturn();
            }
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
