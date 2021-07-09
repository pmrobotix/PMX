/*!
 * \file
 * \brief Définition de la classe AsservDriver (OPOS6UL).
 */

#ifndef OPOS6UL_ASSERVDRIVER_HPP_
#define OPOS6UL_ASSERVDRIVER_HPP_



#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../Thread/Thread.hpp"
//#include <include/CppLinuxSerial/SerialPort.hpp>
#include "serialib.hpp"
#include <unistd.h>
#include <stdio.h>
#include <vector>

using namespace std;
//using namespace mn::CppLinuxSerial;

//#define	SERIAL_ADDRESS      "/dev/ttymxc1"
#define SERIAL_PORT "/dev/ttymxc1"

// convert float to byte array  source: http://mbed.org/forum/helloworld/topic/2053/
union float2bytes_t   // union consists of one variable represented in a number of different ways
{
    float f;
    unsigned char b[sizeof(float)];

    float2bytes_t() :
            b { }
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
    static inline const logs::Logger & loggerSvg()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.OPO.SVG");
        return instance;
    }

    //SerialPort serialPort_;
    serialib serial_;

    int read_error_;
    bool connected_;
    int errorCount_;

    bool asservCardStarted_;

    int statusCountDown_;

    TRAJ_STATE pathStatus_;

    Mutex m_pos; //mutex pour la mise à jour de la position
    Mutex m_statusCountDown;

    void nucleo_flushSerial();
    int nucleo_writeSerial(char c);
    int nucleo_writeSerialSTR(string str);

    TRAJ_STATE nucleo_waitEndOfTraj();

    void parseAsservPosition(string str);

    void tokenize(std::string const &str, const char delim, std::vector<std::string> &out);

protected:

    virtual void execute();
    RobotPosition p_;
public:

    void endWhatTodo();

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
    void getCountsExternal(int32_t* countR, int32_t* countL){} //TODO
    void getCountsInternal(int32_t* countR, int32_t* countL){} //TODO
    long getLeftInternalEncoder();
    long getRightInternalEncoder();
    void resetEncoders();
    void resetInternalEncoders();
    void resetExternalEncoders();

    //commandes optionnelles de gestion externe
    //deprecated
    //void enableHardRegulation(bool enable);

    //fonctions asservissements externe par defaut
    void odo_SetPosition(float x_mm, float y_mm, float angle_rad);
    RobotPosition odo_GetPosition();
    int path_GetLastCommandStatus();
    void path_InterruptTrajectory();
    void path_CollisionOnTrajectory();
    void path_CollisionRearOnTrajectory();
    void path_CancelTrajectory();
    void path_ResetEmergencyStop();
    TRAJ_STATE motion_DoLine(float dist_mm);
    TRAJ_STATE motion_DoFace(float x_mm, float y_mm);
    TRAJ_STATE motion_DoRotate(float angle_radians);
    TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius);
    TRAJ_STATE motion_Goto(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoReverse(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoChain(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoReverseChain(float x_mm, float y_mm);

    void motion_FreeMotion(void);
    void motion_DisablePID();		//! just disable PID Deprecated
    void motion_AssistedHandling(void);		//! Assisted movement mode =)
    void motion_ActivateManager(bool enable);
    //void motion_setLowSpeed(bool enable);
    void motion_setLowSpeedForward(bool enable, int percent); //TODO remove enable
    void motion_setLowSpeedBackward(bool enable, int percent); //TODO remove enable

    void motion_ActivateReguDist(bool enable);
    void motion_ActivateReguAngle(bool enable);
    void motion_ResetReguDist();
    void motion_ResetReguAngle();
    TRAJ_STATE motion_DoDirectLine(float dist_mm);



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
