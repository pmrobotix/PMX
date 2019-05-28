//drivers...OPOS

#include "AsservDriver.hpp"

#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cmath>

#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"

using namespace std;

//TODO change float2bytes_t to int32 converter
/*
 For readability and simplicity, why don't we use a union?

 union converter {
 char    c[4];
 int32_t i;
 }
 Now, to convert, it's as simple as this:

 union converter conv;
 conv.i = yourInt32Value;
 char *cString = conv.c;
 or

 union converter conv;
 conv.c = yourCStringValue;
 int32_t i = conv.i;
 Remember to pay attention to your endianness, however.

 union converter {
 char    c[4];
 int32_t i;
 };

 int main(int argc, const char * argv[]) {
 union converter conv;
 conv.c[0] = 0xFF;
 conv.c[1] = 0xEE;
 conv.c[2] = 0xDD;
 conv.c[3] = 0xCC;

 std::cout << std::hex << conv.i << std::endl;

 return 0;
 }*/

AAsservDriver * AAsservDriver::create(std::string)
{
    static AsservDriver *instance = new AsservDriver();
    return instance;
}

AsservDriver::AsservDriver() :
        mbedI2c_(0) //OPOS6UL_UART5=>1 ; OPOS6UL_UART4=>0
                , connected_(false), asservMbedStarted_(false), pathStatus_(TRAJ_OK), p_( { 0.0, 0.0, 0.0, -1 })
{
    errorCount_ = 0;
    if (mbedI2c_.setSlaveAddr(MBED_ADDRESS) < 0) //0xAA>>1 = 0x55
            {

        logger().error() << "AsservDriver() : ERROR setSlaveAddr !" << logs::end;
        connected_ = false;
    } else {

        //on demarre le check de positionnement...
        this->start("AsservDriver::AsservDriver()");
        connected_ = true;
    }
}

AsservDriver::~AsservDriver()
{
}

void AsservDriver::execute()
{
    int periodTime_us = 50000; //10ms minimum sans le debug MBED activé
    utils::Chronometer chrono("AsservDriver::execute()");
    chrono.setTimer(periodTime_us);

    while (1) {
        if (asservMbedStarted_) {

            odo_GetPosition();

        }
        chrono.waitTimer();
    }
}

void AsservDriver::setMotorLeftPosition(int power, long ticks)
{
    //TODO
}

void AsservDriver::setMotorRightPosition(int power, long ticks)
{
    //TODO
}

void AsservDriver::setMotorLeftPower(int power, int timems)
{
    //TODO
}

void AsservDriver::setMotorRightPower(int power, int timems)
{
    //TODO
}

long AsservDriver::getLeftExternalEncoder()
{
    return 0;
}
long AsservDriver::getRightExternalEncoder()
{
    return 0;
}

long AsservDriver::getLeftInternalEncoder()
{
    return 0;
}
long AsservDriver::getRightInternalEncoder()
{
    return 0;
}

void AsservDriver::resetEncoders()
{
    //TODO
}

void AsservDriver::resetInternalEncoders()
{
    //TODO
}
void AsservDriver::resetExternalEncoders()
{
    //TODO
}

void AsservDriver::stopMotorLeft()
{
    //TODO
}
void AsservDriver::stopMotorRight()
{
    //TODO
}

int AsservDriver::getMotorLeftCurrent()
{
    return 0;
}
int AsservDriver::getMotorRightCurrent()
{
    return 0;
}

void AsservDriver::odo_SetPosition(float x_m, float y_m, float angle_rad)
{
    if (!connected_)
        return;
    //P12
    unsigned char d[12];
    float2bytes_t x_mm;
    x_mm.f = x_m * 1000.0f;
    float2bytes_t y_mm;
    y_mm.f = y_m * 1000.0f;
    float2bytes_t a;
    a.f = angle_rad;

    d[0] = x_mm.b[0];
    d[1] = x_mm.b[1];
    d[2] = x_mm.b[2];
    d[3] = x_mm.b[3];

    d[4] = y_mm.b[0];
    d[5] = y_mm.b[1];
    d[6] = y_mm.b[2];
    d[7] = y_mm.b[3];

    d[8] = a.b[0];
    d[9] = a.b[1];
    d[10] = a.b[2];
    d[11] = a.b[3];
    if (int r = mbed_writeI2c('S', 12, d) < 0) {
        logger().error() << "odo_SetPosition - S12 - ERROR " << r << logs::end;
    } else {
        logger().info() << "odo_SetPosition S12 " << x_mm.f << " " << y_mm.f << " " << a.f << logs::end;

        odo_GetPosition();
    }
}
RobotPosition AsservDriver::odo_GetPosition() //en metre
{
    m_pos.lock();
    p_ = mbed_GetPosition();
    m_pos.unlock();
    return p_;
}

RobotPosition AsservDriver::mbed_GetPosition() //en metre
{
    RobotPosition p;
    p.x = -1;
    p.y = -1;
    p.theta = -1;
    if (!connected_)
        return p;

    int status = -1;

    unsigned char data[13];

    if (int r = mbed_readI2c('p', 13, data) < 0) {
        logger().error() << "mbed_GetPosition - p13 - ERROR " << r << logs::end;
        errorCount_++;
        if (errorCount_ > 20) {
            logger().error() << "mbed_GetPosition Too many Error ==> EXIT !!! " << r << logs::end;
            exit(1);
        }
        return p;
    } else {
        //printf("read %d %d %d %d\n", data[0], data[1], data[2], data[3]);
        float2bytes_t x_mm;
        x_mm.b[0] = data[0];
        x_mm.b[1] = data[1];
        x_mm.b[2] = data[2];
        x_mm.b[3] = data[3];

        float2bytes_t y_mm;
        y_mm.b[0] = data[4];
        y_mm.b[1] = data[5];
        y_mm.b[2] = data[6];
        y_mm.b[3] = data[7];

        float2bytes_t rad;
        rad.b[0] = data[8];
        rad.b[1] = data[9];
        rad.b[2] = data[10];
        rad.b[3] = data[11];

        status = data[12];

        logger().debug() << "mbed_GetPosition p13 " << x_mm.f << " " << y_mm.f << " " << rad.f << " " << status
                << logs::end;

        RobotPosition p; //in m
        p.x = x_mm.f / 1000.0;
        p.y = y_mm.f / 1000.0;
        p.theta = rad.f;
        p.asservStatus = status;

        return p;
    }
}

int AsservDriver::path_GetLastCommandStatus()
{
    //TODO deprecated ?
    return -1;
}
void AsservDriver::path_InterruptTrajectory()
{
    if (!connected_)
        return;
    if (!asservMbedStarted_)
        logger().debug() << "path_InterruptTrajectory() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
    else {
        mbed_writeI2c('h', 0, NULL);
        pathStatus_ = TRAJ_INTERRUPTED;
        usleep(300000);
        mbed_writeI2c('r', 0, NULL); //reset de l'arret d'urgence
    }
}
void AsservDriver::path_CollisionOnTrajectory()
{
    if (!connected_)
        return;
    if (!asservMbedStarted_)
        logger().info() << "path_CollisionOnTrajectory() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
    else {
        //logger().info() << "path_CollisionOnTrajectory() HALT " << asservMbedStarted_ << logs::end;
        mbed_writeI2c('h', 0, NULL);
        pathStatus_ = TRAJ_NEAR_OBSTACLE;
        usleep(300000);
        mbed_writeI2c('r', 0, NULL); //reset de l'arret d'urgence
    }
}
void AsservDriver::path_CollisionRearOnTrajectory()
{
    if (!connected_)
        return;
    if (!asservMbedStarted_)
        logger().info() << "path_CollisionRearOnTrajectory() ERROR MBED NOT STARTED " << asservMbedStarted_
                << logs::end;
    else {
        mbed_writeI2c('h', 0, NULL);
        pathStatus_ = TRAJ_NEAR_OBSTACLE;
        usleep(300000);
        mbed_writeI2c('r', 0, NULL); //reset de l'arret d'urgence
    }
}
void AsservDriver::path_CancelTrajectory()
{
    if (!connected_)
        return;
    if (!asservMbedStarted_)
        logger().debug() << "path_CancelTrajectory() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
    else {
        mbed_writeI2c('h', 0, NULL);
        pathStatus_ = TRAJ_CANCELLED;
        usleep(300000);
        mbed_writeI2c('r', 0, NULL); //reset de l'arret d'urgence
    }
}
void AsservDriver::path_ResetEmergencyStop()
{
    if (!connected_)
        return;
    if (!asservMbedStarted_)
        logger().debug() << "path_ResetEmergencyStop() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
    else {
        mbed_writeI2c('r', 0, NULL);
    }
}
TRAJ_STATE AsservDriver::motion_DoLine(float dist_meters) //v4 +d
{
    if (!connected_)
        return TRAJ_ERROR;
    if (!asservMbedStarted_) {
        logger().debug() << "motion_DoLine() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {

        unsigned char d[4];
        float2bytes_t mm;
        mm.f = (dist_meters * 1000.0);
        d[0] = mm.b[0];
        d[1] = mm.b[1];
        d[2] = mm.b[2];
        d[3] = mm.b[3];
        logger().debug() << "motion_DoLine() DISTmm=" << mm.f << " meters=" << dist_meters << logs::end;
        mbed_writeI2c('v', 4, d);
        pathStatus_ = TRAJ_OK;
        return mbed_waitEndOfTraj();
    }
}

//0 idle
//1 running
//2 emergency stop
//3 blocked
TRAJ_STATE AsservDriver::mbed_waitEndOfTraj()
{
    int timeout = 0;
    //attente du running status passage de 0,2,3 à 1
    while (p_.asservStatus != 1) {

        //logger().debug() << "1111 p= " << p_.asservStatus << " timeout=" << timeout << logs::end;
        usleep(10000);
        timeout++;
        if (timeout > 10)
            break;
    }

    timeout = 0;
    //attente de l'interruption ou fin de trajectoire
    while (p_.asservStatus == 1) {
        //logger().debug() << "2222 p= " << p_.asservStatus << " timeout=" << timeout << logs::end;
        usleep(1000);
        timeout++;
        if (timeout > 10 && p_.asservStatus != 1)
            break;
    }

    if (p_.asservStatus == 3) {
        //logger().error() << "3333 p= " << p_.asservStatus << logs::end;
        return TRAJ_COLLISION;
    }

    return pathStatus_;
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_m, float y_m) // f8 +x+y
{
    if (!connected_)
        return TRAJ_ERROR;
    if (!asservMbedStarted_) {
        logger().error() << "motion_DoFace() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        unsigned char d[8];
        float2bytes_t x, y;
        x.f = x_m * 1000.0;
        d[0] = x.b[0];
        d[1] = x.b[1];
        d[2] = x.b[2];
        d[3] = x.b[3];
        y.f = y_m * 1000.0;
        d[4] = y.b[0];
        d[5] = y.b[1];
        d[6] = y.b[2];
        d[7] = y.b[3];
        mbed_writeI2c('f', 8, d);
        pathStatus_ = TRAJ_OK;
        return mbed_waitEndOfTraj();
    }
}

TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians) //t4 +d
{
    if (!connected_)
        return TRAJ_ERROR;
    if (!asservMbedStarted_) {
        logger().error() << "motion_DoRotate() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        float angle_degrees = (angle_radians * 180.0) / M_PI;
        unsigned char d[4];
        float2bytes_t deg;
        deg.f = angle_degrees;
        d[0] = deg.b[0];
        d[1] = deg.b[1];
        d[2] = deg.b[2];
        d[3] = deg.b[3];
        mbed_writeI2c('t', 4, d);
        pathStatus_ = TRAJ_OK;
        return mbed_waitEndOfTraj();
    }
}
TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
//TODO motion_DoArcRotate
    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_meters)
{
    if (!connected_)
        return TRAJ_ERROR;
    if (!asservMbedStarted_) {
        logger().debug() << "motion_DoDirectLine() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        unsigned char d[4];
        float2bytes_t mm;
        mm.f = (dist_meters * 1000.0);
        d[0] = mm.b[0];
        d[1] = mm.b[1];
        d[2] = mm.b[2];
        d[3] = mm.b[3];
        logger().debug() << "motion_DoDirectLine() DISTmm=" << mm.f << " meters=" << dist_meters << logs::end;
        mbed_writeI2c('V', 4, d);
        pathStatus_ = TRAJ_OK;

        return mbed_waitEndOfTraj();
    }
}

void AsservDriver::motion_setLowSpeedBackward(bool enable, int percent)
{
    unsigned char d[4];
    unsigned char back_div = (unsigned char) percent;
    if (enable) {
        d[0] = 1;
        d[1] = back_div;
        d[2] = 0;
        d[3] = 0;
    } else {
        d[0] = 0;
        d[1] = 0;
        d[2] = 0;
        d[3] = 0;
    }
    mbed_writeI2c('l', 4, d);

}
void AsservDriver::motion_setLowSpeedForward(bool enable, int percent)
{
    unsigned char d[4];
    unsigned char forw_div = (unsigned char) percent;
    if (enable) {
        d[0] = 1;
        d[1] = 0;
        d[2] = forw_div;
        d[3] = 0;
    } else {
        d[0] = 0;
        d[1] = 0;
        d[2] = 0;
        d[3] = 0;
    }
    mbed_writeI2c('l', 4, d);
}

void AsservDriver::motion_ActivateReguDist(bool enable)
{
    unsigned char d[4];
    if (enable) {
        d[0] = 1;
        d[1] = 0;
        d[2] = 0;
        d[3] = 0;
    } else {
        d[0] = 0;
        d[1] = 0;
        d[2] = 0;
        d[3] = 0;
    }
    mbed_writeI2c('D', 4, d);
}
void AsservDriver::motion_ActivateReguAngle(bool enable)
{
    unsigned char d[4];
    if (enable) {
        d[0] = 1;
        d[1] = 0;
        d[2] = 0;
        d[3] = 0;
    } else {
        d[0] = 0;
        d[1] = 0;
        d[2] = 0;
        d[3] = 0;
    }
    mbed_writeI2c('A', 4, d);
}
void AsservDriver::motion_ResetReguDist()
{
    unsigned char d[4];

    d[0] = 1;
    d[1] = 0;
    d[2] = 0;
    d[3] = 0;

    mbed_writeI2c('R', 4, d);
}
void AsservDriver::motion_ResetReguAngle()
{
    unsigned char d[4];

    d[0] = 0;
    d[1] = 0;
    d[2] = 0;
    d[3] = 0;

    mbed_writeI2c('R', 4, d);
}

void AsservDriver::motion_FreeMotion(void)
{
    if (!connected_)
        return;
    if (!asservMbedStarted_)
        logger().debug() << "motion_FreeMotion() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
    else {
        mbed_writeI2c('K', 0, NULL); //stop mbed managers
    }
}
void AsservDriver::motion_DisablePID() //TODO deprecated  mm chose que Freemotion ???
{
    motion_FreeMotion();
}
void AsservDriver::motion_AssistedHandling(void)
{
    if (!connected_)
        return;
    if (!asservMbedStarted_)
        logger().error() << "motion_AssistedHandling() ERROR MBED NOT STARTED " << asservMbedStarted_ << logs::end;
    else {
        mbed_writeI2c('J', 0, NULL);
    }
}
void AsservDriver::motion_ActivateManager(bool enable)
{
    if (!connected_)
        return;
    if (enable) {
        mbed_writeI2c('I', 0, NULL);
        usleep(100000);
        asservMbedStarted_ = true;

    } else {
        asservMbedStarted_ = false;
        usleep(100000);
        mbed_writeI2c('!', 0, NULL);
    }
}

//------------------------------------------------------------------------

int AsservDriver::mbed_writeI2c(unsigned char cmd, unsigned char nbBytes2Write, unsigned char * data)
{
    m_mbed.lock();
    if (int r = mbedI2c_.writeRegByte(cmd, nbBytes2Write) < 0) {
        printf("ERROR AsservDriver::mbed_writeI2c > writeRegByte > %c%d > %d!\n", cmd, nbBytes2Write, r);
        m_mbed.unlock();
        return -1;
    }
    if (nbBytes2Write != 0)
        if (int r = mbedI2c_.write(data, nbBytes2Write) < 0) {
            printf("ERROR AsservDriver::mbed_writeI2c > write > %c%d > %d!\n", cmd, nbBytes2Write, r);
            m_mbed.unlock();
            return -1;
        }
    m_mbed.unlock();
    return 0;
}

int AsservDriver::mbed_readI2c(unsigned char command, unsigned char nbBytes2Read, unsigned char* data)
{
    m_mbed.lock();
    if (mbedI2c_.writeRegByte(command, nbBytes2Read) < 0) {
        printf("ERROR AsservDriver::mbed_readI2c > writeRegByte > %c%d > error!\n", command, nbBytes2Read);
        m_mbed.unlock();
        return -1;
    }

//Read the data back from the slave
    if (mbedI2c_.read(data, nbBytes2Read) < 0) {
        printf("ERROR AsservDriver::mbed_readI2c > read > %c%d > error!\n", command, nbBytes2Read);
        m_mbed.unlock();
        return -1;
    }
    m_mbed.unlock();
    return 0;
}

int AsservDriver::mbed_ack()
{
    m_mbed.lock();
    unsigned char ack[1];

//ACK
    memset(ack, 0, sizeof(ack));
    if (int r = mbedI2c_.read(ack, 1) < 0) {
        printf("ERROR AsservDriver::mbed_ack() error! %d\n", r);
        m_mbed.unlock();
        return -1;
    }
    m_mbed.unlock();
//printf("mbed_ack 0x%02hhX\n", ack[0]); // hh pour indiquer que c'est un char (pas int)
    if (ack[0] == MBED_ADDRESS)
        return 0;
    else
        return -1;
}

