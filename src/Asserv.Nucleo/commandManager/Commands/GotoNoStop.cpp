#include "GotoNoStop.h"

#include <cmath>

#include "../../AccelerationLimiter/AccelerationDecelerationLimiter.h"
#include "../../Regulator.h"

GotoNoStop::GotoNoStop(float consignX_mm, float consignY_mm,
        GotoNoStopConfiguration const *configuration,
        Goto::GotoConfiguration const *gotoconfiguration,
        float backwardMode,
        AccelerationDecelerationLimiter *accelerationDecelerationLimiter)
: m_consignX_mm(consignX_mm), m_consignY_mm(consignY_mm),
  m_configuration(configuration), m_gotoConfiguration(gotoconfiguration)
{
    if( backwardMode)
        m_backModeCorrection = -1;
    else
        m_backModeCorrection = 1;
    m_accelerationDecelerationLimiter = accelerationDecelerationLimiter;
}

void GotoNoStop::computeInitialConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator)
{
//    USBStream::instance()->setXGoal(m_consignX_mm);
//    USBStream::instance()->setYGoal(m_consignY_mm);

   float deltaX = m_consignX_mm - X_mm;
   float deltaY = m_consignY_mm - Y_mm;

   float deltaDist = Goto::computeDeltaDist(deltaX, deltaY);
   float deltaTheta = Goto::computeDeltaTheta(m_backModeCorrection * deltaX, m_backModeCorrection * deltaY, theta_rad);

   if(deltaDist < m_configuration->nextDistanceConsign_mm)
   {
       if( m_accelerationDecelerationLimiter )
           m_accelerationDecelerationLimiter->enable();

       /* If the distance is lower than nextDistanceConsign_mm,
        *  We want to go precisely to the goal, use a classic goto algorithm
        */
       float projectedDist = deltaDist * cosf(deltaTheta);
       if (deltaDist < m_gotoConfiguration->gotoReturnThreshold_mm)
       {
           *distanceConsig = distance_regulator.getAccumulator() + m_backModeCorrection * projectedDist ;
       }
       else
       {
         *angleConsign = angle_regulator.getAccumulator() + deltaTheta;

         if (fabs(deltaTheta) < m_gotoConfiguration->gotoAngleThreshold_rad)
             *distanceConsig = distance_regulator.getAccumulator() + m_backModeCorrection * deltaDist;
       }

   }
   else if (fabs(deltaTheta) < m_configuration->gotoAngleThreshold_rad)
   {
       if( m_accelerationDecelerationLimiter )
           m_accelerationDecelerationLimiter->enable();

       /*  Here we are pointing enough to the right direction to go strait to the goal
        *    use a basic goto command
        */
       *angleConsign = angle_regulator.getAccumulator() + deltaTheta;
       *distanceConsig = distance_regulator.getAccumulator() + m_backModeCorrection * deltaDist;
   }
   else if (fabs(deltaTheta) > m_configuration->tooBigAngleThreshold_rad)
   {
       if( m_accelerationDecelerationLimiter )
           m_accelerationDecelerationLimiter->disable();

        /*  Here we are absolutly not pointing to the right direction (the goal is behind the robot for example)
        *   just compute a angle consign.
        *   A distance consign will generate a big overshot to the trajectory
        */
        *angleConsign = angle_regulator.getAccumulator() + deltaTheta;
   }
   else
   {
       if( m_accelerationDecelerationLimiter )
           m_accelerationDecelerationLimiter->disable();

       /*  We aren't pointing enough to the right direction yet.
        *  So turn to the right direction while going at low speed
        */
       float X_goal, Y_goal;
       computeConsignOnCircle(X_mm, Y_mm, m_configuration->lowSpeedDistanceConsign_mm, &X_goal, &Y_goal);
       deltaX = X_goal - X_mm;
       deltaY = Y_goal - Y_mm;

       deltaDist = Goto::computeDeltaDist(deltaX, deltaY);
       deltaTheta = Goto::computeDeltaTheta(m_backModeCorrection * deltaX, m_backModeCorrection * deltaY, theta_rad);
       *angleConsign = angle_regulator.getAccumulator() + deltaTheta;
       *distanceConsig = distance_regulator.getAccumulator() + m_backModeCorrection * deltaDist;


//       USBStream::instance()->setXGoal(X_goal);
//       USBStream::instance()->setYGoal(Y_goal);
   }


}

void GotoNoStop::updateConsign(float X_mm, float Y_mm, float theta_rad, float *distanceConsig, float *angleConsign, const Regulator &angle_regulator, const Regulator &distance_regulator)
{
    return computeInitialConsign(X_mm, Y_mm, theta_rad, distanceConsig, angleConsign, angle_regulator, distance_regulator);
}

bool GotoNoStop::isGoalReached(float X_mm, float Y_mm, float , const Regulator &, const Regulator &, const Command* nextCommand)
{
    float deltaX = m_consignX_mm - X_mm;
    float deltaY = m_consignY_mm - Y_mm;

    float deltaDist = Goto::computeDeltaDist(deltaX, deltaY);
    bool result;

    if( nextCommand != nullptr && nextCommand->noStop())
    {
        result = (deltaDist < m_configuration->nextDistanceConsign_mm);
    }
    else
    {
        result = (deltaDist < m_gotoConfiguration->arrivalDistanceThreshold_mm);
        if( m_accelerationDecelerationLimiter && result )
                m_accelerationDecelerationLimiter->enable();
    }


    return result;
}

bool GotoNoStop::noStop() const
{
    return true;
}

void GotoNoStop::computeConsignOnCircle(float X_mm, float Y_mm, float radius_mm, float *XGoal_mm, float *YGoal_mm)
{
    float angle = M_PI / 2;

    if ((m_consignX_mm - X_mm) != 0) // with an angle of M_PI/2 an divide by zero will occur. So handle this special case.
    {
        // Find a linear function that go from the current position to goal ...
        float slope = (m_consignY_mm - Y_mm) / (m_consignX_mm - X_mm); // offset of the function is useless

        // ... then find the angle between the previous linear function and a linear function y=0x+X_mm (parallel to the x abscissa)
        // see https://fr.wikipedia.org/wiki/Propri%C3%A9t%C3%A9s_m%C3%A9triques_des_droites_et_des_plans#Angles_de_deux_droites
        angle = atanf(fabs(slope));
    }

    // Correct the angle if we are in the left side of the trigonometric circle
    if (X_mm > m_consignX_mm)
        angle = M_PI - angle;
    // Correct the sign of the angle if we are in the ]-Pi;0[ side of the trigonometric circle
    if (Y_mm > m_consignY_mm)
        angle = -angle;

    // Then find a (x,y) point that will be our current goal
    *XGoal_mm = X_mm + cosf(angle) * radius_mm;
    *YGoal_mm = Y_mm + sinf(angle) * radius_mm;
}
