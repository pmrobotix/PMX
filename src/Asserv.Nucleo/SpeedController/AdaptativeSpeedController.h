#ifndef SRC_SPEEDCONTROLLER_ADAPTATIVESPEEDCONTROLLER_H_
#define SRC_SPEEDCONTROLLER_ADAPTATIVESPEEDCONTROLLER_H_

#include "SpeedController.h"

constexpr uint8_t NB_PI_SUBSET = 3;

class AdaptativeSpeedController : public SpeedController
{
    public:
        explicit AdaptativeSpeedController(
                float KpGains[NB_PI_SUBSET], float KiGains[NB_PI_SUBSET], float speedRange[NB_PI_SUBSET],
                float outputLimit, float maxInputSpeed, float measureFrequency);
        virtual ~AdaptativeSpeedController() {};

        virtual float update(float actualSpeed);

        virtual void setGains(float Kp, float Ki);
        void setGains(float Kp, float Ki, uint8_t range);

        void getGainsForRange(uint8_t range, float *Kp, float *Ki, float *speedRange);

    private:
        void  updateGains(float actualSpeed);

        float m_speedKpSet[NB_PI_SUBSET];
        float m_speedKiSet[NB_PI_SUBSET];
        float m_GainsSpeedRange[NB_PI_SUBSET];

};

#endif /* SRC_SPEEDCONTROLLER_ADAPTATIVESPEEDCONTROLLER_H_ */
