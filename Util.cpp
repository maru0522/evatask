#include "Util.h"
#include <chrono>
#include <cmath>

using namespace std::chrono;

template<class TimeUnit>
inline int32_t Util::Timer::GetNowCount(void)
{
    // template�̌^��[���ԊԊu����\���^]�łȂ���΂Ȃ�Ȃ��B
    // ref: https://cpprefjp.github.io/reference/chrono/duration_aliases.html

    return static_cast<int32_t>(duration_cast<TimeUnit>(steady_clock::now().time_since_epoch()).count());
}

void Util::Timer::Start(float_t endTime)
{
    startTime_ = GetNowCount<milliseconds>();
}

const bool Util::Timer::GetIsEnd(void)
{
    float_t elapsedTime = (GetNowCount<milliseconds>() - startTime_) / 1000.0f;
    return endTime_ < elapsedTime;
}

constexpr float Util::Convert::DegsToRads(float fDegrees)
{
    return fDegrees * MY_PI / 180.0f;
}

constexpr float Util::Convert::RadsToDegs(float fRadians)
{
    return fRadians * 180.0f / MY_PI;
}
