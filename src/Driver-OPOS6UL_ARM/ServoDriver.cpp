//drivers...OPOS

#include <unistd.h>
#include <iostream>
#include <string>
#include "ServoDriver.hpp"

#include "../Log/Logger.hpp"
#include "CCAx12Teensy.hpp"

using namespace std;

AServoDriver * AServoDriver::create() {

    static ServoDriver *instance = new ServoDriver();
    return instance;
}

ServoDriver::ServoDriver() :
        connected_(1)
{
    logger().debug() << "ServoDriver::ServoDriver()" << logs::end;

//    //TODO le type est connu par la denomination port-servo
//    for (int i = 0; i < NB_SERVO_DYN; i++) {
//
//        servo_type_[i] = AServoDriver::SERVO_DYNAMIXEL;
//    }

    int conn = CCAx12Teensy::instance().connect(AX12TEENSY_ADDR);
    /*
     //loop
     while (1) {
     CCAx12Teensy::instance().setLedOn(4);
     setPulsePos(1007, 312, 0);
     usleep(500000);
     CCAx12Teensy::instance().setLedOff(4);
     setPulsePos(1007, 512, 0);
     usleep(500000);
     //int p = CCAx12Teensy::instance().pingAX(1, 7);
     int p = ping(1007);
     logger().error() << "CCAx12Teensy...conn=" << conn << " p=" << p << logs::end;
     }*/

}

//DEPRECATED
void ServoDriver::setType(int servo, ServoType type) {
//    if (!testIf(servo, 0, MAXPORTNUM)) return;
//    servo_type_[constrain(servo, 0, NB_SERVO_DYN - 1)] = type;
}

void ServoDriver::hold(int servo) {
    if (!testIf(servo, 0, MAXPORTNUM)) return;
    int port = servo / 1000;
    if (port == 10) {
        logger().error() << "hold() NOT IMPLEMENTED !" << logs::end;
    }
    else if (port < 5) {
        servo -= (1000 * port);
        int r = CCAx12Teensy::instance().writeAXData(port, servo, P_TORQUE_ENABLE, 1);
        if (r < 0) logger().error() << "hold() ERROR! port=" << port << " servo=" << servo << logs::end;
    }
}

void ServoDriver::setPulsePos(int servo, int pulsewidth, int rate_milli) {
    if (!testIf(servo, 0, MAXPORTNUM)) return;
    int port = servo / 1000;
    if (port == 10) {
        logger().error() << "setPulsePos() NOT IMPLEMENTED !" << logs::end;
    }
    else if (port < 5) {
        servo -= (1000 * port);
        pulsewidth = constrain(pulsewidth, 0, MAX_POS);

        int r = CCAx12Teensy::instance().writeAXData(port, servo, P_GOAL_POSITION, pulsewidth);
        if (r < 0)
            logger().error() << "setPulsePos() writeAXData ERROR port=" << port << " servo=" << servo << " pulsewidth=" << pulsewidth << logs::end;
    }
}

void ServoDriver::turn(int servo, int speed) {
    if (!testIf(servo, 0, MAXPORTNUM)) return;
    logger().error() << "ServoDriver::turn() NOT IMPLEMENTED !" << logs::end;
}

void ServoDriver::release(int servo) {
    if (!testIf(servo, 0, MAXPORTNUM)) return;
    int port = servo / 1000;
    if (port == 10) {
        logger().error() << "release() NOT IMPLEMENTED !" << logs::end;
    }
    else if (port < 5) {
        servo -= (1000 * port);
        int r = CCAx12Teensy::instance().writeAXData(port, servo, P_TORQUE_ENABLE, 0);
        if (r < 0) logger().error() << "release() ERROR! port=" << port << "servo=" << servo << logs::end;
    }
}

void ServoDriver::setRate(int servo, int speed) {
    if (!testIf(servo, 0, MAXPORTNUM)) return;
    int port = servo / 1000;
    if (port == 10) {
        logger().error() << "setRate() NOT IMPLEMENTED !" << logs::end;
    }
    else if (port < 5) {
        servo -= (1000 * port);
        int r = CCAx12Teensy::instance().writeAXData(port, servo, P_TORQUE_LIMIT, constrain(speed, 0, 1023));
        if (r < 0) logger().error() << "setRate() ERROR! port=" << port << " servo=" << servo << " speed=" << speed << logs::end;
    }
}

//return 1 when in progress, 0 when idle, -1 on error
int ServoDriver::getMoving(int servo) {
    if (!testIf(servo, 0, MAXPORTNUM)) return -10;
    int port = servo / 1000;
    if (port == 10) {
        logger().error() << "getMoving() NOT IMPLEMENTED !" << logs::end;
        return -1;
    }
    else if (port < 5) {
        servo -= (1000 * port);
        int r = CCAx12Teensy::instance().readAXData(port, servo, P_MOVING);
        if (r < 0) {
            logger().error() << "getMoving() ERROR! port=" << port << " servo=" << servo << logs::end;
        }
        return r;
    }
}

int ServoDriver::getPulsePos(int servo) {
    if (!testIf(servo, 0, MAXPORTNUM)) return -10;
    int port = servo / 1000;
    if (port == 10) {
        logger().error() << "getPulsePos() NOT IMPLEMENTED !" << logs::end;
        return -1;
    }
    else if (port < 5) {
        servo -= (1000 * port);

        int r = CCAx12Teensy::instance().readAXData(port, servo, P_PRESENT_POSITION);
        if (r < 0) logger().error() << "getPulsePos() ERROR! port=" << port << " servo=" << servo << logs::end;
        return r;
    }
    return -2;
}

//return true if available
int ServoDriver::ping(int servo) {
    if (!testIf(servo, 0, MAXPORTNUM)) return -10;
    int port = servo / 1000;
    if (port == 10) {
        logger().error() << "ping() NOT IMPLEMENTED !" << logs::end;
        return -1;
    }
    else if (port < 5) {
        servo -= (1000 * port);

        int r = CCAx12Teensy::instance().pingAX(port, servo);
        if (r < 0) {
            logger().error() << "ping() ERROR! port=" << port << " servo=" << servo << logs::end;
            return r;
        }
        if (r == AXT_AX_MISSING) return 0;
        if (r == AXT_OK) return 1;
    }
    else {
        logger().error() << "ping() port ERROR! port=" << port << " servo=" << servo << logs::end;
        return -10;
    }
}

void ServoDriver::setMinPulse(int servo, int pulse) {
    logger().debug() << "ServoDriver::setMinPulse() NOT IMPLEMENTED !" << logs::end;
}

void ServoDriver::setMidPulse(int servo, int pulse) {
    logger().debug() << "ServoDriver::setMidPulse() NOT IMPLEMENTED !" << logs::end;
}

void ServoDriver::setMaxPulse(int servo, int pulse) {
    logger().debug() << "ServoDriver::setMaxPulse() NOT IMPLEMENTED !" << logs::end;
}

void ServoDriver::setPolarity(int servo, bool inversed) {
    logger().debug() << "ServoDriver::setPolarity() NOT IMPLEMENTED !" << logs::end;
}

int ServoDriver::getTorque(int servo) {
    if (!testIf(servo, 0, MAXPORTNUM)) return -10;
    int port = servo / 1000;
    if (port == 10) {
        logger().error() << "getTorque() NOT IMPLEMENTED !" << logs::end;
    }
    else if (port < 5) {
        servo -= (1000 * port);
        int r = CCAx12Teensy::instance().readAXData(port, servo, P_PRESENT_LOAD);
        if (r < 0) logger().error() << "getTorque() ERROR! port=" << port << " servo=" << servo << " r=" << r << logs::end;

        logger().debug() << servo << " getTorque =" << r << logs::end;
        return r;

    }
}

bool ServoDriver::testIf(long value, long valeurMin, long valeurMax) {
    if ((value >= valeurMin) && (value <= valeurMax)) return true;
    else {
        logger().error() << "SERVO ID NOT EXISTS!! testIf()" << logs::end;
        return false;
    }
}

