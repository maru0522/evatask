#pragma once
#include "Obj3d.h"
#include "Util.h"
#include "Camera.h"
#include "easing.h"
#include "RailCamera.h"
#include <DirectXMath.h>
#include <array>

class Particle
{
private:
    static constexpr float_t blownValue_{ 0.5f };
    static constexpr int32_t maxFrame_{ 180 };

public:
    void Activate(RailCamera* pCamera, DirectX::XMFLOAT3 startPos);
    void Update(void);
    void Draw(void);

private:
    bool isActive{ false };

    std::array<Obj3d, 8> cubes_;
    //std::array<float_t, 8> randoms_;

    DirectX::XMFLOAT3 startPos_{};

    float_t currentFrame_{ 0 };
};