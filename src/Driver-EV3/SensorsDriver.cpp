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



//inX:i2c80:mux1
//inX:i2c81:mux2
//inX:i2c82:mux3
SensorsDriver::SensorsDriver() :
        IR_1("in3:i2c80:mux1")//, IR_2("in3:i2c81:mux2"), IR_3("in2:i2c82:mux3"), IR_4("in2:i2c81:mux2")
{

    /*
     //device d_ = device("in2:i2c80:mux1");
     //d_.set_attr_
     lego_port p = lego_port("in2:i2c80:mux1");

     int todo = 0;
     string temp = "nop";
     try {
     temp = p.get_file_string("in2:i2c80:mux1:lego-ev3-color/modalias");
     logger().info() << "exist driver temp ==  " << temp << logs::end;
     } catch (...) {
     }
     if (temp != "nop")
     todo = 1;

     if (todo == 0) {
     logger().info() << "port already set to lego-ev3-ir on " << p.address() << logs::end;
     } else {

     logger().info() << "set uart and device=lego-ev3-ir on " << p.address() << logs::end;
     p.set_mode("uart");

     p.set_set_device("lego-ev3-ir");
     usleep(400000);
     }

     ir1_ = infrared_sensor("in2:i2c80:mux1");


     if (ir1_.connected()) {
     logger().info() << ir1_.type_name() << " connected (device " << ir1_.driver_name() << ", port "
     << ir1_.address() << ", mode " << ir1_.mode() << ")" << logs::end;
     ir1_.proximity();
     } else {
     logger().error() << "ir1_ not Connected !!" << logs::end;
     }

     */

    //ir2_ = infrared_sensor(INPUT_2);
    /*
     if (ir2_.connected()) {
     logger().info() << ir2_.type_name() << " connected (device " << ir2_.driver_name() << ", port "
     << ir2_.address() << ", mode " << ir2_.mode() << ")" << logs::end;
     ir2_.proximity();
     } else {
     logger().error() << "ir2_ not Connected !!" << logs::end;
     }*/
    /*
     ir_ = infrared_sensor(INPUT_2);
     if (ir_.connected()) {
     logger().info() << ir_.type_name() << " connected (device " << ir_.driver_name() << ", port " << ir_.address()
     << ", mode " << ir_.mode() << ")" << logs::end;
     ir_.proximity();
     } else {
     logger().error() << "INPUT_2 (IR) not Connected !!" << logs::end;
     }

     irrear_ = infrared_sensor(INPUT_3);
     if (irrear_.connected()) {
     logger().info() << irrear_.type_name() << " connected (device " << irrear_.driver_name() << ", port "
     << irrear_.address() << ", mode " << irrear_.mode() << ")" << logs::end;
     irrear_.proximity();
     } else {
     logger().error() << "INPUT_3 (IR REAR) not Connected !!" << logs::end;
     }
     */
    /*
     us_ = ultrasonic_sensor();
     int value = 0;
     if (us_.connected())
     {
     logger().info() << us_.type_name()
     << " connected (device "
     << us_.driver_name()
     << ", port "
     << us_.address()
     << ", mode "
     << us_.mode()
     << ")"
     << logs::end;

     double mm = 0;
     value = us_.other_sensor_present(); //TODO

     logger().info() << "US other_sensor_present=" << value << logs::end;

     us_.distance_centimeters();

     }
     else
     {
     logger().error() << "INPUT_3 in3:i2c1 (US) not Connected !!" << logs::end;
     }*/
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{

    bool temp = 0;

    double mm1 = IR_1.getDistanceMM();
//    double mm2 = IR_2.getDistanceMM();
//    double mm3 = IR_3.getDistanceMM();
//    double mm4 = IR_4.getDistanceMM();
    logger().debug() << "IR_1=" << mm1 << logs::end;
//    logger().debug() << "IR_2=" << mm2 << logs::end;
//    logger().debug() << "IR_3=" << mm3 << logs::end;
//    logger().debug() << "IR_4=" << mm4 << logs::end;


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
