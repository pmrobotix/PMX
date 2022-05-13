#include "AdvancedAccelerationLimiter.h"
#include <cmath>


AdvancedAccelerationLimiter::AdvancedAccelerationLimiter(float maxAcceleration, float minAcceleration, float highSpeedThreshold) :
AbstractAccelerationLimiter()
{
    m_maxAcceleration = maxAcceleration;
    m_minAcceleration = minAcceleration;
    m_HighSpeedThreshold = highSpeedThreshold;
}

float AdvancedAccelerationLimiter::limitOutput(float dt, float targetSpeed, float previousOutput, float currentSpeed)
{
    float change = targetSpeed - previousOutput;
    float maxDelta;

    if( fabs(currentSpeed) >= m_HighSpeedThreshold)
       maxDelta = dt * m_maxAcceleration;
   else
       maxDelta = dt * (m_minAcceleration + fabs(currentSpeed)*(m_maxAcceleration-m_minAcceleration)/m_HighSpeedThreshold);

    return constrain(change, -maxDelta, maxDelta);
}

void AdvancedAccelerationLimiter::setMaxAcceleration(float maxAcceleration)
{
    m_maxAcceleration = maxAcceleration;
}

void AdvancedAccelerationLimiter::setMinAcceleration(float minAcceleration)
{
    m_minAcceleration = minAcceleration;
}

void AdvancedAccelerationLimiter::setHighSpeedThreshold(float highSpeedThreshold)
{
    m_HighSpeedThreshold = highSpeedThreshold;
}
