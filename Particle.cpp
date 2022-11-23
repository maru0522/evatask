#include "Particle.h"
#include "Obj3d.h"
#include <random>
#include "Util.h"

//std::random_device seed_gen;
//std::mt19937 engine(seed_gen());
//std::uniform_real_distribution<> dist(0, Util::Convert::MY_PI * 2);

void Particle::Activate(RailCamera* pCamera, DirectX::XMFLOAT3 startPos)
{
    startPos_ = startPos;

    isActive = true;

    for (int i = 0; i < cubes_.size(); ++i) {
        cubes_.at(i) = Obj3d{ "Resources/3dModels/cube/cube.obj" };
        cubes_.at(i).worldCoordinate_.scale_ = { 0.03f,0.03f,0.03f };
        cubes_.at(i).worldCoordinate_.position_ = startPos;
        cubes_.at(i).SetCamera(pCamera->getView());

        //randoms_.at(i) = dist(engine);
    }
}

void Particle::Update(void)
{
    if (!isActive) {
        return;
    }

    cubes_.at(0).worldCoordinate_.position_.x = (float)easeOutExpo(startPos_.x, startPos_.x + blownValue_, currentFrame_ / maxFrame_);
    cubes_.at(0).worldCoordinate_.position_.z = (float)easeOutExpo(startPos_.z, startPos_.z + blownValue_, currentFrame_ / maxFrame_);

    cubes_.at(1).worldCoordinate_.position_.x = (float)easeOutExpo(startPos_.x, startPos_.x - blownValue_, currentFrame_ / maxFrame_);
    cubes_.at(1).worldCoordinate_.position_.z = (float)easeOutExpo(startPos_.z, startPos_.z + blownValue_, currentFrame_ / maxFrame_);

    cubes_.at(2).worldCoordinate_.position_.x = (float)easeOutExpo(startPos_.x, startPos_.x - blownValue_, currentFrame_ / maxFrame_);
    cubes_.at(2).worldCoordinate_.position_.z = (float)easeOutExpo(startPos_.z, startPos_.z - blownValue_, currentFrame_ / maxFrame_);

    cubes_.at(3).worldCoordinate_.position_.x = (float)easeOutExpo(startPos_.x, startPos_.x + blownValue_, currentFrame_ / maxFrame_);
    cubes_.at(3).worldCoordinate_.position_.z = (float)easeOutExpo(startPos_.z, startPos_.z - blownValue_, currentFrame_ / maxFrame_);

    cubes_.at(4).worldCoordinate_.position_.z = (float)easeOutExpo(startPos_.z, startPos_.z + blownValue_, currentFrame_ / maxFrame_);

    cubes_.at(5).worldCoordinate_.position_.x = (float)easeOutExpo(startPos_.x, startPos_.x - blownValue_, currentFrame_ / maxFrame_);

    cubes_.at(6).worldCoordinate_.position_.z = (float)easeOutExpo(startPos_.z, startPos_.z - blownValue_, currentFrame_ / maxFrame_);

    cubes_.at(7).worldCoordinate_.position_.x = (float)easeOutExpo(startPos_.x, startPos_.x + blownValue_, currentFrame_ / maxFrame_);

    for (int i = 0; i < cubes_.size(); ++i) {
        cubes_.at(i).worldCoordinate_.position_.y = (float)easeOutCubic(startPos_.y, startPos_.y + blownValue_, currentFrame_ / maxFrame_);
        if (currentFrame_ < 60) {
            cubes_.at(i).worldCoordinate_.scale_.x = easeOutQuad(0.03f, 0.0f, currentFrame_ / (maxFrame_ / 3));
            cubes_.at(i).worldCoordinate_.scale_.y = easeOutQuad(0.03f, 0.0f, currentFrame_ / (maxFrame_ / 3));
            cubes_.at(i).worldCoordinate_.scale_.z = easeOutQuad(0.03f, 0.0f, currentFrame_ / (maxFrame_ / 3));
        }
        cubes_.at(i).Update();
    }

    if (currentFrame_ < maxFrame_) ++currentFrame_;
}

void Particle::Draw(void)
{
    if (!isActive) {
        return;
    }

    for (int i = 0; i < cubes_.size(); ++i) {
        cubes_.at(i).Draw();
    }
}
