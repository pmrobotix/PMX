//drivers...OPOS

#include "DynamixelDriver.hpp"

#include "../Log/Logger.hpp"

using namespace std;

DynamixelDriver::DynamixelDriver() :
        connected_(0)
{
    logger().debug() << "ServoDriver::ServoDriver() CCAx12Adc::instance().begin();" << logs::end;

    connected_ = CCAx12Adc::instance().begin();

    //logger().error() << "connected=" << connected_ << logs::end;
    if (connected_) {
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

    }
}

void DynamixelDriver::hold(int servo)
{

    int err = CCAx12Adc::instance().writeAXData(servo, P_TORQUE_ENABLE, 1);
    logger().info() << "hold enableax=" << err << logs::end;
    /*
     int swap = 0;
     for (int n = 0; n < 5; n++)
     {

     if (swap)
     {
     //usleep(100000);
     CCAx12Adc::instance().setLedOn(2);
     }
     else
     CCAx12Adc::instance().setLedOff(2);

     swap = !swap;

     int ping4 = CCAx12Adc::instance().pingAX(4);
     logger().info() << "ping4=" << ping4 << logs::end;
     int ping6 = CCAx12Adc::instance().pingAX(6);
     logger().info() << "ping6=" << ping6 << logs::end;

     int readax4 = CCAx12Adc::instance().readAXData(4, P_ID);
     logger().info() << "readax4 P_ID =" << readax4 << logs::end;
     int readax6 = CCAx12Adc::instance().readAXData(6, P_ID);
     logger().info() << "readax6 P_ID =" << readax6 << logs::end;

     int posax4 = CCAx12Adc::instance().readAXData(4, P_PRESENT_POSITION);
     logger().info() << "posax4 P_PRESENT_POSITION=" << posax4 << logs::end;
     int posax6 = CCAx12Adc::instance().readAXData(6, P_PRESENT_POSITION);
     logger().info() << "posax6 P_PRESENT_POSITION=" << posax6 << logs::end;

     int writeposax = CCAx12Adc::instance().writeAXData(4, P_GOAL_POSITION, 700);
     logger().info() << "writeposax4 P_GOAL_POSITION =" << writeposax << logs::end;

     int err = CCAx12Adc::instance().writeAXData(6, P_TORQUE_ENABLE, 1);
     logger().info() << "torque6 enableax=" << err << logs::end;

     int writeposax6 = CCAx12Adc::instance().writeAXData(6, P_GOAL_POSITION, 700);
     logger().info() << "writeposax6 P_GOAL_POSITION 700 =" << writeposax6 << logs::end;

     int torqueax4 = CCAx12Adc::instance().writeAXData(4, P_TORQUE_ENABLE, 0);
     logger().info() << "torque4 disableax=" << torqueax4 << logs::end;
     int torqueax6 = CCAx12Adc::instance().writeAXData(6, P_TORQUE_ENABLE, 0);
     logger().info() << "torque6 disableax=" << torqueax6 << logs::end;

     logger().info() << "" << logs::end;

     }
     */

}

void DynamixelDriver::setPosition(int servo, int pos)
{
    if (pos >= 1023)
        pos = 1023;
    if (pos <= 0)
        pos = 0;

    int r = CCAx12Adc::instance().writeAXData(servo, P_GOAL_POSITION, pos);
    logger().debug() << "setPosition P_GOAL_POSITION =" << r << logs::end;

}

void DynamixelDriver::turn(int servo, int speed)
{
    if (speed >= 1023)
        speed = 1023;
    if (speed <= 0)
        speed = 0;

    int rcw = CCAx12Adc::instance().writeAXData(servo, P_CW_ANGLE_LIMIT, 0);
    int rccw = CCAx12Adc::instance().writeAXData(servo, P_CCW_ANGLE_LIMIT, 0);
    int r = CCAx12Adc::instance().writeAXData(servo, P_GOAL_SPEED, speed);
    logger().debug() << "setPosition P_GOAL_SPEED =" << r << logs::end;
    logger().debug() << "setPosition P_GOAL_POSITION =" << r << logs::end;

}

void DynamixelDriver::release(int servo)
{
    int r = CCAx12Adc::instance().writeAXData(servo, P_TORQUE_ENABLE, 0);
    logger().debug() << "release servo=" << servo << " r=" << r << logs::end;
}

void DynamixelDriver::setRate(int servo, int speed)
{
    if (speed >= 1023)
        speed = 1023;
    if (speed <= 0)
        speed = 0;

    int r = CCAx12Adc::instance().writeAXData(servo, P_GOAL_SPEED, speed);
    logger().debug() << "setPosition P_GOAL_SPEED =" << r << logs::end;

}
//return 1 when moving
int DynamixelDriver::getMoving(int servo)
{
    int r = CCAx12Adc::instance().readAXData(servo, P_MOVING);
    logger().debug() << "getMoving P_MOVING =" << r << logs::end;

    return r;
}

int DynamixelDriver::getPos(int servo)
{
    int r = CCAx12Adc::instance().readAXData(servo, P_PRESENT_POSITION);
    return r;
}

int DynamixelDriver::ping(int servo)
{
    int r = CCAx12Adc::instance().pingAX(servo);
    return r;
}

