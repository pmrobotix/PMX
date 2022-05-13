#include "AccelerationDecelerationLimiter.h"

#include <algorithm>
#include <cmath>



AccelerationDecelerationLimiter::AccelerationDecelerationLimiter(
        float maxAcceleration, float maxDeceleration,
        float maxSpeed, float positionCorrectorKp)
    : AccelerationDecelerationLimiter(maxAcceleration, maxDeceleration, maxAcceleration, maxDeceleration, maxSpeed, positionCorrectorKp)
{
}

AccelerationDecelerationLimiter::AccelerationDecelerationLimiter(
        float maxAccelerationForward, float maxDecelerationForward,
        float maxAccelerationBackward, float maxDecelerationBackward,
        float maxSpeed, float positionCorrectorKp): AccelerationDecelerationLimiterInterface()
{
    m_enabled = true;
    m_initialPositionError = 0;
    m_previousPositionGoal = 0;
    m_previousLimitedOutput = 0;
    m_velocityCompensation = 0;
    m_CompensatedOutput = 0;
    m_velocityAtDecTime = maxSpeed;
    m_maxAccelerationForward = maxAccelerationForward;
    m_maxDecelerationForward = maxDecelerationForward;
    m_maxAccelerationBackward = maxAccelerationBackward;
    m_maxDecelerationBackward = maxDecelerationBackward;
    m_maxSpeed = maxSpeed;
    m_positionCorrectorKp = positionCorrectorKp;
    m_maxAttainableSpeed = 0;
    m_damplingFactor = 1.3f;
    m_timeToVmax = 0;
    m_timeFromVmaxToZero = 0;
}

float AccelerationDecelerationLimiter::limitAcceleration(float dt, float targetSpeed, float currentSpeed, float positionGoal, float positionError)
{
    if (!m_enabled)
        return targetSpeed;

    float maxAcceleration = m_maxAccelerationForward;
    float maxDeceleration = m_maxDecelerationForward;
    float way = 1.0f;
    bool forward = true;
    if(positionError < 0.0f)
    {
        maxAcceleration = m_maxAccelerationBackward;
        maxDeceleration = m_maxDecelerationBackward;
        way = -1.0f;
        forward = false;
    }


    if( fabs(m_previousPositionGoal-positionGoal) > 5   )
    {
        // An new position goal in inputed, try to compute the velocity at deceleration time,
        //  using the init speed, the maximum acceleration/deceleration and the error at startup
        //  If the target is far enough to go to the maximum speed, just use the max speed

        m_initialPositionError = positionError;
        m_previousPositionGoal = positionGoal;

        m_timeToVmax = (m_maxSpeed-fabs(currentSpeed))/maxAcceleration;
        m_timeFromVmaxToZero = m_maxSpeed/maxDeceleration;
        m_maxAttainableSpeed = sqrt( (2.0f*maxDeceleration * (fabs(positionError)*2.0f*maxAcceleration + currentSpeed*currentSpeed) ) / (2.0f*maxDeceleration + 2.0f*maxAcceleration)  );

        m_velocityAtDecTime = std::min( m_maxAttainableSpeed , m_maxSpeed );
    }

    m_velocityCompensation = way*((m_positionCorrectorKp*m_damplingFactor)/(2.0*maxDeceleration) - 1.0/m_velocityAtDecTime ) * (m_previousLimitedOutput*m_previousLimitedOutput);

    m_CompensatedOutput = targetSpeed - m_velocityCompensation;


    // Apply a basic "slope filter" to limite maximum acceleration and deceleration
    float max_delta_up = (forward) ? (maxAcceleration*dt) : (maxDeceleration*dt);
    float max_delta_down = (forward) ? (maxDeceleration*dt) : (maxAcceleration*dt);

    if(m_CompensatedOutput > m_previousLimitedOutput+max_delta_up)
        m_CompensatedOutput = m_previousLimitedOutput+max_delta_up;
    if(m_CompensatedOutput < m_previousLimitedOutput-max_delta_down)
        m_CompensatedOutput = m_previousLimitedOutput-max_delta_down;

    // Also limit the maximum speed consign
    if( m_CompensatedOutput > m_maxSpeed)
        m_CompensatedOutput = m_maxSpeed;
    if( m_CompensatedOutput < -m_maxSpeed)
        m_CompensatedOutput = -m_maxSpeed;


//    USBStream::instance()->setDistanceLimiterVelocityAtDecTime(m_velocityAtDecTime);
//    USBStream::instance()->setDistanceLimiterVelocityCompensation(m_velocityCompensation);
//    USBStream::instance()->setDistanceLimiterVelocityCompensated(targetSpeed - m_velocityCompensation);
//    USBStream::instance()->setDistanceLimiterOutput(m_CompensatedOutput);
//    USBStream::instance()->setDistanceLimitercurrentSpeed(currentSpeed);
//    USBStream::instance()->setDistanceLimiterTargetSpeed(targetSpeed);
//    USBStream::instance()->setDistanceLimiterTimeToVMax(m_timeToVmax);
//    USBStream::instance()->setDistanceLimiterMaxAttainableSpeed(m_maxAttainableSpeed);
//    USBStream::instance()->setDistanceLimiterTimeFromVmaxToZero(m_timeFromVmaxToZero);


    m_previousLimitedOutput = m_CompensatedOutput;
    return m_CompensatedOutput;
}


void AccelerationDecelerationLimiter::enable()
{
       m_enabled = true;
}

void AccelerationDecelerationLimiter::disable()
{
    m_enabled = false;
}

void AccelerationDecelerationLimiter::reset()
{
    m_initialPositionError = 0;
    m_previousPositionGoal = 0;
    m_previousLimitedOutput = 0;
    m_velocityAtDecTime = 0;
    m_velocityCompensation = 0;
    m_CompensatedOutput = 0;
}

