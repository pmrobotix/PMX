/*
 * LegoIRSensor.cpp
 *
 *  Created on: 24 mars 2019
 *      Author: pmx
 */

#include "LegoIRSensor.hpp"

#include <unistd.h>
#include <cstddef>
#include <string>

#include "../Log/Logger.hpp"

LegoIRSensor::LegoIRSensor(const char* input)
{
    logger().debug() << "LegoIRSensor::LegoIRSensor()" << logs::end;

    //cas du mux pour IR_PROX
    std::size_t found = string(input).find("mux");
    logger().debug() << "found=" << found << " std::string::npos=" << std::string::npos << logs::end;

    //Dans le cas d'un MUX
    if (found != std::string::npos) {
        logger().debug() << "LegoIRSensor has to be set to lego-ev3-ir" << logs::end;
        lego_port p = lego_port(input);

        logger().debug() << "p.connected()=" << p.connected() << logs::end;

        //Dans le cas d'un MUX connecté
        if (p.connected()) {

            int todo = 0;
            string temp = "nop";
            //usleep(1000);
            try {

                temp = p.get_attr_from_set(string(input + string(":lego-ev3-color/modalias")));

                logger().debug() << "exist driver temp ==  " << temp << logs::end;
            } catch (...) {
                //logger().error() << "LegoIRSensor impossible to access to " << temp << logs::end;
            }
            if (temp != "nop")
                todo = 1;

            if (todo == 0) {
                logger().info() << "port already set to lego-ev3-ir on " << p.address() << logs::end;
            } else {

                logger().debug() << "set uart and device=lego-ev3-ir on " << p.address() << logs::end;
                //usleep(10000);
                p.set_mode("uart");
                usleep(1000);
                p.set_set_device("lego-ev3-ir");
                usleep(1000);
            }
        }
    }

    logger().debug() << "ir_ = infrared_sensor(input)" << logs::end;
    ir_ = infrared_sensor(input);

    if (ir_.connected()) {
        logger().info() << ir_.type_name() << " connected (device " << ir_.driver_name() << ", port " << ir_.address()
                << ", mode " << ir_.mode() << ")" << logs::end;
        //ir_.proximity(true); //BUG de permission à Corriger
        ir_.proximity(false);
    } else {
        logger().error() << input << " ir_ not Connected !!" << logs::end;
    }
}

//std::string LegoIRSensor::get_attr_from_set(const std::string &name)
//{
//    using namespace std;
//
//    string s = get_attr_line(name);
//
//    size_t pos, last_pos = 0;
//    string t;
//    do {
//        pos = s.find(' ', last_pos);
//
//        if (pos != string::npos) {
//            t = s.substr(last_pos, pos - last_pos);
//            last_pos = pos + 1;
//        } else
//            t = s.substr(last_pos);
//
//        if (!t.empty()) {
//            if (*t.begin() == '[') {
//                return t.substr(1, t.length() - 2);
//            }
//        }
//    } while (pos != string::npos);
//
//    return {"none"};
//}

double LegoIRSensor::getDistanceMM()
{
// as a percentage. 100% is approximately 70cm/27in.
    if (ir_.connected()) {
        double percent = ir_.value();
//        usleep(30000);
//        double percent2 = ir_.value();
//        logger().info() << " percent=" << percent << " percent2=" << percent2 << logs::end;
//        percent = (percent + percent2) / 2.0;

        double distance_mm = percent * 6.0;

        return distance_mm;
    } else {
        return -1;
    }

}
