#include "Goto.h"

#include <cmath>

#include "../../Regulator.h"

Goto::Goto(float consignX_mm, float consignY_mm,
        GotoConfiguration const *configuration,
        float backwardMode)
: m_consignX_mm(consignX_mm), m_consignY_mm(consignY_mm),
  m_configuration(configuration), m_alignOnly(false)
{
    if( backwardMode)
        m_backModeCorrection = -1;
    else
        m_backModeCorrection = 1;
}

void Goto::computeInitialConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator)
{
   float deltaX = m_consignX_mm - X_mm;
   float deltaY = m_consignY_mm - Y_mm;

   // Valeur absolue de la distance à parcourir en allant tout droit pour atteindre la consigne
   float deltaDist = computeDeltaDist(deltaX, deltaY);

   // Si on veut aller au prochain, mais qu'il est trop proche, on essaye d'abord de s'alligner ! 
   if (deltaDist < m_configuration->gotoReturnThreshold_mm)
   {
       m_alignOnly = true;
   }

    updateConsign( X_mm, Y_mm, theta_rad, distanceConsig, angleConsign, angle_regulator, distance_regulator);
}

void Goto::updateConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator)
{
   float deltaX = m_consignX_mm - X_mm;
   float deltaY = m_consignY_mm - Y_mm;

   // Valeur absolue de la distance à parcourir en allant tout droit pour atteindre la consigne
   float deltaDist = computeDeltaDist(deltaX, deltaY);

   // La différence entre le thetaCible (= cap à atteindre) et le theta (= cap actuel du robot) donne l'angle à parcourir
   float deltaTheta = computeDeltaTheta(m_backModeCorrection*deltaX, m_backModeCorrection*deltaY, theta_rad);

   //  Si la distance est faible, on ne s'asservit plus qu'en distance en partant du principe qu'on est assez alligné.
   if (deltaDist < m_configuration->gotoReturnThreshold_mm && !m_alignOnly)
   {
       float projectedDist = deltaDist * cosf(deltaTheta);
       *distanceConsig = distance_regulator.getAccumulator() + m_backModeCorrection*projectedDist ;
   }
   else
   {
       *angleConsign = angle_regulator.getAccumulator() + deltaTheta;

       if (fabs(deltaTheta) < m_configuration->gotoAngleThreshold_rad)
       {
           *distanceConsig = distance_regulator.getAccumulator() + m_backModeCorrection*deltaDist;
           m_alignOnly = false;
       }
   }

//   USBStream::instance()->setXGoal(m_consignX_mm);
//   USBStream::instance()->setYGoal(m_consignY_mm);
}

bool Goto::isGoalReached(float X_mm, float Y_mm, float , const Regulator &, const Regulator &, const Command* )
{
    float deltaX = m_consignX_mm - X_mm;
    float deltaY = m_consignY_mm - Y_mm;

    return computeDeltaDist(deltaX, deltaY) < m_configuration->arrivalDistanceThreshold_mm;
}

bool Goto::noStop() const
{
    return true;
}

float Goto::computeDeltaDist(float deltaX, float deltaY)
{
    // On a besoin de min et max pour le calcul de la racine carrée
    float max = fabs(deltaX) > fabs(deltaY) ? fabs(deltaX) : fabs(deltaY);
    float min = fabs(deltaX) <= fabs(deltaY) ? fabs(deltaX) : fabs(deltaY);

    // Valeur absolue de la distance à parcourir en allant tout droit pour atteindre la consigne
    if (max != 0)
        return (max * sqrtf(1.0 + (min / max) * (min / max)));
    else
        return 0;
}


float Goto::computeDeltaTheta(float deltaX, float deltaY, float theta_rad)
{
    // Cap que doit atteindre le robot
    float thetaCible = atan2f(deltaY, deltaX);

    // La différence entre le thetaCible (= cap à atteindre) et le theta (= cap actuel du robot) donne l'angle à parcourir
    float deltaTheta = thetaCible - theta_rad;

    // On ajuste l'angle à parcourir pour ne pas faire plus d'un demi-tour
    // Exemple, tourner de 340 degrés est plus chiant que de tourner de -20 degrés
    if (deltaTheta > M_PI)
    {
        deltaTheta -= 2.0 * M_PI;
    }
    else if (deltaTheta < -M_PI)
    {
        deltaTheta += 2.0 * M_PI;
    }

    return deltaTheta;
}

