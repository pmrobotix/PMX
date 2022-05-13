#ifndef GOTONOSTOP_H_
#define GOTONOSTOP_H_

#include "Command.h"
#include "Goto.h"

class AccelerationDecelerationLimiter;

class GotoNoStop : public Command
{
    public:

        struct GotoNoStopConfiguration
        {
            float gotoAngleThreshold_rad;
            float tooBigAngleThreshold_rad;
            float lowSpeedDistanceConsign_mm;
            float nextDistanceConsign_mm;
        };


        explicit GotoNoStop(float consignX_mm, float consignY_mm,
                GotoNoStopConfiguration const *noStopConfiguration,
                Goto::GotoConfiguration const *gotoconfiguration,
                float backwardMode = false,
                AccelerationDecelerationLimiter *accelerationDecelerationLimiter = nullptr);

        virtual ~GotoNoStop() {};

        virtual void computeInitialConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator);
        virtual void updateConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator);
        virtual bool isGoalReached(float X_mm, float Y_mm, float theta_rad, const Regulator &angle_regulator, const Regulator &distance_regulator, const Command* nextCommand);

        virtual bool noStop() const;
    private:

        void computeConsignOnCircle(float X_mm, float Y_mm, float dist_mm, float *XGoal_mm, float *YGoal_mm);

        float m_consignX_mm;
        float m_consignY_mm;

        float m_backModeCorrection;

        AccelerationDecelerationLimiter *m_accelerationDecelerationLimiter;

        GotoNoStopConfiguration const *m_configuration;
        Goto::GotoConfiguration const *m_gotoConfiguration;
};

#endif /* Goto_H_ */
