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
}

SensorsDriver::~SensorsDriver()
{
}

int SensorsDriver::left()
{
    int d = gp2_2_.getDistanceMm();
    return d;
}

int SensorsDriver::right()
{
    int d = gp2_1_.getDistanceMm();
    return d;
}

bool SensorsDriver::front() //TODO fusionner front et frontVeryClosed avec un retour de données en int
{

    bool front = false;

    if (1) {
        if (irLeft_.getDistance() < 200) {
            logger().debug() << "adc_8_left " << " mm=" << irLeft_.getDistance() << logs::end;
            front = true;
        }

        if (irRight_.getDistance() < 200) {
            logger().debug() << "adc_9_right " << "       mm=" << irRight_.getDistance() << logs::end;
            front = true;
        }

        if (irCenter_.getDistance() < 350) {
            logger().debug() << "adc_5_center " << "   mm=" << irCenter_.getDistance() << logs::end;
            front = true;
        }
    }
    return front;
}

bool SensorsDriver::frontVeryClosed()
{
    bool front = false;

    if (1) {
        if (irLeft_.getDistance() < 118) {
            logger().debug() << "adc_8_left " << " mm=" << irLeft_.getDistance() << logs::end;
            front = true;
        }

        if (irRight_.getDistance() < 118) {
            logger().debug() << "adc_9_right " << "       mm=" << irRight_.getDistance() << logs::end;
            front = true;
        }

        if (irCenter_.getDistance() < 240) {
            logger().debug() << "adc_5_center " << "   mm=" << irCenter_.getDistance() << logs::end;
            front = true;
        }
    }
    return front;
}

bool SensorsDriver::rear()
{

    bool rear = false;
    if (irRear_.getDistance() < 200) {
        logger().debug() << "adc_3_rear " << " mm=" << irRear_.getDistance() << logs::end;
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



