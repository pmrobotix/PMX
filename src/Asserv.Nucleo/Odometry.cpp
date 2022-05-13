#include "Odometry.h"

#include <math.h>

#include "util/asservMath.h"

Odometry::Odometry(float encoderWheelsDistance_mm, float initialX, float initialY)
{
    m_encoderWheelsDistance_mm = encoderWheelsDistance_mm;

    // Initialisation de la position
    m_X_mm = initialX;
    m_Y_mm = initialY;
    m_theta_rad = 0;
    m_deltaDist = 0;
    m_deltaTheta = 0;
}

void Odometry::resetX(float X)
{
    m_X_mm = X;
}

void Odometry::resetY(float Y)
{
    m_Y_mm = Y;
}

void Odometry::resetTheta()
{
    m_theta_rad = 0;
}

void Odometry::reset()
{
    resetX(0);
    resetY(0);
    resetTheta();
}

void Odometry::refresh(float m_encoderDeltaRight_mm, float m_encoderDeltaLeft_mm)
{
    /*
     * deltaDist = la distance parcourue par le robot pendant l'itération = moyenne des distances des codeurs
     * deltaTheta = la variation de l'angle pendant l'itération = rapport de la différence des distances codeurs sur la
     *               distance entre les roues
     */
    m_deltaDist = (m_encoderDeltaLeft_mm + m_encoderDeltaRight_mm) / 2;
    float diffCount = m_encoderDeltaRight_mm - m_encoderDeltaLeft_mm;
    m_deltaTheta = double(diffCount) / double(m_encoderWheelsDistance_mm); // En radian

    if (diffCount == 0)    // On considère le mouvement comme une ligne droite
            {
        // Mise à jour de la position
        m_X_mm += m_deltaDist * cos(m_theta_rad);
        m_Y_mm += m_deltaDist * sin(m_theta_rad);
    } else {
        //On approxime en considérant que le robot suit un arc de cercle
        // On calcule le rayon de courbure du cercle
        float R = float(m_deltaDist) / m_deltaTheta;

        //Mise à jour de la position
        m_X_mm += R * (-sin(m_theta_rad) + sin(m_theta_rad + m_deltaTheta));
        m_Y_mm += R * (cos(m_theta_rad) - cos(m_theta_rad + m_deltaTheta));
        // Mise à jour du cap
        m_theta_rad += m_deltaTheta;

        // On limite le cap à +/- PI afin de ne pouvoir tourner dans les deux sens et pas dans un seul
        if (m_theta_rad > M_PI)
            m_theta_rad -= 2 * M_PI;
        else if (m_theta_rad <= -M_PI)
            m_theta_rad += 2 * M_PI;
    }
}

void Odometry::setPosition(float X_mm, float Y_mm, float theta_rad)
{
    m_X_mm = X_mm;
    m_Y_mm = Y_mm;
    m_theta_rad = theta_rad;
}
