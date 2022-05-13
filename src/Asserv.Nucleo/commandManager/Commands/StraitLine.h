#ifndef STRAITLINE_H_
#define STRAITLINE_H_

#include "Command.h"

class StraitLine : public Command
{
    public:
        explicit StraitLine(float consign, float arrivalDistanceThreshold_mm);
        virtual ~StraitLine() {};

        virtual void computeInitialConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator);
        virtual void updateConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator);
        virtual bool isGoalReached(float X_mm, float Y_mm, float theta_rad, const Regulator &angle_regulator, const Regulator &distance_regulator, const Command* nextCommand);

        virtual bool noStop() const;
    private:
        float m_straitLineConsign;
        float m_arrivalDistanceThreshold_mm;

};

#endif /* STRAITLINE_H_ */
