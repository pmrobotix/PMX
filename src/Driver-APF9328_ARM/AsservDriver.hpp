#ifndef ARM_ASSERVDRIVER_HPP_
#define ARM_ASSERVDRIVER_HPP_

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ExtEncoder.hpp"
#include "Md25.hpp"

using namespace std;

class AsservDriver: public AAsservDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(ARM).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.ARM");
		return instance;
	}

	Md25 md25_;

	ExtEncoder extRightEncoder_;
	ExtEncoder extLeftEncoder_;

public:

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
	void enableHardRegulation(bool enable);

	//fonctions asservissement externe si necessaire. ici c'est asservinsa() qui est utilisé.
	float odo_GetX_mm();
	float odo_GetY_mm();
	float odo_GetTheta_Rad();		// angle in radian
	float odo_GetTheta_Degree();		// angle in degrees
	void odo_SetPosition(float x_m, float y_m, float angle_rad);
	RobotPosition odo_GetPosition();
	int path_GetLastCommandStatus();
	void path_InterruptTrajectory();
	void path_CollisionOnTrajectory();
	void path_CollisionRearOnTrajectory();
	void path_CancelTrajectory();
	void path_ResetEmergencyStop();
	TRAJ_STATE motion_DoLine(float dist_meters);
	TRAJ_STATE motion_DoRotate(float angle_radians);
	TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius);
	void motion_FreeMotion(void);
	void motion_DisablePID(void);		//! Stop motion control and disable PID
	void motion_AssistedHandling(void);		//! Assisted movement mode =)
	void motion_StopManager(void);



	/*!
	 * \brief Constructor.
	 */
	AsservDriver();

	/*!
	 * \brief Destructor.
	 */
	~AsservDriver();

};

#endif
