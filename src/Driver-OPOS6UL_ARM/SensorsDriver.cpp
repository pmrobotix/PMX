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
        gp2_1_(1, ADDRESS_gp2y0e02b), gp2_2_(0, ADDRESS_gp2y0e02b), connected_gp2y0e02b_(false), shift_(0),
        irLeft_(8, 30),
        irCenter_(5, 80),
        irRight_(9, 30),
        irRear_(3, 30)
{
}

SensorsDriver::~SensorsDriver()
{
}
int SensorsDriver::leftSide()
{
    int d = gp2_2_.getDistanceMm();
    return d;
}
int SensorsDriver::rightSide()
{
    int d = gp2_1_.getDistanceMm();
    return d;
}

int SensorsDriver::getFrontDistMmFromObject(int diagonal_dist_mm)
{
    int dist_from_wheel_axe_mm = 155;
    float cos_alpha = 45.0/48.0;
    int lmm = diagonal_dist_mm * cos_alpha + dist_from_wheel_axe_mm;
    return lmm;
}

int SensorsDriver::getBackDistMmFromObject(int diagonal_dist_mm)
{
    int dist_from_wheel_axe_mm = 70;
    float cos_alpha = 42.0/48.0;
    int lmm = diagonal_dist_mm * cos_alpha + dist_from_wheel_axe_mm;
    return lmm;
}

int SensorsDriver::frontLeft()
{
    return getFrontDistMmFromObject(irLeft_.getDistanceMm());
}

int SensorsDriver::frontCenter()
{
    return irCenter_.getDistanceMm();
}
int SensorsDriver::frontRight()
{
    return getFrontDistMmFromObject(irRight_.getDistanceMm());
}

int SensorsDriver::backLeft()
{
    return -1;
}
int SensorsDriver::backCenter()
{
    return getBackDistMmFromObject(irRear_.getDistanceMm());
}
int SensorsDriver::backRight()
{
    return -1;
}
