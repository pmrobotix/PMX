#ifndef ASSERV_E_ASSERVESIALR_H_
#define ASSERV_E_ASSERVESIALR_H_

#include <sys/types.h>

#include "../Common/Interface.Driver/AAsservDriver.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"
//#include "../Common/Utils/ITimerPosixListener.hpp"

class CodeursInterface;
class CommandManagerA;
class ConsignController;
class MotorsController;
class Odometrie;

class Robot;
/*//TODO replace all uint / be careful with AsservInsa which already define them
 #define uint8 unsigned char
 #define uint16 unsigned short
 #define uint32 unsigned int
 #define int8 char
 #define int16 short
 #define int32 int
 //#define BOOL bool*/

class AsservEsialR: public AAsservDriver, public utils::Thread //, public ITimerPosixListener
{

protected:
     void execute();
//     void onTimer(utils::Chronometer chrono);
//     void onTimerEnd(utils::Chronometer chrono);

private:
     bool loop_finished_;
     unsigned long long last_;

    /*!
     *\brief Chronomètre lié.
     */
    utils::Chronometer chronoTimer_;

    /*!
     *\brief ref vers le robot lié.
     */
    Robot * robot_;

    AAsservDriver* asservdriver;

    bool run_;
    //bool loop_not_finished_ ;

    //nb of period since the beginning
    uint periodNb_;

    //loop delay
    uint loopDelayInMillisec_;

    CodeursInterface *codeurs_;
    Odometrie *odo_;
    MotorsController *motorC_;
    ConsignController *consignC_;
    CommandManagerA *commandM_;

    TRAJ_STATE pathStatus_;
    ROBOTPOSITION p_; //position du robot via cet asservissement

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref AsservEsialR.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("AsservEsialR");
        return instance;
    }

    /*!
     * \brief Retourne le \ref Logger file associé à la classe \ref AsservEsialR.
     */
    static inline const logs::Logger & loggerFile() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("logFileAsservEsialR");
        return instance;
    }

public:
    AsservEsialR(Robot * robot);
    ~AsservEsialR() {
    }

    void setSamplingFrequency(uint frequency);
    void loadConfig();
    void startAsserv(int freqHz);
    void initAsserv();
    void stopAsserv();
    void resetAsserv();
    TRAJ_STATE waitEndOfTraj();
    int getAsservStatus();

    void endWhatTodo();

    //fonctions asservissements externe par defaut
    void odo_SetPosition(float x_mm, float y_mm, float angle_rad);
    ROBOTPOSITION odo_GetPosition();

    int path_GetLastCommandStatus();
    void path_InterruptTrajectory();
    void path_CollisionOnTrajectory();
    void path_CollisionRearOnTrajectory();
    void path_CancelTrajectory();
    void path_ResetEmergencyStop();
    TRAJ_STATE motion_DoLine(float dist_mm);
    TRAJ_STATE motion_DoFace(float x_mm, float y_mm);
    TRAJ_STATE motion_DoFaceReverse(float x_mm, float y_mm);
    TRAJ_STATE motion_DoRotate(float angle_radians);
    TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius);

    TRAJ_STATE motion_Goto(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoReverse(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoChain(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoReverseChain(float x_mm, float y_mm);

    void motion_FreeMotion(void);
    void motion_DisablePID();           //! just disable PID
    void motion_AssistedHandling(void);           //! Assisted movement mode =)
    void motion_ActivateManager(bool enable);
    void motion_setLowSpeedForward(bool enable, int percent = 0);
    void motion_setLowSpeedBackward(bool enable, int percent = 0);
    void motion_setMaxSpeed(bool enable, int speed_dist_m_sec = 0, int speed_angle_rad_sec=0);

    void motion_ActivateReguDist(bool enable);
    void motion_ActivateReguAngle(bool enable);
    void motion_ResetReguDist();
    void motion_ResetReguAngle();
    TRAJ_STATE motion_DoDirectLine(float dist_mm);

    void motion_ActivateQuadRamp(bool enable);

    //FCT NON utilisée : commandes directes concernant les moteurs et codeurs
    void setMotorLeftPosition(int power, long ticks);
    void setMotorRightPosition(int power, long ticks);
    void setMotorLeftPower(int power, int time);
    void setMotorRightPower(int power, int time);
    void stopMotorLeft();
    void stopMotorRight();
    int getMotorLeftCurrent();
    int getMotorRightCurrent();
    long getLeftExternalEncoder();
    long getRightExternalEncoder();
    void getCountsExternal(int32_t* countR, int32_t* countL);
    void getDeltaCountsExternal(int32_t* deltaR, int32_t* deltaL);
    long getLeftInternalEncoder();
    long getRightInternalEncoder();
    void getCountsInternal(int32_t* countR, int32_t* countL);

    void resetEncoders();
    void resetInternalEncoders();
    void resetExternalEncoders();

};

#endif /* ASSERV_ESIALR_ASSERVESIALR_H_ */
