#ifndef COMMON_ASSERVDRIVER_AASSERVDRIVER_HPP_
#define COMMON_ASSERVDRIVER_AASSERVDRIVER_HPP_

#include <cstdint>
#include <string>

#include "ARobotPositionShared.hpp"

enum TRAJ_STATE
{
    TRAJ_OK,			//init before trajectory
    TRAJ_FINISHED,                      //trajectory successfully completed
    TRAJ_ERROR,			//unknown error (not implemented !)
    TRAJ_COLLISION,			//trajectory interrupted because of a collision (blocked)
    TRAJ_NEAR_OBSTACLE,		//trajectory interrupted because of a near collision (sensors)
    TRAJ_IMPOSSIBLE,                    //trajectory cancelled by remote user or not found
    TRAJ_INTERRUPTED,		//trajectory interrupted by software
    TRAJ_NEAR_OBSTACLE_REAR, //deprecated ?
    TRAJ_COLLISION_REAR //deprecated ?
};
/*
 enum MOVEMENT_DIRECTION {
 NONE, FORWARD, BACKWARD, TURN
 };
 */
enum MOTION_STATE
{
    TRAJECTORY_RUNNING, ASSISTED_HANDLING, FREE_MOTION, DISABLE_PID,
};
/*
 typedef struct RobotPosition {
 float x; //millimetres
 float y; //millimetres
 float theta; //radians

 //asserv status
 //0 idle
 //1 running
 //2 emergency stop
 //3 blocked/halted
 int asservStatus;

 int queueSize; //deprecated ?
 int l_motor_speed; //deprecated ?
 int r_motor_speed; //deprecated ?
 MOVEMENT_DIRECTION direction; //deprecated ?
 unsigned int debug_nb;

 } RobotPosition;
 */
class AAsservDriver
{
//protected:
//    inline float degToRad(float deg)
//    {
//        return deg * M_PI / 180.0;
//    }
//
//    inline float radToDeg(float rad)
//    {
//        return rad * 180.0 / M_PI;
//    }

public:

    /*!
     * \brief single instance creation.
     */
    static AAsservDriver* create(std::string botid, ARobotPositionShared *aRobotPositionShared);

    /*!
     * \brief Destructor.
     */
    virtual ~AAsservDriver()
    {
    }

    /*!
     * \brief Constructor.
     */
    AAsservDriver()
    {
    }

    /*!
     * \brief actions à faire avant de quitter le programme.
     */
    virtual void endWhatTodo() = 0;

    /*!
     * \brief Fonctions utilisées par la movingBase et permettant un simple control des moteurs et codeurs.
     */
    virtual void setMotorLeftPosition(int power, long ticks) = 0;
    virtual void setMotorRightPosition(int power, long ticks) = 0;
    virtual void setMotorLeftPower(int power, int time_ms) = 0;
    virtual void setMotorRightPower(int power, int time_ms) = 0;
    virtual long getLeftExternalEncoder() = 0;
    virtual long getRightExternalEncoder() = 0;
    virtual void getCountsExternal(int32_t *countR, int32_t *countL) = 0; //ticks cumules
    virtual void getDeltaCountsExternal(int32_t *deltaR, int32_t *deltaL) = 0; //ticks since last call
    virtual long getLeftInternalEncoder() = 0;
    virtual long getRightInternalEncoder() = 0;
    virtual void getCountsInternal(int32_t *countR, int32_t *countL) = 0;
    virtual void resetEncoders() = 0;
    virtual void resetInternalEncoders() = 0;
    virtual void resetExternalEncoders() = 0;
    virtual void stopMotorLeft() = 0;
    virtual void stopMotorRight() = 0;
    virtual int getMotorLeftCurrent() = 0;
    virtual int getMotorRightCurrent() = 0;

    /*!
     * \brief Fonctions permettant d'utiliser un asservissement externe.
     */
    virtual void motion_ActivateManager(bool enablethread) = 0; //true to create thread and false to kill thread
    virtual void odo_SetPosition(float x_mm, float y_mm, float angle_rad) = 0;
    virtual ROBOTPOSITION odo_GetPosition() = 0; //pos in metre/radian

    virtual int path_GetLastCommandStatus() = 0; //Deprecated
    virtual void path_InterruptTrajectory() = 0;
    virtual void path_CollisionOnTrajectory()= 0;
    virtual void path_CollisionRearOnTrajectory()= 0;
    virtual void path_CancelTrajectory()= 0;
    virtual void path_ResetEmergencyStop() = 0;

    virtual TRAJ_STATE motion_DoFace(float x_mm, float y_mm) = 0;
    virtual TRAJ_STATE motion_DoLine(float dist_meters) = 0;
    virtual TRAJ_STATE motion_DoRotate(float angle_radians) = 0;
    virtual TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius) = 0;
    virtual TRAJ_STATE motion_Goto(float x_mm, float y_mm)= 0;
    virtual TRAJ_STATE motion_GotoReverse(float x_mm, float y_mm)= 0;
    virtual TRAJ_STATE motion_GotoChain(float x_mm, float y_mm)= 0;
    virtual TRAJ_STATE motion_GotoReverseChain(float x_mm, float y_mm)= 0;

    virtual void motion_FreeMotion(void) = 0;
    virtual void motion_DisablePID(void) = 0;		//! Stop motion control and disable PID
    virtual void motion_AssistedHandling(void) = 0;		//! Assisted movement mode =) (activate PID)

    virtual void motion_setLowSpeedForward(bool enable, int percent = 0)=0;
    virtual void motion_setLowSpeedBackward(bool enable, int percent = 0)=0;

    //Functions deprecated
    virtual void motion_ActivateReguDist(bool enable) = 0;
    virtual void motion_ActivateReguAngle(bool enable) = 0;
    virtual void motion_ResetReguDist() = 0;
    virtual void motion_ResetReguAngle() = 0;
    virtual TRAJ_STATE motion_DoDirectLine(float dist_mm) = 0; //uniquement en consigne sans le command manager

};

#endif

