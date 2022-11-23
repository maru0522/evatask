#pragma once
#include<DirectXMath.h>

using namespace DirectX;

XMFLOAT3 easeOutSineVec3(XMFLOAT3 startpos, XMFLOAT3 endpos, float time);

XMFLOAT3 easeOutQuintVec3(XMFLOAT3 start, XMFLOAT3 end, double time);
double lerp(double a, double b, double t);

double easeInSine(double start, double end, double time);

double easeOutSine(double start, double end, double time);

double easeInOutSine(double start, double end, double time);

double easeOutQuad(double start, double end, double time);

double easeInQuint(double start, double end, double time);

double easeOutQuint(double start, double end, double time);

double easeOutExpo(double start, double end, double time);

double easeInOutQuint(double start, double end, double time);

double easeInBack(double start, double end, double time);

double easeOutBack(double start, double end, double time);

double easeOutElastic(double start, double end, double time);

double easeOutCubic(double start, double end, double time);