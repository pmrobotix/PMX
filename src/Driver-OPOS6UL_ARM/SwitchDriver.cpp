//drivers...ARM

#include "SwitchDriver.hpp"

#include <unistd.h>

#include "../Log/Logger.hpp"
#include "GpioPCA9555.hpp"

using namespace std;

ASwitchDriver * ASwitchDriver::create(std::string)
{
    return new SwitchDriver();
}

SwitchDriver::SwitchDriver()
{

    bool c = GpioPCA9555::instance().begin();
    if (c) {
        //logger().debug() << "GpioPCA9555 CONNECTED !" << logs::end;
        GpioPCA9555::instance().setOnP0(0); //set up PIN0
    } else
        logger().debug() << "SwitchDriver() : GpioPCA9555 NOT CONNECTED !" << logs::end;
}

SwitchDriver::~SwitchDriver()
{
}

bool SwitchDriver::is_connected()
{
    return GpioPCA9555::instance().isConnected();
}

int SwitchDriver::pressed(unsigned char pin)
{
    return GpioPCA9555::instance().getValueP1(pin);
}


int SwitchDriver::tirettePressed()
{
    //utils::sleep_for_micros(30000);
//    logger().error() << logs::end;
//    logger().error() << "pressed(7)= " << pressed(7)<< logs::end;
//    logger().error() << "pressed(6)= " << pressed(6)<< logs::end;
//    logger().error() << "pressed(5)= " << pressed(5)<< logs::end;
//    logger().error() << "pressed(4)= " << pressed(4)<< logs::end;
//    logger().error() << "pressed(3)= " << pressed(3)<< logs::end;
//    logger().error() << "pressed(2)= " << pressed(2)<< logs::end;
//    logger().error() << "pressed(1)= " << pressed(1)<< logs::end;
//    logger().error() << "pressed(0)= " << pressed(0)<< logs::end;



    return pressed(7);
}

int SwitchDriver::backLeftPressed()
{
    return pressed(0);
}
int SwitchDriver::backRightPressed()
{
    return pressed(1);
}
void SwitchDriver::setGPIO(int gpio, bool activate)
{
    if (activate)
        GpioPCA9555::instance().setOnP0(gpio);
    else
        GpioPCA9555::instance().setOffP0(gpio);
}

