#pragma once
#include "Obj3d.h"
#include "Util.h"
#include "Camera.h"

class Particle
{
private:
    Camera* camera{ nullptr };

    Obj3d cube1{ "Resources/3dModels/cube/cube.obj", camera };
    Obj3d cube2{ "Resources/3dModels/cube/cube.obj", camera };
    Obj3d cube3{ "Resources/3dModels/cube/cube.obj", camera };
    Obj3d cube4{ "Resources/3dModels/cube/cube.obj", camera };
    Obj3d cube5{ "Resources/3dModels/cube/cube.obj", camera };

    Util::Timer timer_{};
};