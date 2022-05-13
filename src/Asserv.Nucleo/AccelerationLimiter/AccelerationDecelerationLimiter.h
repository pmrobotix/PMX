#ifndef SRC_ACCELERATIONDECELERATIONLIMITER_SIMPLEACCELERATIONLIMITER_H_
#define SRC_ACCELERATIONDECELERATIONLIMITER_SIMPLEACCELERATIONLIMITER_H_

#include "AccelerationDecelerationLimiterInterface.h"

class AccelerationDecelerationLimiter : public AccelerationDecelerationLimiterInterface
{

public:
    explicit AccelerationDecelerationLimiter(float maxAcceleration, float maxDeceleration, float maxSpeed, float positionCorrectorKp);
    explicit AccelerationDecelerationLimiter(float maxAccelerationForward, float maxDecelerationForward, float maxAccelerationBackward, float maxDecelerationBackward, float maxSpeed, float positionCorrectorKp);
    virtual ~AccelerationDecelerationLimiter(){};

    virtual float limitAcceleration(float dt, float targetSpeed, float currentSpeed, float positionGoal, float positionError);

    virtual void enable();
    virtual void disable();
    virtual void reset();

    inline void setDamplingFactor(float value ){m_damplingFactor = value;};

private:

    bool  m_enabled;
    float m_initialPositionError;
    float m_previousPositionGoal;
    float m_maxAccelerationForward;
    float m_maxDecelerationForward;
    float m_maxAccelerationBackward;
    float m_maxDecelerationBackward;
    float m_maxSpeed;
    float m_positionCorrectorKp;
    float m_previousLimitedOutput;
    float m_velocityCompensation;
    float m_CompensatedOutput;
    float m_velocityAtDecTime;
    float m_maxAttainableSpeed;
    float m_timeToVmax;
    float m_timeFromVmaxToZero;

    float m_damplingFactor;
};

#endif /* SRC_ACCELERATIONDECELERATIONLIMITER_SIMPLEACCELERATIONLIMITER_H_ */
