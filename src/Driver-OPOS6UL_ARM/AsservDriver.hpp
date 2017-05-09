/*!
 * \file
 * \brief Définition de la classe AsservDriver (OPOS6UL).
 */

#ifndef OPOS6UL_ASSERVDRIVER_HPP_
#define OPOS6UL_ASSERVDRIVER_HPP_

#include <as_devices/cpp/as_i2c.hpp>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"

using namespace std;

#define	MBED_ADDRESS    0x05

// convert float to byte array  source: http://mbed.org/forum/helloworld/topic/2053/
union float2bytes_t   // union consists of one variable represented in a number of different ways
{
	float f;
	unsigned char b[sizeof(float)];

	float2bytes_t() :
			b
			{ }
	{
	} //initialisation
};

class AsservDriver: public AAsservDriver, utils::Thread
{


private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(OPOS6UL).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.OPO");
		return instance;
	}
	AsI2c mbedI2c_;
	bool connected_;

	bool asservStarted_;

	TRAJ_STATE pathStatus_;
	Mutex m_;

	int mbed_ack();
	int mbed_readI2c(unsigned char, unsigned char, unsigned char* data);
	int mbed_writeI2c(unsigned char cmd, unsigned char nbBytes2Write,
			unsigned char* data);

	RobotPosition mbed_GetPosition();
	TRAJ_STATE mbed_waitEndOfTraj();
protected:

	virtual void execute();
	RobotPosition p_;
public:

	//commandes directes concernant les moteurs
	void setMotorLeftPosition(int power, long ticks);
	void setMotorRightPosition(int power, long ticks);
	void setMotorLeftPower(int power, int time);
	void setMotorRightPower(int power, int time);
	void stopMotorLeft();
	void stopMotorRight();
	int getMotorLeftCurrent();
	int getMotorRightCurrent();

	//commandes concernant les codeurs
	long getLeftExternalEncoder();
	long getRightExternalEncoder();
	long getLeftInternalEncoder();
	long getRightInternalEncoder();
	void resetEncoders();
	void resetInternalEncoders();
	void resetExternalEncoders();

	//commandes optionnelles de gestion externe
	//deprecated
	//void enableHardRegulation(bool enable);



	//fonctions asservissements externe par defaut
	/*float odo_GetX_mm();
	float odo_GetY_mm();
	float odo_GetTheta_Rad();		// angle in radian
	float odo_GetTheta_Degree();		// angle in degrees*/
	void odo_SetPosition(double x_m, double y_m, double angle_rad);
	RobotPosition odo_GetPosition();
	int path_GetLastCommandStatus();
	void path_InterruptTrajectory();
	void path_CollisionOnTrajectory();
	void path_CollisionRearOnTrajectory();
	void path_CancelTrajectory();
	void path_ResetEmergencyStop();
	TRAJ_STATE motion_DoLine(float dist_meters);
	TRAJ_STATE motion_DoRotate(float angle_deg);
	TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius);
	void motion_FreeMotion(void);
	void motion_DisablePID();		//! just disable PID
	void motion_AssistedHandling(void);		//! Assisted movement mode =)
	void motion_ActivateManager(bool enable);

	TRAJ_STATE motion_DoFace(float x_mm, float y_mm);

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
