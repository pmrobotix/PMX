#ifndef _AROBOTPOSSHARED_HPP_
#define _AROBOTPOSSHARED_HPP_

#include <cmath>

#include "../../Log/Logger.hpp"

enum MOVEMENT_DIRECTION
{
    NONE, FORWARD, BACKWARD, TURN
};

struct sRobotPosition
{
    float x; //millimetres
    float y; //millimetres
    float theta; //radians

    //asserv status
    //0 idle
    //1 running
    //2 emergency stop
    //3 blocked/halted
    int asservStatus;

    //int queueSize; //deprecated ?
    //int l_motor_speed; //deprecated ?
    //int r_motor_speed; //deprecated ?
    //MOVEMENT_DIRECTION direction; //deprecated ?
    unsigned int debug_nb;

};
typedef struct sRobotPosition ROBOTPOSITION;

inline float degToRad(float deg)
{
    return deg * M_PI / 180.0;
}

inline float radToDeg(float rad)
{
    return rad * 180.0 / M_PI;
}

inline bool cmpf(float A, float B, float epsilon = 0.005f)
{
    return (fabs(A - B) < epsilon);
}

class ARobotPositionShared
{

public:

    /*!
     * \brief instance creation.
     */
    static ARobotPositionShared* create();

    virtual ROBOTPOSITION getRobotPosition() = 0;

    virtual void setRobotPosition(ROBOTPOSITION p) = 0;

    ROBOTPOSITION convertPositionBeaconToRepereTable(float d_mm, float x_mm, float y_mm, float theta_deg,
            float *x_botpos, float *y_botpos)
    {
        ROBOTPOSITION p = getRobotPosition();

        //coordonnées de l'objet detecté sur la table
        float a = (p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f));
        std::fmod(a, 2 * M_PI);

        if (a < 0)
            a += 2 * M_PI;
//            if (a < -M_PI)
//                a += M_PI;
//            if (a > M_PI)
//                a -= M_PI;

        //ADV coord
        float fx_botpos = p.x + (d_mm * cos(a));
        float fy_botpos = p.y + (d_mm * sin(a));

        *x_botpos = fx_botpos;
        *y_botpos = fy_botpos;
//        logger().debug() << "DEBUG --xy_botpos= " << *x_botpos << " " << *y_botpos
//                        << " pos: " << p.x << " " << p.y << " p_deg:" << p.theta  * 180.0f / M_PI<< " --balise: " << d_mm << " " << x_mm << " "
//                        << y_mm << " t_deg:" << theta_deg << logs::end;

        return p;
    }

    /*!
     * \brief Destructor.
     */
    virtual ~ARobotPositionShared()
    {
    }

protected:

    /*!
     * \brief Constructor.
     */
    ARobotPositionShared()
    {
    }

};

#endif
