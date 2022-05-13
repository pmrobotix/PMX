#ifndef ASSERVMAIN_H_
#define ASSERVMAIN_H_

#include <stdint.h>
#include <sys/types.h>

#include "../Thread/Mutex.hpp"

class MotorController;

class CommandManager;
class Encoders;
class Odometry;
class Pll;
class AccelerationDecelerationLimiterInterface;
class SpeedController;
class Regulator;
class BlockingDetector;

class AsservMain
{
public:
    explicit AsservMain(uint16_t loopFrequency, uint16_t speedPositionLoopDivisor, float wheelRadius_mm,
            float encoderWheelsDistance_mm, uint32_t encodersTicksByTurn, CommandManager &commandManager,
            MotorController &motorController, Encoders &encoders, Odometry &odometrie,
            Regulator &angleRegulator, Regulator &distanceRegulator,
            AccelerationDecelerationLimiterInterface &angleRegulatorAccelerationLimiter, AccelerationDecelerationLimiterInterface &distanceRegulatorAccelerationLimiter,
            SpeedController &speedControllerRight, SpeedController &speedControllerLeft,
            Pll &rightPll, Pll &leftPll,
            BlockingDetector *blockingDetector = nullptr);

    virtual ~AsservMain()
    {
    }
    ;

    void mainLoop();

    /*
     *    On peut donner une vitesse par roue en utilisant la fonction: setWheelsSpeed
     *    Le mode de fonctionnement change et doit être remis à la normal en utilisant: resetToNormalMode
     *     avant de donner une consigne via setRegulatorsSpeed ou via le CommandManager
     */
    void setWheelsSpeed(float rightWheelSpeed, float leftWheelSpeed);

    /*
     *    On peut donner une commande de vitesse tel qu'elle sortirait des régulateur d'angle/distance
     *    Le mode de fonctionnement change et doit être remis à la normal en utilisant: resetToNormalMode
     *     avant de donner une consigne via setRegulatorsSpeed ou via le CommandManager
     */
    void setRegulatorsSpeed(float distSpeed, float angleSpeed);
    void resetToNormalMode();

    void enableMotors(bool enable);
    void enablePolar(bool enable);

    void reset();

    void setEmergencyStop();
    void resetEmergencyStop();

    void enableAngleRegulator();
    void disableAngleRegulator();
    void enableDistanceRegulator();
    void disableDistanceRegulator();

    void setPosition(float X_mm, float Y_mm, float theta_rad);
    void limitMotorControllerConsignToPercentage(float percentage);
private:

    float convertSpeedTommSec(float speed_ticksPerSec);
    float estimateDeltaAngle(int16_t deltaCountRight, int16_t deltaCountLeft);
    float estimateDeltaDistance(int16_t deltaCountRight, int16_t deltaCountLeft);

    typedef enum
    {
        normal_mode, direct_speed_mode, regulator_output_control
    } asserv_mode_t;

    MotorController &m_motorController;
    Encoders &m_encoders;
    Odometry &m_odometry;
    SpeedController &m_speedControllerRight;
    SpeedController &m_speedControllerLeft;
    Regulator &m_angleRegulator;
    Regulator &m_distanceRegulator;
    AccelerationDecelerationLimiterInterface &m_angleRegulatorAccelerationLimiter;
    AccelerationDecelerationLimiterInterface &m_distanceRegulatorAccelerationLimiter;
    CommandManager &m_commandManager;
    Pll &m_pllRight;
    Pll &m_pllLeft;
    BlockingDetector *m_blockingDetector;

    const float m_distanceByEncoderTurn_mm;
    const float m_encodersTicksByTurn;
    const float m_encodermmByTicks;
    const float m_encoderWheelsDistance_mm;
    const float m_encoderWheelsDistance_ticks;

    const uint16_t m_loopFrequency;
    const float m_loopPeriod;
    const uint16_t m_speedPositionLoopDivisor;
    uint8_t m_asservCounter;

    float m_distRegulatorOutputSpeedConsign;
    float m_distSpeedLimited;

    float m_angleRegulatorOutputSpeedConsign;
    float m_angleSpeedLimited;

    bool m_enableMotors;
    bool m_enablePolar;
    asserv_mode_t m_asservMode;
    float m_directSpeedMode_rightWheelSpeed;
    float m_directSpeedMode_leftWheelSpeed;


    utils::Mutex mutex_;
};

#endif /* ASSERVMAIN_H_ */
