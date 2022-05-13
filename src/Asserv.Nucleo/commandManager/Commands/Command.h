#ifndef SRC_COMMAND_H_
#define SRC_COMMAND_H_
class Regulator;

class Command
{
public:
    virtual ~Command() {}

    virtual void computeInitialConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator) = 0;
    virtual void updateConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator) = 0;
    virtual bool isGoalReached(float X_mm, float Y_mm, float theta_rad, const Regulator &angle_regulator, const Regulator &distance_regulator, const Command* nextCommand) = 0;

    virtual bool noStop() const = 0;
};

#endif /* SRC_COMMAND_H_ */
