#ifndef SRC_BLOCKINGDETECTOR_SPEEDERRORBLOCKINGDETECTOR_H_
#define SRC_BLOCKINGDETECTOR_SPEEDERRORBLOCKINGDETECTOR_H_

#include <stdint.h>

#include "BlockingDetector.h"


class SpeedController;

class SpeedErrorBlockingDetector : BlockingDetector
{
    public:
        explicit SpeedErrorBlockingDetector(float dt,  SpeedController& rightSpeedController, SpeedController& leftSpeedController, float movingIntegralDuration, float movingIntegralErrorThreshold);
        virtual ~SpeedErrorBlockingDetector();

        virtual bool isBlocked();
        virtual void update();

    private:
        float m_dt;
        SpeedController& m_rightSpeedController;
        SpeedController& m_leftSpeedController;
        uint32_t m_nbValues;
        uint32_t m_currentIdx;
        float *m_errorValues;
        float m_movingIntegralError;
        float m_movingIntegralErrorThreshold;
};

#endif /* SRC_BLOCKINGDETECTOR_SPEEDERRORBLOCKINGDETECTOR_H_ */
