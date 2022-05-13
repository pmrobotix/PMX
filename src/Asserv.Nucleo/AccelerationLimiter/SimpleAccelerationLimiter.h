#ifndef SRC_ACCELERATIONLIMITER_SIMPLEACCELERATIONLIMITER_H_
#define SRC_ACCELERATIONLIMITER_SIMPLEACCELERATIONLIMITER_H_

#include "AbstractAccelerationLimiter.h"

class SimpleAccelerationLimiter : public AbstractAccelerationLimiter
{

    /*
     * Simple acceleration limiter is ... Simple !
     *
     *   When the robot is accelerating
     *   ( determined by the upper part of the class, ie: AbstractAccelerationLimiter),
     *    the next output speed consign is limited by m_maxAcceleration.
     *
     *  Output speed will be a trapezoidal shape velocity
     */

public:
    explicit SimpleAccelerationLimiter(float maxAcceleration);
    virtual ~SimpleAccelerationLimiter(){};

    void setMaxAcceleration(float maxAcceleration);
    inline float getMaxAcceleration() const { return m_maxAcceleration; };

private:
    virtual float limitOutput(float dt, float targetSpeed, float previousOutput, float currentSpeed);

    float m_maxAcceleration;
};

#endif /* SRC_ACCELERATIONLIMITER_SIMPLEACCELERATIONLIMITER_H_ */
