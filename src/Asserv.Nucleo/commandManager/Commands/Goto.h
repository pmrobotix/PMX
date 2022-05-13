#ifndef GOTO_H_
#define GOTO_H_

#include "Command.h"

class Goto : public Command
{
    public:

        struct GotoConfiguration
        {
            float gotoReturnThreshold_mm;
            float gotoAngleThreshold_rad;
            float arrivalDistanceThreshold_mm;
        };

        explicit Goto(float consignX_mm, float consignY_mm,
                GotoConfiguration const *configuration,
                float backwardMode = false);

        virtual ~Goto() {};

        virtual void computeInitialConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator);
        virtual void updateConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator);
        virtual bool isGoalReached(float X_mm, float Y_mm, float theta_rad, const Regulator &angle_regulator, const Regulator &distance_regulator, const Command* nextCommand);

        virtual bool noStop() const;

        static float computeDeltaDist(float deltaX, float deltaY);
        static float computeDeltaTheta(float deltaX, float deltaY, float theta_rad);
    private:
        float m_consignX_mm;
        float m_consignY_mm;

        GotoConfiguration const *m_configuration;

        float m_backModeCorrection;

        bool m_alignOnly;
};

#endif /* GOTO_H_ */
