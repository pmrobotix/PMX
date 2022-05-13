#include "Turn.h"

#include <cmath>

#include "../../Regulator.h"

Turn::Turn(float consign_rad, float arrivalAngleThreshold_rad)
 : m_angleConsign(consign_rad), m_arrivalAngleThreshold_rad(arrivalAngleThreshold_rad)
{
}

void Turn::computeInitialConsign(float , float , float , float *, float *angleConsign, const Regulator &, const Regulator &)
{
    *angleConsign += m_angleConsign;
}

void Turn::updateConsign(float , float , float , float *, float *, const Regulator &, const Regulator &)
{
}

bool Turn::isGoalReached(float , float , float , const Regulator &angle_regulator, const Regulator &, const Command* )
{
    return fabs(angle_regulator.getError()) <= m_arrivalAngleThreshold_rad;
}

bool Turn::noStop() const
{
    return false;
}
