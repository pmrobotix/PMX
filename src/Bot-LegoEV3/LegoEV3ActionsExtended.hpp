#ifndef LEGOEV3_ACTIONSEXTENDED_HPP_
#define LEGOEV3_ACTIONSEXTENDED_HPP_

#include <unistd.h>
#include <string>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action/ServoUsingMotor.hpp"
#include "../Common/Action/SoundBar.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Enumération des libellés des servos.
 */
enum ServoLabel
{
    SERVO_0 = 0,
    SERVO_1_ARM_R = 1,
    SERVO_2 = 2,
    SERVO_3_FORK_R = 3,
    SERVO_4 = 4,
    SERVO_5 = 5,
    SERVO_6 = 6,
    SERVO_7_fUNNY_BACK = 7,
    SERVO_8_fUNNY_FRONT = 8,
    SERVO_9 = 9,
    SERVO_10 = 10,
    SERVO_11 = 11,
    SERVO_12 = 12,
    SERVO_13_FORK_L = 13,
    SERVO_14 = 14,
    SERVO_15_ARM_L = 15,
    // SERVO_16 = 16,
    SERVO_enumTypeEnd
};

/*!
 * \brief Enumération des libellés des servos associés au numéro de servo
 */
enum ServoAx12Label //TODO se servir de cette liste
{
    AX12_SERVO_1 = 5, //AX12

    AX12_enumTypeEnd
};

/*
 * Contains all specific robot element parts.
 */
class LegoEV3ActionsExtended: public Actions
{
private:

    /*!
     * \brief Return \ref Logger linked to \ref LegoEV3ActionsExtended.
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("LegoEV3ActionsExtended");
        return instance;
    }

    LedBar ledbar_;
    ButtonBar buttonbar_;
    SoundBar soundbar_;
    Tirette tirette_;
    Sensors sensors_;
    LcdShield lcd_;
    //ServoUsingMotor servoUsingMotor_;
    ServoObjectsSystem servos_;

public:


    LegoEV3ActionsExtended(std::string botId, Robot *robot);

    ~LegoEV3ActionsExtended()
    {
        //printf("~LegoEV3ActionsExtended()\n");
    }

    LedBar& ledBar()
    {
        return ledbar_;
    }

    ButtonBar& buttonBar()
    {
        return buttonbar_;
    }

    SoundBar& soundBar()
    {
        return soundbar_;
    }

    Tirette& tirette()
    {
        return tirette_;
    }

    Sensors& sensors()
    {
        return sensors_;
    }

    LcdShield& lcd()
    {
        return lcd_;
    }

//    ServoUsingMotor& servoUsingMotor() {
//        return servoUsingMotor_;
//    }

    ServoObjectsSystem& servos()
    {
        return servos_;
    }

    void stopExtra()
    {

        sensors_.stopTimerSensors();
        ledbar_.resetAll();
        ledbar_.stop();
        soundbar_.stop();
        servos_.stopTimers();
        releaseAll();
    }

    void releaseAll()
    {

        for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
            ServoLabel foo = static_cast<ServoLabel>(fooInt);
            //servosStd().deploy(foo, 0); //NE PAS FAIRE car cette fonction est utilisée dans la destruction des objets
            servos().release(foo);
        }
    }

    //--------------------------------------------------------------
    //Actions 2023
    //--------------------------------------------------------------
    void init_servos(int keep = 0)
    {
        /*
         int r = -1;
         for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
         ServoLabel foo = static_cast<ServoLabel>(fooInt);
         r = servos().getPulseWidth(foo); //afin de mettre la position des servo a jour //TODO pwm renvoi 0 ?? A CORRIGER ?
         //logger().info() << "foo "<< foo << " : " << r << logs::end;
         }
         */
        funny_action_init(0);
        fork_front_right_init(0);
        fork_front_left_init(0);
        usleep(400000);
        arm_left_init(0);
        arm_right_init(keep);
    }
    //keep_millisec = -1 : wait moving until position
    //keep_millisec > 0 : time ms to wait then release
    //keep_millisec = 0 : continue and hold

    //FUNNY
    void funny_action_full(int keep = 0)
    {

        arm_left_deploy(0);
        arm_right_deploy(0);
        usleep(400000);
        fork_front_right_deploy(0);
        fork_front_left_deploy(0);

        servos().deploy(SERVO_7_fUNNY_BACK, 2015, 0);
        servos().deploy(SERVO_8_fUNNY_FRONT, 540, keep);

    }

    void funny_action_init(int keep = 0)
    {
        servos().deploy(SERVO_7_fUNNY_BACK, 935, 0);
        servos().deploy(SERVO_8_fUNNY_FRONT, 1500, keep);
    }

    void fork_test_parallele(bool wait = true)
    {
        fork_close_init();
        utils::sleep_for_micros(1000000);
        fork_front_right_deploy_half(0);
        fork_front_left_deploy_half(0);
        utils::sleep_for_micros(2000000);
        servos().move_2_servos(wait, 300, SERVO_3_FORK_R, 1380, 0, SERVO_13_FORK_L, 1450, 0, 0, 0);
        utils::sleep_for_micros(2000000);
        fork_close_init();
        utils::sleep_for_micros(1000000);
    }

    void fork_init_slow(bool wait = true)
    {
        if (!this->is_started())
        {
            logger().error() << "ACTION NOT STARTED !!" << logs::end;
        }
        servos().move_2_servos(wait, 2000, SERVO_3_FORK_R, 1380, 0, SERVO_13_FORK_L, 1450, 0, 0, 0);
    }

    //FORK
    void fork_open_take_slow(bool wait = true)
    {
        servos().move_2_servos(wait, 1000, SERVO_3_FORK_R, 585, 0, SERVO_13_FORK_L, 2175, 0, 0, 0);
    }

    void fork_open_take()
    {
        fork_front_right_deploy(0);
        fork_front_left_deploy(0);
    }
    void fork_open_half()
    {
        fork_front_right_deploy_half(0);
        fork_front_left_deploy_half(1000);
    }
    void fork_close_init()
    {
        fork_front_left_init(0);
        fork_front_right_init(0);
    }
    //RIGHT
    void fork_front_right_init_slow(int keep = 0)
    {
        servos().deployWithVelocity(SERVO_3_FORK_R, 1380, keep, 2000);
    }
    void fork_front_right_init(int keep = 0)
    {
        servos().deploy(SERVO_3_FORK_R, 1380, keep);
    }
    void fork_front_right_deploy_half(int keep = 0)
    {
        servos().deploy(SERVO_3_FORK_R, 975, keep);
    }
    void fork_front_right_deploy(int keep = 0)
    {
        servos().deploy(SERVO_3_FORK_R, 585, keep);

    }

    //LEFT
    void fork_front_left_init_slow(int keep = 0)
    {
        servos().deployWithVelocity(SERVO_13_FORK_L, 1500, keep, 2000);
    }
    void fork_front_left_init(int keep = 0)
    {
        servos().deploy(SERVO_13_FORK_L, 1450, keep);
    }
    void fork_front_left_deploy_half(int keep = 0)
    {
        servos().deploy(SERVO_13_FORK_L, 1800, keep);
    }
    void fork_front_left_deploy(int keep = 0)
    {
        servos().deploy(SERVO_13_FORK_L, 2175, keep);
        //servos().deployWithVelocity(SERVO_13_FORK_L, 2175, keep, 3000);
    }

    void ranger_pinces(int keep = 0)
    {
        arm_left_init(0);
        arm_right_init(keep);
    }

    //ARM LEFT
    void arm_left_init(int keep = 0)
    {
        servos().deploy(SERVO_15_ARM_L, 2260, keep);//2205
    }
    void arm_left_deploy(int keep = 0)
    {
        servos().deploy(SERVO_15_ARM_L, 1420, keep);
    }

    void arm_left_deploy_full(int keep = 0)
    {
        servos().deploy(SERVO_15_ARM_L, 1200, keep);
    }

    //ARM RIGHT
    void arm_right_init(int keep = 0)
    {
        servos().deploy(SERVO_1_ARM_R, 445, keep);
    }
    void arm_right_deploy(int keep = 0)
    {
        servos().deploy(SERVO_1_ARM_R, 1460, keep);    //1310
    }
    void arm_right_deploy_full(int keep = 0)
    {
        servos().deploy(SERVO_1_ARM_R, 1500, keep);
    }

}
;

#endif

