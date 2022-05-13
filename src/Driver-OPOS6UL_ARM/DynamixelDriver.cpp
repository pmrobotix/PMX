//drivers...OPOS

#include "DynamixelDriver.hpp"

#include <unistd.h>

#include "../Log/Logger.hpp"

using namespace std;

DynamixelDriver::DynamixelDriver() :
        connected_(0)
{
    logger().debug() << "ServoDriver::ServoDriver() CCAx12Adc::instance().begin();" << logs::end;

    connected_ = CCAx12Adc::instance().begin();

    //logger().error() << "connected=" << connected_ << logs::end;
    if (connected_) {
        /*
         CCAx12Adc::instance().setLedOn(3);
         CCAx12Adc::instance().setLedOn(4);
         CCAx12Adc::instance().setLedOn(5);

         int ping5 = CCAx12Adc::instance().pingAX(5);
         if (ping5 < 0)
         logger().error() << "ping5=" << ping5 << logs::end;
         int ping7 = CCAx12Adc::instance().pingAX(7);
         if (ping7 < 0)
         logger().error() << "ping7=" << ping7 << logs::end;
         int ping51 = CCAx12Adc::instance().pingAX(51);
         if (ping51 < 0)
         logger().error() << "ping51=" << ping51 << logs::end;
         */
    }
}

void DynamixelDriver::setType(int servo, ServoType type)
{

}


void DynamixelDriver::hold(int servo) {
    int err = CCAx12Adc::instance().writeAXData(servo, P_TORQUE_ENABLE, 1);
    logger().info() << "hold enableax=" << err << logs::end;
}

void DynamixelDriver::setPulsePos(int servo, int pulsewidth) {
//    if (pos >= 1023)
//        pos = 1023;
//    if (pos <= 0)
//        pos = 0;

    int r = CCAx12Adc::instance().writeAXData(servo, P_GOAL_POSITION, constrain(pulsewidth, 0, 4095));
    logger().debug() << "setPosition P_GOAL_POSITION =" << r << logs::end;

}

void DynamixelDriver::turn(int servo, int speed) {
//    if (speed >= 1023)
//        speed = 1023;
//    if (speed <= 0)
//        speed = 0;

//int rcw = CCAx12Adc::instance().writeAXData(servo, P_CW_ANGLE_LIMIT, 0);
//int rccw = CCAx12Adc::instance().writeAXData(servo, P_CCW_ANGLE_LIMIT, 0);
    int r = CCAx12Adc::instance().writeAXData(servo, P_GOAL_SPEED, constrain(speed, 0, 1023));
    logger().debug() << "setPosition P_GOAL_SPEED =" << r << logs::end;
    //logger().debug() << "setPosition P_GOAL_POSITION =" << r << logs::end;

}

void DynamixelDriver::release(int servo) {
    int r = CCAx12Adc::instance().writeAXData(servo, P_TORQUE_ENABLE, 0);
    logger().debug() << "release servo=" << servo << " r=" << r << logs::end;
}

void DynamixelDriver::setRate(int servo, int speed) {
//    if (speed >= 1023)
//        speed = 1023;
//    if (speed <= 0)
//        speed = 0;

    int r = CCAx12Adc::instance().writeAXData(servo, P_TORQUE_LIMIT, constrain(speed, 0, 1023));
    logger().debug() << "setPosition P_TORQUE_LIMIT =" << r << logs::end;

}
//return 1 when in progress, 0 when completed
// return -1 on error
int DynamixelDriver::getMoving(int servo) {
    int r = CCAx12Adc::instance().readAXData(servo, P_MOVING);
    if (r < 0) {
        logger().error() << "getMoving error on AX[" << servo << "] err =" << r << logs::end;
    }
    if (r == 252) {
        logger().error() << "getMoving error : CRC error on AX[" << servo << "] bus " << logs::end;
        return -1;
    }
    logger().debug() << "getMoving on AX[" << servo << "] = " << r << logs::end;

    return r;
}

int DynamixelDriver::getPulsePos(int servo) {
    int r = CCAx12Adc::instance().readAXData(servo, P_PRESENT_POSITION);
    return r;
}

int DynamixelDriver::ping(int servo) {
    int r = CCAx12Adc::instance().pingAX(servo);
//    if (servo == 51) {
//        CCAx12Adc::instance().setLedOn(1);
//        CCAx12Adc::instance().setLedOff(2);
//        CCAx12Adc::instance().setLedOn(3);
//        CCAx12Adc::instance().setLedOff(4);
//        CCAx12Adc::instance().setLedOn(5);
//        CCAx12Adc::instance().setLedOff(6);
//        CCAx12Adc::instance().setLedOn(7);
//    }
    return r;
}

void DynamixelDriver::setMinPulse(int servo, int pulse) {

}

void DynamixelDriver::setMidPulse(int servo, int pulse) {

}

void DynamixelDriver::setMaxPulse(int servo, int pulse) {

}

void DynamixelDriver::setPolarity(int servo, bool inversed) {

}

int DynamixelDriver::getTorque(int servo) {
    int r = CCAx12Adc::instance().readAXData(servo, P_PRESENT_LOAD);
    logger().error() << servo << " getTorque =" << r << logs::end;
    if (r >= 1024) {
        r = (r - 1024); //a multiplier par -1 si on veut le sens
    }
//    if (r>1023)
//        return 0;
    return r;
}
