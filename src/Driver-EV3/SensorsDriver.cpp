//drivers...EV3

#include "SensorsDriver.hpp"

#include <unistd.h>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string botName)
{
    return new SensorsDriver();
}

//ev3-ports:inX:i2c80:mux1
//ev3-ports:inX:i2c81:mux2
//ev3-ports:inX:i2c82:mux3
SensorsDriver::SensorsDriver() :beacon_(1, 0x2D)
//        IR_1_AV_D("ev3-ports:in4:i2c80:mux1"), IR_2_AV_G("ev3-ports:in3:i2c80:mux1"), IR_3_AR_D(
//                "ev3-ports:in4:i2c81:mux2"), IR_4_AR_G("ev3-ports:in3:i2c82:mux3")
{

//    int connected = beacon_.connect();
//    logger().debug() << "SensorsDriver()... " << logs::end;
//    beacon_.ScanBus();

}

SensorsDriver::~SensorsDriver()
{
}


ASensorsDriver::bot_positions SensorsDriver::getvPositionsAdv() {
    ASensorsDriver::bot_positions pos;
    return pos;
}
int SensorsDriver::sync() {
    return 0;
}


int SensorsDriver::rightSide()
{
    return -1;
}
int SensorsDriver::leftSide()
{
    return -1;
}

int SensorsDriver::frontLeft()
{
//    float mm1 = IR_2_AV_G.getDistanceMM();
//
//    //real distance on the ground
//    mm1 = (mm1 * 0.75) + 155; // cos a = 400/500 = 0.8
//
//    return mm1;
}
int SensorsDriver::frontCenter()
{
    return -1;
}
int SensorsDriver::frontRight()
{
//    float mm1 = IR_1_AV_D.getDistanceMM();
//    //real distance on the ground
//    mm1 = (mm1 * 0.75) + 145; // cos a = 400/500 = 0.8
//
//    return mm1;

}

int SensorsDriver::backLeft()
{
//    float mm1 = IR_4_AR_G.getDistanceMM();
//    //real distance on the ground
//    mm1 = (mm1 * 0.85) + 45; // cos a = 400/500 = 0.8
//    return mm1;

}
int SensorsDriver::backCenter()
{
    return -1;
}
int SensorsDriver::backRight()
{
//    float mm1 = IR_3_AR_D.getDistanceMM();
//    //real distance on the ground
//    mm1 = (mm1 * 0.75) + 45; // cos a = 400/500 = 0.8
//    return mm1;

}
