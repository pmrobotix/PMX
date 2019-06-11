#ifndef ASSERV_ESIALR_ASSERVESIALR_H_
#define ASSERV_ESIALR_ASSERVESIALR_H_

#include <sys/types.h>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"

class CodeursInterface;
class CommandManager;
class ConsignController;
class MotorsController;
class Odometrie;

class Robot;
/*
 #define uint8 unsigned char
 #define uint16 unsigned short
 #define uint32 unsigned int
 #define int8 char
 #define int16 short
 #define int32 int
 //#define BOOL bool*/

class AsservEsialR: public AAsservDriver, public utils::Thread
{

protected:
    virtual void execute();

private:

    /*!
     *\brief Chronomètre lié.
     */
    utils::Chronometer chronoTimer_;

    /*!
     *\brief ref vers le robot lié.
     */
    Robot * robot_;

    bool run_ = false;

    //nb of period since the beginning
    uint periodNb_; //static

    //loop delay
    uint loopDelayInMillisec_;

    CodeursInterface *codeurs_;
    Odometrie *odo_;
    MotorsController *motorC_;
    ConsignController *consignC_;
    CommandManager *commandM_;

    TRAJ_STATE pathStatus_;
    RobotPosition p_;

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref AsservEsialR.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("AsservEsialR");
        return instance;
    }

    /*!
     * \brief Retourne le \ref Logger file associé à la classe \ref AsservEsialR.
     */
    static inline const logs::Logger & loggerFile()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("logFileAsservEsialR");
        return instance;
    }

public:
    AsservEsialR(Robot * robot);
    ~AsservEsialR()
    {
    }

    void setSamplingFrequency(uint frequency);
    void loadConfig();
    void startAsserv(int freqHz);
    void initAsserv();
    void stopAsserv();
    void resetAsserv();
    TRAJ_STATE waitEndOfTraj();

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

    //fonctions asservissements externe par defaut
    void odo_SetPosition(float x_m, float y_m, float angle_rad);
    RobotPosition odo_GetPosition();

    int path_GetLastCommandStatus();
    void path_InterruptTrajectory();
    void path_CollisionOnTrajectory();
    void path_CollisionRearOnTrajectory();
    void path_CancelTrajectory();
    void path_ResetEmergencyStop();
    TRAJ_STATE motion_DoLine(float dist_meters);
    TRAJ_STATE motion_DoFace(float x_m, float y_m);
    TRAJ_STATE motion_DoRotate(float angle_radians);
    TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius);
    void motion_FreeMotion(void);
    void motion_DisablePID();           //! just disable PID
    void motion_AssistedHandling(void);           //! Assisted movement mode =)
    void motion_ActivateManager(bool enable);
    void motion_setLowSpeedForward(bool enable, int percent = 0);
    void motion_setLowSpeedBackward(bool enable, int percent = 0);

    void motion_ActivateReguDist(bool enable);
    void motion_ActivateReguAngle(bool enable);
    void motion_ResetReguDist();
    void motion_ResetReguAngle();
    TRAJ_STATE motion_DoDirectLine(float dist_meters);

    void motion_ActivateQuadRamp(bool enable);

};

#endif /* ASSERV_ESIALR_ASSERVESIALR_H_ */
