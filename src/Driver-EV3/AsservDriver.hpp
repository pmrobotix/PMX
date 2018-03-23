#ifndef EV3_ASSERVDRIVER_HPP_
#define EV3_ASSERVDRIVER_HPP_

#include <sys/types.h>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ev3dev.h"

#define MAXVALUE_speed_sp 	860 //power
#define MAXVALUE_duty_cycle_sp 		100 //percentage

using namespace std;
using namespace ev3dev;



class AsservDriver: public AAsservDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.EV3");
		return instance;
	}

	motor _motor_left = { OUTPUT_A };
	motor _motor_right = { OUTPUT_D };

	large_motor     _motor_left_;
	large_motor     _motor_right_;

	motor arrMotors[4] = {
	    { OUTPUT_A },
	    { OUTPUT_B },
	    { OUTPUT_C },
	    { OUTPUT_D }
	  };


protected:

public:
	/*!
	 * \brief Constructor.
	 */
	AsservDriver();

	/*!
	 * \brief Destructor.
	 */
	inline ~AsservDriver()
	{
	}


	void reset();
	int limit(int power, int max);

	//regulation enabled  => power in ticks per second -860 / +860
	//regulation disabled => power in percentage -100 / +100
	//ticks : relative position to go
	void setMotorLeftPosition(int power, long ticks);
	void setMotorRightPosition(int power, long ticks);


	void setMotorLeftPower(int power, int time);
	void setMotorRightPower(int power, int time);

	long getLeftExternalEncoder();
	long getRightExternalEncoder();

	long getLeftInternalEncoder();
	long getRightInternalEncoder();

	void resetEncoders();
	void resetInternalEncoders();
	void resetExternalEncoders();

	void stopMotorLeft();
	void stopMotorRight();

	int getMotorLeftCurrent();
	int getMotorRightCurrent();



	//fonctions asservissements externe par defaut
	void odo_SetPosition(double x_m, double y_m, double angle_rad);
	RobotPosition odo_GetPosition();
	int path_GetLastCommandStatus();
	void path_InterruptTrajectory();
	void path_CollisionOnTrajectory();
	void path_CollisionRearOnTrajectory();
	void path_CancelTrajectory();
	void path_ResetEmergencyStop();

	TRAJ_STATE motion_DoFace(float x_m, float y_m);
	TRAJ_STATE motion_DoLine(float dist_meters);
	TRAJ_STATE motion_DoRotate(float angle_radians);
	TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius);
	void motion_FreeMotion();
	void motion_DisablePID();		//! Stop motion control and disable PID
	void motion_AssistedHandling();		//! Assisted movement mode =)
	void motion_ActivateManager(bool enable);
	void motion_setLowSpeed(bool enable);

};

#endif
