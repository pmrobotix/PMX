#ifndef _AROBOTPOSSHARED_HPP_
#define _AROBOTPOSSHARED_HPP_

#include <cmath>

#include "../Utils/Chronometer.hpp"

enum MOVEMENT_DIRECTION {
	NONE, FORWARD, BACKWARD, TURN
};

struct sRobotPosition {
	float x; //millimetres
	float y; //millimetres
	float theta; //radians

	//asserv status
	//0 idle
	//1 running
	//2 emergency stop / halted
	//3 blocked
	int asservStatus;

	unsigned int queueSize;
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

inline float WrapAngle2PI(float rad)
{
	//// force it to be the positive remainder, so that 0 <= angle < 360
	//    degrees = (((int) (degrees * 1000.0f) + 360000) % 360000) / 1000.0f;
	////reduction sur une plage de [0 à 360]
	//    if (degrees >= 360.0) {
	//        degrees = ((int) (degrees * 1000.0f) % 360000) / 1000.0f;
	//    }
	//    if (degrees < -360.0) {
	//        int d = (int) -(degrees * 1000.0f);
	//        d = d % 360000;
	//        degrees = -d / 1000.0f;
	//    }
	// force into the minimum absolute value residue class, so that -180 < angle <= 180
	//    if (degrees >= 180)
	//        degrees -= 360;

	rad = std::fmod(rad, 2.0 * M_PI);
	if (rad < -M_PI) rad += (2.0 * M_PI);
	if (rad > M_PI) rad -= (2.0 * M_PI);
	return rad;
}

class ARobotPositionShared {

public:

	utils::Chronometer chrono_;

	/*!
	 * \brief instance creation.
	 */
	static ARobotPositionShared* create();

	virtual ROBOTPOSITION getRobotPosition(int debug = 0) = 0;

	virtual void setRobotPosition(ROBOTPOSITION p) = 0;

	ROBOTPOSITION convertPositionBeaconToRepereTable(float d_mm, float x_mm, float y_mm, float theta_deg,
			float *x_botpos, float *y_botpos)
	{
		ROBOTPOSITION p = getRobotPosition(0);

		//coordonnées de l'objet detecté sur la table
		float a = (p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f));
//        std::fmod(a, 2 * M_PI);
//
//        if (a < 0)
//            a += 2 * M_PI;
////            if (a < -M_PI)
////                a += M_PI;
////            if (a > M_PI)
////                a -= M_PI;

		a= WrapAngle2PI(a);

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
	ARobotPositionShared() :
			chrono_("ARobotPositionShared")
	{
	}

};

#endif
