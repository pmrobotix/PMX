//drivers...OPOS nucleo serial

#include "AsservDriver.hpp"

#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include <string>

#include "../Common/Utils/Chronometer.hpp"
#include "../Log/Logger.hpp"
#include <include/CppLinuxSerial/SerialPort.hpp>

using namespace mn::CppLinuxSerial;
using namespace std;

AAsservDriver * AAsservDriver::create(string) {
    static AsservDriver *instance = new AsservDriver();
    return instance;
}

AsservDriver::AsservDriver() :
        //mbedI2c_(0) //OPOS6UL_UART5=>1 ; OPOS6UL_UART4=>0
        serialPort_(SERIAL_ADDRESS, BaudRate::B_115200), connected_(false), asservCardStarted_(false), pathStatus_(TRAJ_OK),
        p_({ 0.0, 0.0, 0.0, -1 })
{
    errorCount_ = 0;
    statusCountDown_ = 0;

    // Create serial port object and open serial port
    //serialPort_(SERIAL_ADDRESS, BaudRate::B_115200);
    // SerialPort serialPort("/dev/ttyACM0", 13000);
    serialPort_.SetTimeout(500); // Block when reading until any data is received
    serialPort_.Open();

//    // Write some ASCII datae
//    serialPort_.Write("p\r\n");
//
//    // Read some data back
//    while (1) {
//        string readData;
//        serialPort_.Read(readData);
//        cout << readData; //TODO test que la carte repond ?
//        if (readData == "\r\n") break;
//    }

    //on demarre le check de positionnement...
   // this->start("AsservDriver::AsservDriver()", 1); //TODO ajouter une priorité
    //connected_ = true;

    /*
     if (mbedI2c_.setSlaveAddr(MBED_ADDRESS) < 0) //0xAA>>1 = 0x55
     {

     logger().error() << "AsservDriver() : ERROR setSlaveAddr !" << logs::end;
     connected_ = false;
     }
     else {

     //on demarre le check de positionnement...
     this->start("AsservDriver::AsservDriver()");
     connected_ = true;
     }*/

}

AsservDriver::~AsservDriver() {
    endWhatTodo();
    serialPort_.Close();
}

void AsservDriver::endWhatTodo() {
    asservCardStarted_ = false;
    if (!this->isFinished()) this->cancel();
}

void AsservDriver::parseAsservPosition(string str) {
    //chprintf(outputStreamSd4, "#%d;%d;%f;%d;%d;%d;%d\r\n",
//    (int32_t)odometry->getX(), (int32_t)odometry->getY(), odometry->getTheta(),
//    commandManager->getCommandStatus(), commandManager->getPendingCommandCount(),
//    md22MotorController->getLeftSpeed(), md22MotorController->getRightSpeed());

    if ((str.rfind("#", 0) == 0) && (str.find("\r\n") == (str.length() - 2))) {
        str = str.substr(1);

        const char delim = ';';
        vector<string> out;
        tokenize(str, delim, out);

//        for (auto &s: out) {
//            std::cout << s << std::endl;
//        }

        int x = std::stoi(out.operator[](0));
        int y = std::stoi(out.operator[](1));
        float a_rad = std::stof(out.operator[](2));
        int CommandStatus = std::stoi(out.operator[](3));
        int PendingCommandCount = std::stoi(out.operator[](4));
        int lSpeed = std::stoi(out.operator[](5));
        int rSpeed = std::stoi(out.operator[](6));
        int debg = std::stoi(out.operator[](7));

        m_pos.lock();
        p_.x = (float) x; //mm
        p_.y = (float) y; //mm
        p_.theta = a_rad;
        p_.asservStatus = CommandStatus;
        p_.queueSize = PendingCommandCount;
        p_.l_motor_speed = lSpeed;
        p_.r_motor_speed = rSpeed;
        m_pos.unlock();

//        logger().debug() << " ok# "
//                << x
//                << " "
//                << y
//                << " "
//                << a_rad * 180.0 / M_PI
//                << " "
//                << CommandStatus
//                << " "
//                << PendingCommandCount
//                << " "
//                << lSpeed
//                << " "
//                << rSpeed
//                << " "
//                << debg
//                << logs::end;
    }
    else {
        logger().error() << " >> BAD LINE : " << str << logs::end;
    }
}

void AsservDriver::tokenize(std::string const &str, const char delim, vector<string> &out) {
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

void AsservDriver::execute() {
    std::string readData = "";
    std::string sdata = "";

    //recuperation de la ligne d'asserv
//    chprintf(outputStreamSd4, "#%d;%d;%f;%d;%d;%d;%d\r\n",
//                (int32_t)odometry->getX(), (int32_t)odometry->getY(), odometry->getTheta(),
//                commandManager->getCommandStatus(), commandManager->getPendingCommandCount(),
//                md22MotorController->getLeftSpeed(), md22MotorController->getRightSpeed());

    while (1) {
        if (asservCardStarted_) {
            //std::string readData;
            serialPort_.Read(readData);
            sdata = sdata + readData;

            int idx = sdata.find("\r\n");

            if (idx >= 0) {
                //on coupe la string à idx+2
                string str2analyse = sdata.substr(0, idx + 2);
                sdata = sdata.substr(idx + 2);

                //logger().debug() << " => (" << sdata  << ") tobeanalysed:" << str2analyse << "!!!"<< logs::end;
                parseAsservPosition(str2analyse);

            }

            this->yield();
        }
        utils::Thread::sleep_for_millis(100);

        this->yield();
    }

}

void AsservDriver::setMotorLeftPosition(int power, long ticks) {
    //TODO
}

void AsservDriver::setMotorRightPosition(int power, long ticks) {
    //TODO
}

void AsservDriver::setMotorLeftPower(int power, int timems) {
    //TODO
}

void AsservDriver::setMotorRightPower(int power, int timems) {
    //TODO
}

long AsservDriver::getLeftExternalEncoder() {
    return 0;
}
long AsservDriver::getRightExternalEncoder() {
    return 0;
}

long AsservDriver::getLeftInternalEncoder() {
    return 0;
}
long AsservDriver::getRightInternalEncoder() {
    return 0;
}

void AsservDriver::resetEncoders() {
    //TODO
}

void AsservDriver::resetInternalEncoders() {
    //TODO
}
void AsservDriver::resetExternalEncoders() {
    //TODO
}

void AsservDriver::stopMotorLeft() {
    //TODO
}
void AsservDriver::stopMotorRight() {
    //TODO
}

int AsservDriver::getMotorLeftCurrent() {
    return 0;
}
int AsservDriver::getMotorRightCurrent() {
    return 0;
}

void AsservDriver::odo_SetPosition(float x_mm, float y_mm, float angle_rad) {
    if (!connected_) return;

    nucleo_writeSerial("P" + to_string((int)x_mm) + "#" + to_string((int)y_mm) + "#" + to_string(angle_rad)+ "\0");

}
RobotPosition AsservDriver::odo_GetPosition() //en metre
{

    RobotPosition p = nucleo_GetPosition();

    return p;
}

RobotPosition AsservDriver::nucleo_GetPosition() //en metre
{
    m_pos.lock();
    return p_;
    m_pos.unlock();
    /*
     RobotPosition p;
     p.x = -1;
     p.y = -1;
     p.theta = -1;
     if (!connected_) return p;


     // Write some ASCII datae
     serialPort_.Write("p");

     // Read some data back
     while(1) {
     std::string readData;
     serialPort_.Read(readData);
     std::cout  << readData;
     if (readData == "\n")
     break;
     }
     RobotPosition p; //in m
     p.x = x_mm.f / 1000.0;
     p.y = y_mm.f / 1000.0;
     p.theta = rad.f;
     p.asservStatus = status;
     */

    /*
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
     }
     else {
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

     logger().debug() << "mbed_GetPosition p13 " << x_mm.f << " " << y_mm.f << " " << rad.f << " " << status << logs::end;

     RobotPosition p; //in m
     p.x = x_mm.f / 1000.0;
     p.y = y_mm.f / 1000.0;
     p.theta = rad.f;
     p.asservStatus = status;

     return p;
     }*/
}

//TODO path_GetLastCommandStatus deprecated ? A supprimer
int AsservDriver::path_GetLastCommandStatus() {
    return -1;
}

void AsservDriver::path_InterruptTrajectory() {
    if (!connected_) return;
    if (!asservCardStarted_) logger().debug() << "path_InterruptTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {

        serialPort_.Write("h");
        pathStatus_ = TRAJ_INTERRUPTED;
    }
}
void AsservDriver::path_CollisionOnTrajectory() {
    if (!connected_) return;
    if (!asservCardStarted_) logger().info() << "path_CollisionOnTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {
        logger().error() << "path_CollisionOnTrajectory() HALT " << asservCardStarted_ << logs::end;

        serialPort_.Write("h");
        pathStatus_ = TRAJ_NEAR_OBSTACLE;
    }
}
void AsservDriver::path_CollisionRearOnTrajectory() {
    if (!connected_) return;
    if (!asservCardStarted_) logger().info() << "path_CollisionRearOnTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {

        serialPort_.Write("h");
        pathStatus_ = TRAJ_NEAR_OBSTACLE;
    }
}
void AsservDriver::path_CancelTrajectory() {
    if (!connected_) return;
    if (!asservCardStarted_) logger().debug() << "path_CancelTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {
        //TODO ? p_.asservStatus = 3;
        serialPort_.Write("h");
        pathStatus_ = TRAJ_CANCELLED;
    }
}
void AsservDriver::path_ResetEmergencyStop() {
    if (!connected_) return;
    if (!asservCardStarted_) logger().debug() << "path_ResetEmergencyStop() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {
        logger().error() << "path_ResetEmergencyStop() !! " << logs::end;

        m_pos.lock();
        p_.asservStatus = 0;
        m_pos.unlock();
        serialPort_.Write("r");
        pathStatus_ = TRAJ_OK;
    }
}
TRAJ_STATE AsservDriver::motion_DoLine(float dist_mm) //v4 +d
{
    if (!connected_) return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().debug() << "motion_DoLine() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    }
    else {

        m_pos.lock();
        p_.asservStatus = 1;
        p_.direction = dist_mm > 0 ? MOVEMENT_DIRECTION::FORWARD : MOVEMENT_DIRECTION::BACKWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        serialPort_.Write("v" + (int) (dist_mm));

        return nucleo_waitEndOfTraj();
    }
}

//0 idle
//1 running
//2 emergency stop/halted
//3 blocked
TRAJ_STATE AsservDriver::nucleo_waitEndOfTraj() {

    while (!(p_.queueSize == 0 && p_.asservStatus == 0)) {
        utils::Thread::sleep_for_millis(5);
    }

    if (p_.asservStatus == 3) {
        return TRAJ_COLLISION;
    }
    else if (p_.asservStatus == 0) {
        statusCountDown_--;
        if (statusCountDown_ <= 0) return TRAJ_FINISHED;
    }
    else if (p_.asservStatus == 2) {
        logger().error() << "_______________________waitEndOfTraj() EMERGENCY STOP OCCURRED  pathStatus_= " << pathStatus_ << logs::end;
        return pathStatus_;
    }
    else return TRAJ_ERROR;

    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm) {
    if (!connected_) return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().error() << "motion_DoFace() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    }
    else {
        m_pos.lock();
        p_.asservStatus = 1;
        p_.direction = MOVEMENT_DIRECTION::TURN;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        serialPort_.Write("f" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)));

        return nucleo_waitEndOfTraj();
    }
}

TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians) {
    if (!connected_) return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().error() << "motion_DoRotate() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    }
    else {
        m_pos.lock();
        p_.asservStatus = 1;
        p_.direction = MOVEMENT_DIRECTION::TURN;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        serialPort_.Write("t" + to_string(AAsservDriver::degToRad(angle_radians)));

        return nucleo_waitEndOfTraj();
    }
}
TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius) {
//TODO motion_DoArcRotate
    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_Goto(float x_mm, float y_mm) {

    if (!connected_) return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().error() << "motion_Goto() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    }
    else {
        m_pos.lock();
        p_.asservStatus = 1; //running
        p_.direction = MOVEMENT_DIRECTION::FORWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        serialPort_.Write("g" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)));

        return nucleo_waitEndOfTraj();
    }
}

TRAJ_STATE AsservDriver::motion_GotoReverse(float x_mm, float y_mm) {

    if (!connected_) return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().error() << "motion_GotoReverse() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    }
    else {
        m_pos.lock();
        p_.asservStatus = 1; //running
        p_.direction = MOVEMENT_DIRECTION::BACKWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        serialPort_.Write("b" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)));

        return nucleo_waitEndOfTraj();
    }
}

TRAJ_STATE AsservDriver::motion_GotoChain(float x_mm, float y_mm) {

    if (!connected_) return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().error() << "motion_GotoReverse() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    }
    else {
        m_pos.lock();
        p_.asservStatus = 1; //running
        p_.direction = MOVEMENT_DIRECTION::FORWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        serialPort_.Write("e" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)));

        return nucleo_waitEndOfTraj();
    }
}

TRAJ_STATE AsservDriver::motion_GotoReverseChain(float x_mm, float y_mm) {

    if (!connected_) return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().error() << "motion_GotoReverse() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    }
    else {
        m_pos.lock();
        p_.asservStatus = 1; //running
        p_.direction = MOVEMENT_DIRECTION::BACKWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        serialPort_.Write("n" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)));

        return nucleo_waitEndOfTraj();
    }
}

TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_mm) {
    if (!connected_) return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().debug() << "motion_DoDirectLine() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    }
    else {
        /*
         unsigned char d[4];
         float2bytes_t mm;
         mm.f = (dist_meters * 1000.0);
         d[0] = mm.b[0];
         d[1] = mm.b[1];
         d[2] = mm.b[2];
         d[3] = mm.b[3];
         logger().debug() << "motion_DoDirectLine() DISTmm=" << mm.f << " meters=" << dist_meters << logs::end;
         mbed_writeI2c('V', 4, d);
         */
        return nucleo_waitEndOfTraj();
    }
}

void AsservDriver::motion_setLowSpeedBackward(bool enable, int percent) {
    serialPort_.Write("S" + to_string(percent));

}
void AsservDriver::motion_setLowSpeedForward(bool enable, int percent) {
    serialPort_.Write("S" + to_string(percent));

}

void AsservDriver::motion_ActivateReguDist(bool enable) {
    /*
     unsigned char d[4];
     if (enable) {
     d[0] = 1;
     d[1] = 0;
     d[2] = 0;
     d[3] = 0;
     }
     else {
     d[0] = 0;
     d[1] = 0;
     d[2] = 0;
     d[3] = 0;
     }
     mbed_writeI2c('D', 4, d);*/
}
void AsservDriver::motion_ActivateReguAngle(bool enable) {
    /*
     unsigned char d[4];
     if (enable) {
     d[0] = 1;
     d[1] = 0;
     d[2] = 0;
     d[3] = 0;
     }
     else {
     d[0] = 0;
     d[1] = 0;
     d[2] = 0;
     d[3] = 0;
     }
     mbed_writeI2c('A', 4, d);*/
}
void AsservDriver::motion_ResetReguDist() {
    /*
     unsigned char d[4];

     d[0] = 1;
     d[1] = 0;
     d[2] = 0;
     d[3] = 0;

     mbed_writeI2c('R', 4, d);*/
}
void AsservDriver::motion_ResetReguAngle() {
    /*
     unsigned char d[4];

     d[0] = 0;
     d[1] = 0;
     d[2] = 0;
     d[3] = 0;

     mbed_writeI2c('R', 4, d);*/
}

void AsservDriver::motion_FreeMotion(void) {
    if (!connected_) return;
    if (!asservCardStarted_) logger().debug() << "motion_FreeMotion() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {
        serialPort_.Write("M0\0");
    }
}
void AsservDriver::motion_DisablePID() //TODO deprecated  mm chose que Freemotion ???
{
    motion_FreeMotion();
}
void AsservDriver::motion_AssistedHandling(void) {
    if (!connected_) return;
    if (!asservCardStarted_) logger().error() << "motion_AssistedHandling() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {
        serialPort_.Write("M1\0");
    }
}
void AsservDriver::motion_ActivateManager(bool enable) {
    if (!connected_) return;
    if (enable) {
        serialPort_.Write("r");
        serialPort_.Write("M1\0");

        /*
         mbed_writeI2c('I', 0, NULL);
         usleep(100000); //???*/
        asservCardStarted_ = true;

    }
    else {
        asservCardStarted_ = false;

        serialPort_.Write("M0\0");
        /*
         usleep(100000);
         mbed_writeI2c('!', 0, NULL);*/
    }
}

//------------------------------------------------------------------------

int AsservDriver::nucleo_writeSerial(string str) {
    try {
        serialPort_.Write(str);
    }
    catch (...) {
        errorCount_++;
    }

}
//int AsservDriver::mbed_writeI2c(unsigned char cmd, unsigned char nbBytes2Write, unsigned char * data) {
/*
 m_mbed.lock();
 if (int r = mbedI2c_.writeRegByte(cmd, nbBytes2Write) < 0) {
 printf("ERROR AsservDriver::mbed_writeI2c > writeRegByte > %c%d > %d!\n", cmd, nbBytes2Write, r);
 m_mbed.unlock();
 return -1;
 }
 if (nbBytes2Write != 0) if (int r = mbedI2c_.write(data, nbBytes2Write) < 0) {
 printf("ERROR AsservDriver::mbed_writeI2c > write > %c%d > %d!\n", cmd, nbBytes2Write, r);
 m_mbed.unlock();
 return -1;
 }
 m_mbed.unlock();*/
//    return 0;
//}
//int AsservDriver::mbed_readI2c(unsigned char command, unsigned char nbBytes2Read, unsigned char* data) {
/*
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
 m_mbed.unlock();*/
//    return 0;
//}
int AsservDriver::nucleo_ack() {
    /*
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
     if (ack[0] == MBED_ADDRESS) return 0;
     else return -1;

     */
}

