#ifndef SRC_ACCELERATIONLIMITER_H_
#define SRC_ACCELERATIONLIMITER_H_

class AccelerationDecelerationLimiterInterface
{
public:
    virtual ~AccelerationDecelerationLimiterInterface()
    {
    }

    virtual float limitAcceleration(float dt, float targetSpeed, float currentSpeed, float positionGoal, float positionError) = 0;

    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void reset() = 0;
};

#endif /* SRC_ACCELERATIONLIMITER_H_ */
