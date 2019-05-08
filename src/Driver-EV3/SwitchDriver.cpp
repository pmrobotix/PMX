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
    string input = "ev3-ports:in3:i2c81:mux2";

    logger().debug() << "TOUCH MUX has to be set to lego-ev3-touch" << logs::end;
    lego_port p = lego_port(input);

    logger().debug() << "p.connected()=" << p.connected() << logs::end;

    //Dans le cas d'un MUX connecté
    if (p.connected()) {

        int todo = 0;
        string temp = "nop";

        try {
            //temp = p.get_attr_string("ev3-ports:in3:i2c81:mux2:lego-ev3-color/modalias");
            temp = p.get_attr_string(string(input + string(":lego-ev3-color/modalias")));

            logger().debug() << "exist driver temp ==  " << temp << logs::end;
        } catch (...) {
            //logger().error() << "SwitchDriver()  access to " << temp << logs::end;
        }

        if (temp != "nop")
            todo = 1;

        if (todo == 0) {
            logger().debug() << "port already set to lego-ev3-touch on " << p.address() << logs::end;
        } else {

            logger().debug() << "set uart and device=lego-ev3-touch on " << p.address() << logs::end;

            p.set_mode("analog"); //TOUCH ev3-analog
            usleep(10000);
            p.set_set_device("lego-ev3-touch");
            usleep(500000);

            //verif ev3-ports:in3:i2c81:mux2:lego-ev3-touch
            temp = p.get_attr_string(string(input + string(":lego-ev3-touch/modalias")));
            logger().debug() << "vERIF driver lego-ev3-touch temp ==  " << temp << logs::end;
        }
    }

    touch_ = touch_sensor(input);

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
