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
    void init_servos()
    {
        /*
         int r = -1;
         for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
         ServoLabel foo = static_cast<ServoLabel>(fooInt);
         r = servos().getPulseWidth(foo); //afin de mettre la position des servo a jour //TODO pwm renvoi 0 ?? A CORRIGER ?
         //logger().info() << "foo "<< foo << " : " << r << logs::end;
         }
         */
        funny_action_init();
        fork_front_right_init(0);
        fork_front_left_init(0);
        sleep(1);
        arm_left_init(0);
        arm_right_init(0);
    }
    //keep_millisec = -1 : wait moving until position
    //keep_millisec > 0 : time ms to wait then release
    //keep_millisec = 0 : continue and hold

    //FUNNY
    void funny_action()
    {

        arm_left_deploy(0);
        arm_right_deploy(0);
        sleep(1);
        fork_front_right_deploy(0);
        fork_front_left_deploy(0);

        servos().deploy(SERVO_7_fUNNY_BACK, 2015, 0);
        servos().deploy(SERVO_8_fUNNY_FRONT, 540, 0);

    }

    void funny_action_init()
    {
        servos().deploy(SERVO_7_fUNNY_BACK, 935, 0);
        servos().deploy(SERVO_8_fUNNY_FRONT, 1500, 0);
    }

    void fork_open_init_parallele()
    {
        fork_front_right_deploy_half(0);
        servos().move_2_servos(true, 4000, SERVO_3_FORK_R, 1380, 0, SERVO_13_FORK_L, 1500, 0, 0, 0);
    }

    //FORK
    void fork_open()
    {
        fork_front_right_deploy(0);
        fork_front_left_deploy(1500);
    }
    void fork_open_half()
    {
        fork_front_right_deploy_half(0);
        fork_front_left_deploy_half(1000);
    }
    void fork_close()
    {
        fork_front_left_init(500);
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
        servos().deploy(SERVO_13_FORK_L, 1500, keep);
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

    //ARM LEFT
    void arm_left_init(int keep = 0)
    {
        servos().deploy(SERVO_15_ARM_L, 2205, keep);
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

    //2022
    /*
     void init_servos() {
     for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
     ServoLabel foo = static_cast<ServoLabel>(fooInt);
     servos().getPulseWidth(foo); //afin de mettre la position des servo a jour //TODO pwm renvoi 0 ??
     }

     cube_keep(0);
     square_middle_init(0);
     fork_front_right_init(0);
     fork_front_left_init(0);
     arm_left_init(0);
     arm_right_init(0);
     }
     //keep_millisec = -1 : wait moving until position
     //keep_millisec > 0 : time to wait then release
     //keep_millisec = 0 : continue and hold

     void init_mettre_cube() {
     cube_up_trophy(1000); //mode rapide
     cube_normal_pos(4000);
     cube_keep(0);
     }

     //CUBE AND TROPHY
     void cube_keep(int keep = 0) {
     servos().deploy(SERVO_11_TAKE_TROPHY, 1500, keep);
     }
     void cube_normal_pos(int milli0to90 = 4000) {
     //servos().deploy(SERVO_11_TAKE_TROPHY, 1650, keep);
     servos().setPos(SERVO_11_TAKE_TROPHY, 1600, milli0to90); //utilisation du speed
     }
     void cube_push_and_take_trophy(int milli0to90 = 4000) {
     //servos().deploy(SERVO_11_TAKE_TROPHY, 1950, keep);
     servos().setPos(SERVO_11_TAKE_TROPHY, 2000, milli0to90); //utilisation du speed
     }
     //fonction normal
     void cube_up_trophy(int keep = 0) {
     servos().deploy(SERVO_11_TAKE_TROPHY, 2000, keep);
     }

     //PUSH SQUARE
     void square_push_right(int keep = 0) {
     servos().deploy(SERVO_7_PUSH_SQUARES, 2400, keep);
     }
     void square_middle_init(int keep = 0) {
     servos().deploy(SERVO_7_PUSH_SQUARES, 1600, keep);
     }
     void square_push_left(int keep = 0) {
     servos().deploy(SERVO_7_PUSH_SQUARES, 900, keep);
     }

     //FORK
     void fork_open() {
     fork_front_right_deploy(0);
     fork_front_left_deploy(0);
     }
     void fork_open_half() {
     fork_front_right_deploy_half(0);
     fork_front_left_deploy_half(1000);
     }
     void fork_close() {
     fork_front_left_init(500);
     fork_front_right_init(0);
     }

     void fork_front_right_init(int keep = 0) {
     servos().deploy(SERVO_3_FORK_R, 2450, keep);
     }
     void fork_front_right_deploy_half(int keep = 0) {
     servos().deploy(SERVO_3_FORK_R, 1300, keep);
     }
     void fork_front_right_deploy(int keep = 0) {
     servos().deploy(SERVO_3_FORK_R, 750, keep);
     }
     void fork_front_left_init(int keep = 0) {
     servos().deploy(SERVO_13_FORK_L, 550, keep);
     }
     void fork_front_left_deploy_half(int keep = 0) {
     servos().deploy(SERVO_13_FORK_L, 1800, keep);
     }
     void fork_front_left_deploy(int keep = 0) {
     servos().deploy(SERVO_13_FORK_L, 2200, keep);
     }

     //ARM

     void arm_left_init(int keep = 0) {
     servos().deploy(SERVO_15_ARM_L, 1500, keep);
     }
     void arm_left_deploy(int keep = 0) {
     servos().deploy(SERVO_15_ARM_L, 530, keep); //450
     }
     void arm_right_init(int keep = 0) {
     servos().deploy(SERVO_1_ARM_R, 1500, keep);
     }
     void arm_right_deploy(int keep = 0) {
     servos().deploy(SERVO_1_ARM_R, 2320, keep); //2320
     }
     */

//    void test_servo(int speed = 512) {
//        servos().deploy(SERVO_1, 1900, 0);
//        usleep(1000000);
//        servos().deploy(SERVO_1, 500, 0);
//        usleep(1000000);
//        servos().release(SERVO_1);
//
//        servos().deploy(SERVO_2, 1900, 0);
//                usleep(1000000);
//                servos().deploy(SERVO_2, 500, 0);
//                usleep(1000000);
//                servos().release(SERVO_2);
//    }
    /*
     //--------------------------------------------------------------
     //Actions 2019
     //--------------------------------------------------------------
     void init_servos()
     {
     conveyorBelt_Left_center();
     left_arm_retract();
     right_arm_retract();
     }
     void right_eject_all(int sleeptimems)
     {
     logger().info() << "conveyorBelt_Left_low" << logs::end;
     conveyorBelt_Left_low(0);

     logger().info() << "conveyorBelt_EjectRight" << logs::end;
     conveyorBelt_EjectRight(sleeptimems);
     }

     void left_eject_all(int sleeptimems)
     {
     logger().info() << "conveyorBelt_Right_low" << logs::end;
     conveyorBelt_Right_low(0);

     logger().info() << "conveyorBelt_EjectLeft" << logs::end;
     conveyorBelt_EjectLeft(sleeptimems);
     }

     void left_prendre_palet(int push, int nb, bool bleu = false)
     {
     conveyorBelt_Left_low(0);

     for (int i = 1; i <= nb; i++) {
     logger().info() << "left_arm_take" << logs::end;
     left_arm_take();
     logger().info() << "left_arm_retract" << logs::end;

     if (bleu) {
     logger().info() << "bleu!!!!!!!!!!!!!!!!" << logs::end;
     conveyorBelt_PushRightTremble();
     }
     left_arm_retract(0);
     conveyorBelt_PushRight(push);
     }
     //2nd time
     //logger().info() << "left_arm_take" << logs::end;
     //left_arm_take();
     //logger().info() << "left_arm_retract" << logs::end;
     //left_arm_retract();

     }
     void right_prendre_palet(int push, int nb, bool bleu = false)
     {
     conveyorBelt_Right_low(0);
     for (int i = 1; i <= nb; i++) {
     logger().info() << "right_arm_take" << logs::end;
     right_arm_take();

     logger().info() << "right_arm_retract" << logs::end;
     if (bleu) {
     logger().info() << "bleu!!!!!!!!!!!!!!!!" << logs::end;
     conveyorBelt_PushLeftTremble();
     }
     right_arm_retract(0);
     conveyorBelt_PushLeft(push);
     }
     //2nd time
     //logger().info() << "right_arm_take" << logs::end;
     //right_arm_take();
     //logger().info() << "right_arm_retract" << logs::end;
     //right_arm_retract();

     }

     void left_arm_center(int speed = 512)
     {
     servosStd().deploy(SERVO_7, 0, 0);
     usleep(1000000);
     servosStd().release(SERVO_7);
     }
     void left_arm_take(int keep_ms = 1000) //1500
     {
     servosStd().deploy(SERVO_7, -90, keep_ms);
     //usleep(1500000);
     //servosStd().release(SERVO_7);
     }
     void left_arm_retract(int keep_ms = 500)
     {
     servosStd().deploy(SERVO_7, 90, keep_ms);
     //        usleep(1000000);
     //        servosStd().release(SERVO_7);
     }

     void right_arm_center()
     {
     servosStd().deploy(SERVO_5, 0, 1000);
     //        usleep(1000000);
     //        servosStd().release(SERVO_5);
     }
     void right_arm_take(int keep_ms = 1000) //1500
     {
     servosStd().deploy(SERVO_5, 90, keep_ms);
     //usleep(1500000);
     //servosStd().release(SERVO_5);
     }
     void right_arm_retract(int keep_ms = 500)
     {
     servosStd().deploy(SERVO_5, -90, keep_ms);
     //        usleep(1000000);
     //        servosStd().release(SERVO_5);
     }

     void conveyorBelt_Left_center()
     {
     servosStd().deploy(SERVO_1, 0, 1000);
     //        usleep(1000000);
     //        servosStd().release(SERVO_1);
     }
     void conveyorBelt_Right_low(int keep_ms = 2000)
     {
     servosStd().deploy(SERVO_1, -100, keep_ms);

     //        if (!keep_activated) {
     //            usleep(2000000);
     //            servosStd().release(SERVO_1);
     //        }
     }

     void conveyorBelt_Left_low(int keep_ms = 2000)
     {
     servosStd().deploy(SERVO_1, 100, keep_ms);

     //        if (!keep_activated) {
     //            usleep(2000000);
     //            servosStd().release(SERVO_1);
     //        }

     }

     void conveyorBelt_PushRightTremble()
     {
     servoUsingMotor().moveRight(400);
     usleep(400000);
     servoUsingMotor().moveRight(-400);
     usleep(400000);
     //servoUsingMotor().moveRight(400);
     //usleep(300000);

     }

     void conveyorBelt_PushLeftTremble()
     {
     servoUsingMotor().moveLeft(400);
     usleep(400000);
     servoUsingMotor().moveLeft(-400);
     usleep(400000);
     //servoUsingMotor().moveRight(400);
     //usleep(300000);

     }

     void conveyorBelt_PushRight(int nb_position)
     {
     servoUsingMotor().moveRight(nb_position);
     sleep(1);
     }

     void conveyorBelt_PushLeft(int nb_position)
     {
     servoUsingMotor().moveLeft(nb_position);
     sleep(1);
     }
     void conveyorBelt_EjectLeft(int sleeptimems)
     {
     servoUsingMotor().ejectLeft();
     usleep(sleeptimems * 1000.0);
     }
     void conveyorBelt_EjectRight(int sleeptimems)
     {
     servoUsingMotor().ejectRight();
     usleep(sleeptimems * 1000.0);
     }
     */
}
;

#endif

