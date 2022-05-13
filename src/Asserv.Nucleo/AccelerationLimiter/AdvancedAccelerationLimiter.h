#ifndef SRC_ACCELERATIONLIMITER_ADVANCEDACCELERATIONLIMITER_H_
#define SRC_ACCELERATIONLIMITER_ADVANCEDACCELERATIONLIMITER_H_

#include "AbstractAccelerationLimiter.h"

class AdvancedAccelerationLimiter : public AbstractAccelerationLimiter
{

    /*
     * Advanced acceleration limiter is a bit more complex!
     *
     *   When the robot is accelerating
     *   ( determined by the upper part of the class, ie: AbstractAccelerationLimiter),
     *    the next output speed consign is limited considering the current speed.
     *
     *  As the robot can accelerate more when it's already moving than when it's almost stationary.
     *  So we determine a max acceleration that will be used when the robot speed reach highSpeedThreshold.
     *  Below this speed, a linear interpolation between minAcceleration and maxAcceleration will be use !
     *
     *  This implementation will limit the jerk of the robot.
     */

public:
    explicit AdvancedAccelerationLimiter(float maxAcceleration, float minAcceleration, float highSpeedThreshold);
    virtual ~AdvancedAccelerationLimiter(){};

    void setMaxAcceleration(float maxAcceleration);
    void setMinAcceleration(float minAcceleration);
    void setHighSpeedThreshold(float highSpeedThreshold);

    inline float getMaxAcceleration() const { return m_maxAcceleration; };
    inline float getMinAcceleration() const { return m_minAcceleration; };
    inline float getHighSpeedThreshold() const{ return m_HighSpeedThreshold; };

private:
    virtual float limitOutput(float dt, float targetSpeed, float previousOutput, float currentSpeed);

    float m_maxAcceleration;
    float m_minAcceleration;
    float m_HighSpeedThreshold;
};

#endif /* SRC_ACCELERATIONLIMITER_ADVANCEDACCELERATIONLIMITER_H_ */
