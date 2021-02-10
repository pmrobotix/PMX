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
enum ServoLabel {
    SERVO_1, SERVO_2, SERVO_3, SERVO_4, SERVO_5, SERVO_6, SERVO_7, SERVO_8, SERVO_enumTypeEnd
};

/*
 * Contains all specific robot element parts.
 */
class LegoEV3ActionsExtended: public Actions {
private:

    /*!
     * \brief Return \ref Logger linked to \ref LegoEV3ActionsExtended.
     */
    static inline const logs::Logger& logger() {
        static const logs::Logger &instance = logs::LoggerFactory::logger("LegoEV3ActionsExtended");
        return instance;
    }

    LedBar ledbar_;
    ButtonBar buttonbar_;
    SoundBar soundbar_;
    Tirette tirette_;
    Sensors sensors_;
    LcdShield lcd_;
    ServoUsingMotor servoUsingMotor_;
    ServoObjectsSystem servos_std_;

public:
    LegoEV3ActionsExtended(std::string botId, Robot *robot);

    ~LegoEV3ActionsExtended() {
        //printf("~LegoEV3ActionsExtended()\n");
    }

    LedBar& ledBar() {
        return ledbar_;
    }

    ButtonBar& buttonBar() {
        return buttonbar_;
    }

    SoundBar& soundBar() {
        return soundbar_;
    }

    Tirette& tirette() {
        return tirette_;
    }

    Sensors& sensors() {
        return sensors_;
    }

    LcdShield& lcd() {
        return lcd_;
    }

    ServoUsingMotor& servoUsingMotor() {
        return servoUsingMotor_;
    }

    ServoObjectsSystem& servosStd() {
        return servos_std_;
    }

    void stopExtra() {
        sensors_.stopTimerSensors();
        ledbar_.resetAll();
        ledbar_.stop();
        soundbar_.stop();
        releaseAll();
    }

    void releaseAll() {

        for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
            ServoLabel foo = static_cast<ServoLabel>(fooInt);
            //servosStd().deploy(foo, 0); //NE PAS FAIRE car utilsier dans la destruction des objets
            servosStd().release(foo);
        }
    }
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
};

#endif

