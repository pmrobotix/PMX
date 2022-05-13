#include "AbstractAccelerationLimiter.h"

AbstractAccelerationLimiter::AbstractAccelerationLimiter() : AccelerationDecelerationLimiterInterface()
{
    m_enabled = true;
    m_lastOutput = 0;
}

float AbstractAccelerationLimiter::limitAcceleration(float dt, float targetSpeed, float currentSpeed, float , float )
{
    // First of all, if this limiter is disabled, just return the targeted speed
    if( !m_enabled)
    {
        m_lastOutput = targetSpeed;
        return targetSpeed;
    }

    /* Then determine if we are
     * accelerating or decelerating ...
     */
    bool isAccelerating;
    if( m_lastOutput >= 0 && targetSpeed >= 0)
    {
        if (m_lastOutput <= targetSpeed)
            isAccelerating = true;
        else
            isAccelerating = false;
    }
    else if( m_lastOutput < 0 && targetSpeed < 0)
    {
        if (m_lastOutput <= targetSpeed)
            isAccelerating = false;
        else
            isAccelerating = true;
    }
    else
    {
        // In this case we will change way (ie: going front to going back), consider this as acceleration
        isAccelerating = true;
    }


    /*
     *  ... as the acceleration only is limited !
     */
    if( isAccelerating )
        m_lastOutput += limitOutput(dt, targetSpeed, m_lastOutput, currentSpeed);
    else
        m_lastOutput = targetSpeed;

    return m_lastOutput;
}

float AbstractAccelerationLimiter::constrain(float value, float low, float high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

void AbstractAccelerationLimiter::enable()
{
    m_enabled = true;
}

void AbstractAccelerationLimiter::disable()
{
    m_enabled = false;
}

void AbstractAccelerationLimiter::reset()
{
    m_lastOutput = 0;
}

