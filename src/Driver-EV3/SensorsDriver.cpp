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
SensorsDriver::SensorsDriver() :
        IR_1_AV_D("ev3-ports:in4:i2c80:mux1"), IR_2_AV_G("ev3-ports:in3:i2c80:mux1"), IR_3_AR_D(
                "ev3-ports:in4:i2c81:mux2"), IR_4_AR_G("ev3-ports:in3:i2c82:mux3")
{
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{
    int seuil = 250;
    bool temp = 0;

    double mm1 = IR_1_AV_D.getDistanceMM();
    double mm2 = IR_2_AV_G.getDistanceMM();

    logger().debug() << "IR_2_AV_G== " << mm2 << " \tIR_1_AV_D== " << mm1 << logs::end;

    if (mm1 < seuil || mm2 < seuil) {
        logger().info() << "=> IR_2_AV_G== " << mm2 << " \tIR_1_AV_D== " << mm1 << logs::end;
        temp = 1;
    }
    return temp;

}

bool SensorsDriver::rear()
{
    int seuil = 200;
    bool temp = 0;
    double mm3 = IR_3_AR_D.getDistanceMM();
    double mm4 = IR_4_AR_G.getDistanceMM();

    logger().debug() << "IR_4_AR_G== " << mm4 << " \tIR_3_AR_D== " << mm3 << logs::end;

    if (mm3 < seuil || mm4 < seuil) {
        logger().info() << "=> IR_4_AR_G== " << mm4 << " \tIR_3_AR_D== " << mm3 << logs::end;
        temp = 1;
    }
    return temp;

    /*
     double mm = us_.value() * 10.0;
     double mm2 = us_.value() * 10.0;
     mm = (mm + mm2) / 2.0;


     logger().info() << "rear mm=" << mm << logs::end;
     if (mm < 170)
     {
     logger().info() << "!! detected REAR mm=" << mm << logs::end;
     return 1;
     }
     else
     return 0;*/
}

bool SensorsDriver::frontVeryClosed()
{

}
bool SensorsDriver::rearVeryClosed()
{

}
