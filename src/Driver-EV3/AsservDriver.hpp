#ifndef EV3_ASSERVDRIVER_HPP_
#define EV3_ASSERVDRIVER_HPP_

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "LegoAngleSensor.hpp"
#include "LegoMotor.hpp"

#define MAXVALUE_speed_sp 	1049 //power
#define MAXVALUE_duty_cycle_sp 100 //percentage

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

    LegoMotor _motor_right_ ;
    LegoMotor _motor_left_ ;

    LegoAngleSensor angleR_;
    LegoAngleSensor angleL_;

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



    void reset(); //motor reset!

    int limit(int power, int max);

    //regulation enabled  => power in ticks per second -860 / +860
    //regulation disabled => power in percentage -100 / +100
    //ticks : relative position to go
    void setMotorLeftPosition(int ticks_per_second, long ticks);
    void setMotorRightPosition(int ticks_per_second, long ticks);

    void setMotorLeftPower(int percent, int time);
    void setMotorRightPower(int percent, int time);

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

    void motion_ActivateReguDist(bool enable);
    void motion_ActivateReguAngle(bool enable);
    void motion_ResetReguDist();
    void motion_ResetReguAngle();
    TRAJ_STATE motion_DoDirectLine(float dist_meters); //uniquement en consigne sans le command manager

};

#endif
