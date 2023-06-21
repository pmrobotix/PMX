#ifndef SIMU_ASSERVDRIVER_HPP_
#define SIMU_ASSERVDRIVER_HPP_

#include <bits/stdint-intn.h>
#include <cmath>
#include <cstdio>
#include <string>
#include <thread>

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Common/Utils/Chronometer.hpp"
#include "../Common/Action/ITimerPosixListener.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class AsservDriver: public AAsservDriver, utils::Thread //, ITimerPosixListener
{

public:
    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(SIMU).
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("AsservDriver.SIMU");
        return instance;
    }

private:
    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref AsservDriverMemory(SIMU).
     */
    static inline const logs::Logger& loggerM()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("AsservDriverMemory.SIMU");
        return instance;
    }

    static inline const logs::Logger& loggerSvg()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("AsservDriver.SIMU.SVG");
        return instance;
    }

    Mutex m_pos; //mutex pour la mise à jour de la position
    bool asservSimuStarted_;

    Mutex mutexL_;
    Mutex mutexR_;

    float simuTicksPerMeter_;
    float simuMaxSpeed_;
    float simuMaxPower_;

    utils::Chronometer chrono_;

    float tLeft_ms_;
    float tRight_ms_;

    //float timesMultiplicator_;

    float rightCounter_; //real ticks
    float leftCounter_; //real ticks
    float rightMm_;
    float leftMm_;

    float currentRightCounter_;
    float currentLeftCounter_;

    std::string botid_;

    //Go to Project -> Properties -> C/C++ General -> Path and Symbols -> Tab [Symbols].
    //Add the symbol : __cplusplus with the value 201103L
    std::thread twLeft_;
    std::thread twRight_;

    float inverseCodeurG_;
    float inverseCodeurD_;

    float inverseMoteurG_;
    float inverseMoteurD_;

protected:

    virtual void execute();
    RobotPosition p_; //position SIMU du robot

    float convertPowerToSpeed(int power);
    float convertMmToTicks(float meters);

public:

    //fontions en public pour l'accessibilité aux threads de simu de calcul de position
    void computeCounterL();
    void computeCounterR();

    bool emergencyStop_;

    float leftSpeed_; //real speed in m/s
    float rightSpeed_;

    float wantedRightSpeed_;
    float wantedLeftSpeed_;

//    void onTimer(utils::Chronometer chrono);
//    void onTimerEnd(utils::Chronometer chrono);

    //functions Asserv direct et minimum en cas d'asserv interne

    //functions Asserv interne
    void endWhatTodo();

    void setMotorLeftPosition(int power, long ticks);
    void setMotorRightPosition(int power, long ticks);
    void setMotorLeftPower(int power, int time);
    void setMotorRightPower(int power, int time);
    long getLeftExternalEncoder();
    long getRightExternalEncoder();
    void getCountsExternal(int32_t *countR, int32_t *countL); //ticks cumulés
    void getDeltaCountsExternal(int32_t *deltaR, int32_t *deltaL);
    void getCountsInternal(int32_t *countR, int32_t *countL); //ticks cumulés
    long getLeftInternalEncoder(); //ticks cumulés
    long getRightInternalEncoder(); //ticks cumulés
    void resetEncoders();
    void resetInternalEncoders();
    void resetExternalEncoders();
    void stopMotorLeft();
    void stopMotorRight();
    int getMotorLeftCurrent();
    int getMotorRightCurrent();

    //fonctions asservissements externe par defaut
    void odo_SetPosition(float x_mm, float y_mm, float angle_rad);
    RobotPosition odo_GetPosition();
    int path_GetLastCommandStatus(); //deprecated
    void path_InterruptTrajectory();
    void path_CollisionOnTrajectory();
    void path_CollisionRearOnTrajectory();
    void path_CancelTrajectory();
    void path_ResetEmergencyStop();

    TRAJ_STATE motion_DoFace(float x_mm, float y_mm);
    TRAJ_STATE motion_DoFaceReverse(float x_mm, float y_mm);
    TRAJ_STATE motion_DoLine(float dist_mm);
    TRAJ_STATE motion_DoRotate(float angle_radians);
    TRAJ_STATE motion_DoArcRotate(float angle_radians, float radius);
    TRAJ_STATE motion_Goto(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoReverse(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoChain(float x_mm, float y_mm);
    TRAJ_STATE motion_GotoReverseChain(float x_mm, float y_mm);

    void motion_FreeMotion();
    void motion_DisablePID();		//! disable PID
    void motion_AssistedHandling();		//! Assisted movement mode =)
    void motion_ActivateManager(bool enable);
    void motion_setLowSpeedForward(bool enable, int percent);
    void motion_setLowSpeedBackward(bool enable, int percent);

    //Functions deprecated
    void motion_ActivateReguDist(bool enable);
    void motion_ActivateReguAngle(bool enable);
    void motion_ResetReguDist();
    void motion_ResetReguAngle();
    TRAJ_STATE motion_DoDirectLine(float dist_mm); //uniquement en consigne sans le command manager

    /*!
     * \brief Constructor.
     */
    AsservDriver(std::string botid);

    /*!
     * \brief Destructor.
     */
    ~AsservDriver();

};

/*
 // http://stackoverflow.com/questions/10673585/start-thread-with-member-function
 Wrapper *w = new Wrapper();
 //std::thread tw1 = w->member1Thread();
 std::thread tw2 = w->member2Thread("hello", 100);
 //tw1.join();
 tw2.join();
 */
class AsservDriverWrapper
{
public:
    AsservDriverWrapper(AsservDriver *asserv)
    {
        asserv_ = asserv;
    }
    ~AsservDriverWrapper()
    {
    }

    AsservDriver *asserv_;

    void member1left(const char *arg1, int time_ms)
    {
        float lastspeed = asserv_->leftSpeed_;

        if (time_ms > 0) {
            utils::Chronometer chrono_member1left("chrono_member1left");
            chrono_member1left.start();
            while (chrono_member1left.getElapsedTimeInMilliSec() < (double) time_ms) {
                asserv_->computeCounterL();
                //usleep(500);
                if (asserv_->emergencyStop_)
                    break;
            }
            asserv_->stopMotorLeft();
        } else {
            while (asserv_->wantedLeftSpeed_ != lastspeed) //stop when speed has changed
            {
                asserv_->computeCounterL();
                //usleep(500);
                if (asserv_->emergencyStop_)
                    break;
            }
        }
    }
    void member2right(const char *arg1, int time_ms)
    {
        float lastspeed = asserv_->rightSpeed_;

        if (time_ms > 0) {
            utils::Chronometer chrono_member2right("chrono_member2right");
            chrono_member2right.start();
            while (chrono_member2right.getElapsedTimeInMilliSec() < (double) time_ms) {
                asserv_->computeCounterR();
                //usleep(500);
                if (asserv_->emergencyStop_)
                    break;
            }
            asserv_->stopMotorRight();
        } else {
            while (asserv_->wantedRightSpeed_ != lastspeed) //stop when speed has changed
            {
                asserv_->computeCounterR();
                //usleep(500);
                if (asserv_->emergencyStop_)
                    break;
            }
        }
    }

    void positionLeft(const char *arg1, long internal_ticksToDo) //tick encoder à atteindre
    {
        printf("positionLeft internal_ticksToDo=%ld   leftSpeed_=%f wanted=%f\n", internal_ticksToDo,
                asserv_->leftSpeed_, asserv_->wantedLeftSpeed_);

        long ticks = std::abs(asserv_->getLeftInternalEncoder());
        if (asserv_->leftSpeed_ > 0) {
            //stop when internal ticks is achieved..
            while (ticks < internal_ticksToDo) {
                asserv_->computeCounterL();
                ticks = std::abs(asserv_->getLeftInternalEncoder());
                //usleep(500);
            }
        } else if (asserv_->leftSpeed_ < 0) {
            while (ticks > internal_ticksToDo) {
                asserv_->computeCounterL();
                ticks = std::abs(asserv_->getLeftInternalEncoder());
                //usleep(500);
            }
        } else {
            printf("positionLeft rightSpeed_=0 !! STOP");
        }

        //printf("AFTER positionLeft internal_ticksToDo=%ld   ticks=%ld\n", internal_ticksToDo, ticks);
        asserv_->stopMotorLeft();
    }

    void positionRight(const char *arg1, long internal_ticksToDo)
    {
        //printf("positionRight internal_ticksToDo=%ld rightSpeed_=%f wanted=%f\n", internal_ticksToDo, asserv_->rightSpeed_, asserv_->wantedRightSpeed_);

        long ticks = std::abs(asserv_->getRightInternalEncoder());
        if (asserv_->rightSpeed_ > 0) {
            //stop when internal ticks is achieved..
            while (ticks < internal_ticksToDo) {
                asserv_->computeCounterR();
                ticks = std::abs(asserv_->getRightInternalEncoder());
                //usleep(500);
            }
            printf("> positionRight while out ticks=%ld internal_ticksToDo=%ld \n", ticks, internal_ticksToDo);

        } else if (asserv_->rightSpeed_ < 0) {
            while (ticks > internal_ticksToDo) {
                asserv_->computeCounterR();
                ticks = std::abs(asserv_->getRightInternalEncoder());
                //usleep(500);
            }
            printf("< positionRight while out ticks=%ld internal_ticksToDo=%ld \n", ticks, internal_ticksToDo);
        } else {
            printf("positionRight rightSpeed_=0 !! STOP");

        }

        //printf("AFTER positionRight internal_ticksToDo=%ld   ticks=%ld\n", internal_ticksToDo, ticks);
        asserv_->stopMotorRight();
    }

    std::thread memberLeftThread(const char *arg1, int timems)
    {
        return std::thread([=]
        {
            this->member1left(arg1, timems);
        });
    }

    std::thread memberRightThread(const char *arg1, int timems)
    {
        return std::thread([=]
        {
            this->member2right(arg1, timems);
        });
    }

    std::thread positionLeftThread(const char *arg1, int internal_ticks)
    {
        return std::thread([=]
        {
            this->positionLeft(arg1, internal_ticks);
        });
    }

    std::thread positionRightThread(const char *arg1, int internal_ticks)
    {
        return std::thread([=]
        {
            this->positionRight(arg1, internal_ticks);
        });
    }

};

#endif
