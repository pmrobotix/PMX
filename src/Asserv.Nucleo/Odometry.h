#ifndef ODOMETRIE
#define ODOMETRIE

#include <cstdint>

class Odometry
{
public:
    explicit Odometry(float encoderWheelsDistance_mm, float initialX = 0, float initialY = 0);
    ~Odometry()
    {
    }
    ;

    // Reset de la position du robot
    void resetX(float xval);
    void resetY(float yval);
    void resetTheta();

    void reset();

    void refresh(float m_encoderDeltaRight_mm, float m_encoderDeltaLeft_mm);

    float getX() const
    {
        return m_X_mm;   // Renvoie la position en X par rapport au point de départ
    }
    float getY() const
    {
        return m_Y_mm;   // Renvoie la position en Y par rapport au point de départ
    }
    float getTheta() const
    {
        return m_theta_rad;   // Renvoie l'angle par rapport au cap de départ
    }

    void setPosition(float X_mm, float Y_mm, float theta_rad);

    void setEncoderWheelsDistance(float encoderWheelsDistance_mm)
    {
        m_encoderWheelsDistance_mm =  encoderWheelsDistance_mm;
    }

    float getDeltaTheta() const
    {
        return m_deltaTheta;
    }
    float getDeltaDist() const
    {
        return m_deltaDist;
    }

private:

    float m_encoderWheelsDistance_mm;

    double m_deltaTheta;
    float m_deltaDist;

    // Position actuelle
    float m_X_mm, m_Y_mm; // En mm
    float m_theta_rad; //En radian
};

#endif
