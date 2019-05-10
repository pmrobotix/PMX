//drivers...OPO

#include "SensorsDriver.hpp"

#include <cmath>
#include <string>

#include "../Log/Logger.hpp"
#include "CCAx12Adc.hpp"

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string)
{
    return new SensorsDriver();
}

SensorsDriver::SensorsDriver() :
        gp2_1_(1, ADDRESS_gp2y0e02b), gp2_2_(0, ADDRESS_gp2y0e02b), connected_gp2y0e02b_(false), shift_(0), irLeft_(8,
                30), irCenter_(5, 150), irRight_(9, 30), irRear_(3, 30)
{
    /*
     i2c_gp2y0e02b_.setSlaveAddr(ADDRESS_gp2y0e02b);

     //read shift
     shift_ = read_i2c(SHIFT_gp2y0e02b);

     if (shift_ <= 0) {
     logger().error() << "SensorsDriver() : gp2y0e02b NOT CONNECTED !" << logs::end;
     connected_gp2y0e02b_ = false;
     } else
     connected_gp2y0e02b_ = true;
     */

    //exit(0);
}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::front()
{

    bool front = false;

    if (1) {
        if (irLeft_.getDistance() < 200) {
            logger().info() << "adc_8_left " << " mm=" << irLeft_.getDistance() << logs::end;
            front = true;
        }

        if (irRight_.getDistance() < 200) {
            logger().info() << "adc_9_right " << "       mm=" << irRight_.getDistance() << logs::end;
            front = true;
        }

        if (irCenter_.getDistance() < 350) {
            logger().info() << "adc_5_center " << "   mm=" << irCenter_.getDistance() << logs::end;
            front = true;
        }

    }
    /*
     if (connected_gp2y0e02b_) {
     //read distance
     int d = read_i2c_2bytes_optimised(DISTANCE_REG_gp2y0e02b);
     if (d < 0) {
     logger().error() << "ERROR : front() : read_i2c_2bytes neg!" << logs::end;
     } else {
     logger().debug() << "gp2y0e02b dist=" << d << logs::end;
     if (d < 250)
     front = true;
     }
     }*/

    return front;
}

bool SensorsDriver::frontVeryClosed()
{
    bool front = false;

    if (1) {
        if (irLeft_.getDistance() < 105) {
            logger().info() << "adc_8_left " << " mm=" << irLeft_.getDistance() << logs::end;
            front = true;
        }

        if (irRight_.getDistance() < 105) {
            logger().info() << "adc_9_right " << "       mm=" << irRight_.getDistance() << logs::end;
            front = true;
        }

        if (irCenter_.getDistance() < 240) {
            logger().info() << "adc_5_center " << "   mm=" << irCenter_.getDistance() << logs::end;
            front = true;
        }

    }
    return front;
}

bool SensorsDriver::rear()
{

    bool rear = false;
    if (irRear_.getDistance() < 200) {
        logger().info() << "adc_3_rear " << " mm=" << irRear_.getDistance() << logs::end;
        rear = true;
    }
    return rear;
}

bool SensorsDriver::rearVeryClosed()
{

    bool rear = false;
    if (irRear_.getDistance() < 150) {
        logger().info() << "adc_3_rear " << " mm=" << irRear_.getDistance() << logs::end;
        rear = true;
    }
    return rear;
}

bool SensorsDriver::gp2()
{
//read distance
    int d = gp2_1_.getDistanceMm();
    if (d < 0) {
        //logger().error() << "ERROR : front() : getDistanceMm neg!" << logs::end;
    } else {
        logger().debug() << "gp2y0e02b 1 dist=" << d << logs::end;
//        if (d < 250)
//            front = true;
    }

    d = gp2_2_.getDistanceMm();
    if (d < 0) {
        //logger().error() << "ERROR : front() : getDistanceMm neg!" << logs::end;
    } else {
        logger().debug() << "gp2y0e02b 2 dist=" << d << logs::end;
//        if (d < 250)
//            front = true;
    }

    //TODO return
}

/*
 long SensorsDriver::write_i2c(unsigned char command, unsigned char value)
 {
 return i2c_gp2y0e02b_.writeRegByte(command, value);
 }*/
/*
 int SensorsDriver::read_i2c(unsigned char command)
 {
 return i2c_gp2y0e02b_.readRegByte(command);
 }

 int SensorsDriver::read_i2c_2bytes_optimised(unsigned char command)
 {
 unsigned char data[2];
 long r = i2c_gp2y0e02b_.readReg(command, data, 2);
 if (r < 0)
 logger().error() << "ERROR read_i2c_2bytes_optimised() " << logs::end;
 unsigned char high = data[0];
 unsigned char low = data[1];

 logger().error() << "high: " << (int) high << " \tlow: " << (int) low << logs::end;
 int distance = ((high * 16 + low) / 16) * 10 / (int) std::pow(2, shift_); // Calculate the range in CM

 return distance;
 }

 int SensorsDriver::read_i2c_2bytes(unsigned char command)
 {
 if (!connected_gp2y0e02b_)
 return -1;
 unsigned char low = 0;
 unsigned char high = 0;

 high = read_i2c(command);
 low = read_i2c(command + 1);

 logger().error() << "low: " << (int) low << " \thigh: " << (int) high << logs::end;
 int distance = ((high * 16 + low) / 16) / (int) std::pow(2, shift_); // Calculate the range in CM
 logger().error() << "dist: " << distance << logs::end;
 return distance;
 }*/

