//drivers...SIMULAUNCHER

#include "SensorsDriver.hpp"

#include <string>

using namespace std;

ASensorsDriver * ASensorsDriver::create(std::string) {

    return new SensorsDriver();
}

SensorsDriver::SensorsDriver() {

}

SensorsDriver::~SensorsDriver() {
}

ASensorsDriver::bot_positions SensorsDriver::getvPositionsAdv() {
    ASensorsDriver::bot_positions pos;
    return pos;
}
int SensorsDriver::sync() {
    return 0;
}

int SensorsDriver::rightSide() {

}
int SensorsDriver::leftSide() {

}

int SensorsDriver::frontLeft() {

}
int SensorsDriver::frontCenter() {

}
int SensorsDriver::frontRight() {

}

int SensorsDriver::backLeft() {

}
int SensorsDriver::backCenter() {

}
int SensorsDriver::backRight() {

}
