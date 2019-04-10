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
        IR_1("ev3-ports:in3:i2c80:mux1"), IR_2("ev3-ports:in3:i2c81:mux2"), IR_3("ev3-ports:in3:i2c82:mux3") //, IR_4("ev3-ports:in2:i2c81:mux2")
{
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{

    bool temp = 0;

    double mm1 = IR_1.getDistanceMM();
    double mm2 = IR_2.getDistanceMM();
    double mm3 = IR_3.getDistanceMM();
//    double mm4 = IR_4.getDistanceMM();
    logger().info() << "IR_1==" << mm1 << "   IR_2==" << mm2 << "   IR_3==" << mm3 << logs::end;
//    logger().debug() << "IR_2=" << mm2 << logs::end;
//    logger().debug() << "IR_3=" << mm3 << logs::end;
//    logger().debug() << "IR_4=" << mm4 << logs::end;

    if (mm1 < 160 || mm2 < 160 || mm3 < 160)
        temp = 1;
    return temp;

}

bool SensorsDriver::rear()
{
    return 0;

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
