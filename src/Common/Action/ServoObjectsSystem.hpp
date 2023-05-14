#ifndef SERVOOBJECTSSYSTEM_HPP_
#define SERVOOBJECTSSYSTEM_HPP_

#include <sys/types.h>
#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../Action.Driver/AServoDriver.hpp"
#include "../Utils/Chronometer.hpp"
#include "ITimerPosixListener.hpp"
#include "ITimerListener.hpp"
#include "AActionsElement.hpp"

/*!
 * \brief Enumération des libellés des actions.
 */
enum ServoTimerName {
    MOVE_1_SERVO,
    MOVE_2_SERVOS
};


class ServoObjectsSystem: public AActionsElement {

private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoObjectsSystem.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoObjectsSystem");
        return instance;
    }

    AServoDriver* servodriver_;

    /*!
     * \brief ID du robot.
     */
    std::string botId_;

protected:

public:

    /*!
     * \brief Constructor.
     */
    ServoObjectsSystem(std::string botId, Actions & actions);

    /*!
     * \brief Destructor.
     */
    ~ServoObjectsSystem();

    AServoDriver * servodriver()
    {
        return servodriver_;
    }

    bool is_connected();
    /*!
     * \brief setup 1 servo with type, min, mid, max, inv values.
     */
    bool setup(int servo, AServoDriver::ServoType type, int valueMinPulse, int valueMidPulse, int valueMaxPulse, bool inversed = false);

    /*!
     * \brief move 1 servo.
     */
    void move_1_servo(int servo1, int pos1, int torque1, int time_eta_ms, bool keep_torque, int escape_torque);
    /*!
     * \brief move 2 servos.
     */
    void move_2_servos(int servo1, int pos1, int torque1, int servo2, int pos2, int torque2, int time_eta_ms, int keep_torque, int escape_torque);

    //void deployByTimerTask(int servo, int pos, int keep_millisec = -1);
    void deploy(int servo, int pos, int keep_millisec = -1);

    //velocity est differente entre type de servo! ax12 =1023 ; servo std = time_ms from 0 to 90°
    void deployWithVelocity(int servo, int pos, int keep_millisec = -1, int velocity = 0);

    void turn(int servo, int speed, int keep_millisec = 0);
    void release(int servo);
    void hold(int servo);

    int getTorque(int servo);
    void setTorque(int servo, int torque);

    void setSpeed(int servo, int speed);
    int getSpeed(int servo);

    /*!
     * \brief fonction avec vitesse reglable
     * pos : AX12 : 0 - 512 - 1023
     */
    int setPos(int servo, int pos, int milli0to90 = 0);

    /*!
     * \brief get the pulsewidth
     */
    int getPulseWidth(int servo);
    void detectAll();
    void detect();

    std::string id() {
        return botId_;
    }
};

/*!
 * \brief Cette action permet de definir les timers concernant les servomotors.
 *
 */
class ServoObjectsTimer: public ITimerListener //public ITimerPosixListener//
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoObjectsTimer.
     */
    static const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoObjectsTimer");
        return instance;
    }

    /*!
     * \brief Référence vers le ledbar.
     */
    ServoObjectsSystem & servoObjectsSystem_;

    int servo_;
    int goal_pos_;
    int velocity_;

public:

    /*!
     * \brief Constructeur de la classe.
     * \param sensors
     *        Reference vers l'objet associée.
     */
    ServoObjectsTimer(ServoObjectsSystem & sOsS, int number_servos, uint timeSpan_us, int servo1, int pos1, int velocity);

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~ServoObjectsTimer() {
    }

    /*!
     * \brief fonction qui sera executer à chaque traitement du timer.
     */
    virtual void onTimer(utils::Chronometer chrono);

    /*!
     * \brief fonction qui sera executer en dernière action à faire pour ce timer.
     */
    virtual void onTimerEnd(utils::Chronometer chrono);

    /*!
     * \brief nom du timer.
     */
    virtual std::string info();

};

#endif
