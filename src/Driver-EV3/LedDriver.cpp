#include "LedDriver.hpp"

#include <unistd.h>
#include <string>

#include "../Log/Logger.hpp"
#include "../Common/Utils/json.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

ALedDriver * ALedDriver::create(std::string, int nb)
{
    return new LedDriver(nb);
}

LedDriver::LedDriver(int nb)
{
    nb_ = nb; //Force number of leds.
    float volts = power_supply::battery.measured_volts();
    float amps = power_supply::battery.measured_amps();

    //logger().error() << "===> BAT = " << "voltage is " << volts << logs::end;
    //logger().error() << "===> AMP = " << "Current is " << amps << logs::end;
    logger().debug() << "=> V=" << volts << " A=" << amps << logs::end;

    if (volts <= 7.6 && volts >= 7.4) {
        setBytes(0xFF, LED_ORANGE);
        logger().error() << "LOW VOLTAGE => V=" << volts << " A=" << amps << logs::end;
    }
    if (volts < 7.4) {
        setBytes(0xFF, LED_RED);
        logger().error() << "LOW VOLTAGE => V=" << volts << " A=" << amps << logs::end;
    }
    if (volts > 7.6) {
        setBytes(0xFF, LED_GREEN);
    }

}

LedDriver::~LedDriver()
{
}

void LedDriver::setBit(int index, LedColor color)
{

    if (index == 0) //Right
            {
        switch (color) {
        case LED_BLACK: //Off
            led::set_color(led::right, led::black);
            break;
        case LED_GREEN: //green
            led::set_color(led::right, led::green);
            break;
        case LED_RED: //red
            led::set_color(led::right, led::red);
            break;
        case LED_ORANGE: //orange
            led::set_color(led::right, led::orange);
            break;
        case LED_AMBER: //amber
            led::set_color(led::right, led::amber);
            break;
        case LED_YELLOW: //yellow
            led::set_color(led::right, led::yellow);
            break;
        case LED_OFF: //Off
        default:
            ev3dev::led::green_right.off();
            ev3dev::led::red_right.off();
            break;
        }
    }

    if (index == 1) //Left
            {
        switch (color) {
        case LED_BLACK: //Off
            led::set_color(led::left, led::black);
            break;
        case LED_GREEN: //green
            led::set_color(led::left, led::green);
            break;
        case LED_RED: //red
            led::set_color(led::left, led::red);
            break;
        case LED_ORANGE: //orange
            led::set_color(led::left, led::orange);
            break;
        case LED_AMBER: //amber
            led::set_color(led::left, led::amber);
            break;
        case LED_YELLOW: //yellow
            led::set_color(led::left, led::yellow);
            break;
        case LED_OFF: //Off
        default:
            ev3dev::led::green_left.off();
            ev3dev::led::red_left.off();
            break;
        }
    }

    //telemetry log
    nlohmann::json j;
    j["pos"] = index;
    j["color"] = color;
    logger().telemetry(j.dump());
}

void LedDriver::setBytes(uint hex, LedColor color)
{
    for (int i = 0; i < nb_; i++) {
        if (((hex >> i) & 0x01) == 1) {
            setBit(i, color);
        } else {
            setBit(i, LED_OFF);
        }
    }
}
