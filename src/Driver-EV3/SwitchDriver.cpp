//drivers...EV3

#include "SwitchDriver.hpp"

#include <unistd.h>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;
using namespace ev3dev;

ASwitchDriver * ASwitchDriver::create(std::string botName)
{
    static SwitchDriver *instance = new SwitchDriver();
    return instance;
}

SwitchDriver::SwitchDriver()
{

    logger().debug() << "TOUCH has to be set to lego-ev3-touch" << logs::end;
    lego_port p = lego_port("ev3-ports:in3:i2c81:mux2");

    logger().debug() << "p.connected()=" << p.connected() << logs::end;

    //Dans le cas d'un MUX connecté
    if (p.connected()) {

        int todo = 0;
        string temp = "nop";
        //usleep(10000);
        try {
            //temp = p.get_attr_from_set()string(input + string(":lego-ev3-color/modalias"));
            temp = p.driver_name();
            logger().debug() << "exist driver temp ==  " << temp << logs::end;
        } catch (...) {
            logger().error() << "SwitchDriver()  access to " << temp << logs::end;
        }
        if (temp != "lego-ev3-touch")
            todo = 1;

        if (todo == 0) {
            logger().debug() << "port already set to lego-ev3-touch on " << p.address() << logs::end;
        } else {

            logger().debug() << "set uart and device=lego-ev3-touch on " << p.address() << logs::end;
            //usleep(10000);

            p.set_mode("analog"); //TOUCH ev3-analog
            //usleep(10000);
            p.set_set_device("lego-ev3-touch");
            usleep(10000);
        }
    }

    touch_ = touch_sensor("ev3-ports:in3:i2c81:mux2");

    //cas NON MUX
    //touch_ = touch_sensor(INPUT_AUTO);

    if (touch_.connected()) {
        logger().debug() << touch_.type_name() << " connected (device " << touch_.driver_name() << ", port "
                << touch_.address() << ", mode " << touch_.mode() << ")" << logs::end;
    } else {
        logger().error() << "INPUT_1 (Tirette) not Connected !!" << logs::end;
    }
}

SwitchDriver::~SwitchDriver()
{
}

int SwitchDriver::tirettePressed()
{
    if (touch_.value() == 257) //in case of MUX
        return true;
    else
        return false;
}

int SwitchDriver::backLeftPressed()
{
    return 0;
}
int SwitchDriver::backRightPressed()
{
    return 0;
}

int SwitchDriver::pressed(unsigned char pin)
{
    return 0;
}
