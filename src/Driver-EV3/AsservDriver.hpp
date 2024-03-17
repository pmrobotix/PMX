#ifndef EV3_ASSERVDRIVER_HPP_
#define EV3_ASSERVDRIVER_HPP_

#include <cstdint>

#include "../Common/Interface.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "LegoMotor.hpp"
#include "MagEncoders.hpp"

#define MAXVALUE_speed_sp 	1049 //power
#define MAXVALUE_duty_cycle_sp 100 //percentage

using namespace std;
//using namespace ev3dev;

class AsservDriver: public AAsservDriver {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(EV3).
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.EV3");
        return instance;
    }

    LegoMotor _motor_right_;
    LegoMotor _motor_left_;

    MagEncoders mag_;

    int32_t m_encoderLSum;
    int32_t m_encoderRSum;
    float m_encoder1Previous;
    float m_encoder2Previous;

protected:

public:
    /*!
     * \brief Constructor.
     */
    AsservDriver();

    /*!
     * \brief Destructor.
     */
    inline ~AsservDriver() {
    }

    void endWhatTodo();

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
    void getCountsExternal(int32_t* countR, int32_t* countL); //ticks cumulés
    void getDeltaCountsExternal(int32_t* deltaR, int32_t* deltaL); //ticks since last call

    long getLeftInternalEncoder();
    long getRightInternalEncoder();
    void getCountsInternal(int32_t* countR, int32_t* countL); //ticks cumulés

    void resetEncoders();
    void resetInternalEncoders();
    void resetExternalEncoders();

    void stopMotorLeft();
    void stopMotorRight();

    int getMotorLeftCurrent();
    int getMotorRightCurrent();





    //fonctions asservissements externe par defaut
    void odo_SetPosition(float x_mm, float y_mm, float angle_rad);
    ROBOTPOSITION odo_GetPosition();
    int path_GetLastCommandStatus();
    void path_InterruptTrajectory();
    void path_CollisionOnTrajectory();
    void path_CollisionRearOnTrajectory();
    void path_CancelTrajectory();
    void path_ResetEmergencyStop();

    TRAJ_STATE motion_DoFace(float x_mm, float y_mm);
    TRAJ_STATE motion_DoLine(float dist_mm);
    TRAJ_STATE motion_DoRotate(float angle_radians);
    TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius);
    TRAJ_STATE motion_Goto(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoReverse(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoChain(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoReverseChain(float x_mm, float y_mm);

    void motion_FreeMotion();
    void motion_DisablePID();		//! Stop motion control and disable PID
    void motion_AssistedHandling();		//! Assisted movement mode =)
    void motion_ActivateManager(bool enable);
    void motion_setLowSpeedForward(bool enable, int percent);
    void motion_setLowSpeedBackward(bool enable, int percent);

    void motion_ActivateReguDist(bool enable);
    void motion_ActivateReguAngle(bool enable);
    /*
    void motion_ResetReguDist();
    void motion_ResetReguAngle();
    TRAJ_STATE motion_DoDirectLine(float dist_mm); //uniquement en consigne sans le command manager
*/
};

#endif
