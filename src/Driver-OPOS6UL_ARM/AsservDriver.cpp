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
#include "serialib.hpp"
//#include <include/CppLinuxSerial/SerialPort.hpp>

//using namespace mn::CppLinuxSerial;
using namespace std;

AAsservDriver* AAsservDriver::create(string)
{
    static AsservDriver *instance = new AsservDriver();
    return instance;
}

AsservDriver::AsservDriver() :
        // //OPOS6UL_UART5=>1 ; OPOS6UL_UART4=>0
        //serialPort_(SERIAL_ADDRESS, BaudRate::B_115200),
        connected_(true), asservCardStarted_(true), pathStatus_(TRAJ_OK), p_( { 0.0, 0.0, 0.0, -1 })
{
    pp_ = p_;
    errorCount_ = 0;
    statusCountDown_ = 0;
    read_error_ = -1;

    // Create serial port object and open serial port
    char errorOpening = serial_.openDevice(SERIAL_PORT, 115200);
    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening != 1) {
        logger().error() << ">> Error connection to " << SERIAL_PORT << " errorOpening=" << errorOpening << logs::end;
        printf("Error connection to %s errorOpening=%c\n", SERIAL_PORT, errorOpening);
    }

    // Set DTR
    serial_.DTR(true);
    // Clear RTS
    serial_.RTS(false);

    // Read and display the status of each pin
    // DTR should be 1
    // RTS should be 0
//    printf("4-DTR=%d\t", serial_.isDTR());
//    printf("7-RTS=%d\t", serial_.isRTS());
//
//    printf("1-DCD=%d\t", serial_.isDCD());
//    printf("8-CTS=%d\t", serial_.isCTS());
//    printf("6-DSR=%d\t", serial_.isDSR());
//    printf("9-RING=%d\n", serial_.isRI());

    /*
     //TEST OK
     std::string readData = "";
     std::string sdata = "";

     while (1)
     {

         char readData[100] = { 0 };
         //serialPort_.Read(readData);
         int err = serial_.readString(readData, '\n', 100, 1000);

         //                \return  >0 success, return the number of bytes read
         //                    \return  0 timeout is reached
         //                    \return -1 error while setting the Timeout
         //                    \return -2 error while reading the byte
         //                    \return -3 MaxNbBytes is reached
         if (err < 0) {
         printf("serial_.readString error=%d", err);

         }
         else {
         printf("serial_.readString SUCCESS : %s", readData);
         sdata = sdata + *readData;

         int idx = sdata.find("\r\n");

         if (idx >= 0) {
         //on coupe la string à idx+2
         string str2analyse = sdata.substr(0, idx + 2);
         sdata = sdata.substr(idx + 2);

         //logger().debug() << " => (" << sdata  << ") tobeanalysed:" << str2analyse << "!!!"<< logs::end;
         parseAsservPosition(str2analyse);

         }
         }
         odo_SetPosition(100, 500, 1.6);
         utils::Thread::sleep_for_millis(100);

         this->yield();

         motion_AssistedHandling();
         utils::Thread::sleep_for_millis(2000);
     }*/

// Loop forever
//    while (1) {
//        //
//        logger().error() << ">> debug number missed !!" << logs::end;
//        //serial_.writeChar('p');
//        //int err = nucleo_writeSerial("p");
//        //int err = serial_.writeChar('p');
//        int err = serial_.writeString("p\0");
//        if (err < 0) printf("nucleo_writeSerial nucleo_writeSerial(p); error: %d\n", err);
//        char readData[100];
//        serial_.readString(readData, '\n\r', 100, 100000);
//        printf("\n==>readString=%s\n", readData);
//        usleep(1000000);
//    }
//printf("--- readdata %s\n", readData);
//    serialPort_.SetTimeout(500); // Block when reading until any data is received
//    serialPort_.Open();
    /*
     //    // Write some ASCII datae
     serialPort_.Write("p");
     //
     //    // Read some data back
     while (1) {
     string readData;
     serialPort_.Read(readData);
     cout << readData; //TODO timeout
     if (readData == "\r\n") break;
     }
     */

}

AsservDriver::~AsservDriver()
{
    serial_.closeDevice();
}

void AsservDriver::endWhatTodo()
{
    motion_FreeMotion();
    asservCardStarted_ = false;
    if (!this->isFinished())
        this->cancel();
}

void AsservDriver::parseAsservPosition(string str)
{

    if ((str.rfind("#", 0) == 0)) { // && (str.find("\r\n") == (str.length() - 2))
        str = str.substr(1);

        const char delim = ';';
        vector<string> out;
        tokenize(str, delim, out);

        int x = std::stoi(out.operator[](0));
        int y = std::stoi(out.operator[](1));
        float a_rad = std::stof(out.operator[](2));
        int CommandStatus = std::stoi(out.operator[](3));
        int PendingCommandCount = std::stoi(out.operator[](4));
        int lSpeed = std::stoi(out.operator[](5));
        int rSpeed = std::stoi(out.operator[](6));
        int debg = std::stoi(out.operator[](7));

        if (p_.debug_nb != debg - 1) {
            read_error_++;
        } else
            read_error_ = -1;
        if (read_error_ > 0)
            logger().error() << ">> parseAsservPosition : debug number missed !!" << logs::end;

        //test de depart de trajectoire
        if (CommandStatus == 0) {
            m_statusCountDown.lock();
            statusCountDown_--;

            if (statusCountDown_ <= 0) {
                CommandStatus = 0; //idle

            } else {
                CommandStatus = 1; //running
            }
            m_statusCountDown.unlock();
        }



        m_pos.lock();
        p_.x = (float) x; //mm
        p_.y = (float) y; //mm
        p_.theta = a_rad;
        p_.asservStatus = CommandStatus;
        p_.queueSize = PendingCommandCount;
        p_.l_motor_speed = lSpeed;
        p_.r_motor_speed = rSpeed;
        p_.debug_nb = debg;

        m_pos.unlock();

        //si different du precedent, on affiche
        if(!(p_.x == pp_.x && p_.y == pp_.y))
        {
            loggerSvg().info() << "<circle cx=\""
                << p_.x
                << "\" cy=\""
                << -p_.y
                << "\" r=\"1\" fill=\"blue\" />"
                << "<line x1=\""
                << p_.x
                << "\" y1=\""
                << -p_.y
                << "\" x2=\""
                << p_.x + cos(p_.theta) * 25
                << "\" y2=\""
                << -p_.y - sin(p_.theta) * 25
                << "\" stroke-width=\"0.1\" stroke=\"grey\"  />"
                << logs::end;
        }

        pp_ = p_;

        logger().debug() << " ok# " << x << " " << y << " " << a_rad * 180.0 / M_PI << " " << CommandStatus << " "
                << PendingCommandCount << " " << lSpeed << " " << rSpeed << " " << debg << logs::end;
    } else {
        logger().error() << " >> parseAsservPosition BAD LINE : " << str << logs::end;
    }
}

void AsservDriver::tokenize(std::string const &str, const char delim, vector<string> &out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

void AsservDriver::execute()
{
    std::string readData = "";
    //std::string sdata = "";

//printf("execute() START ASSERV POSITION!!\n");
    nucleo_flushSerial();
    while (1) {
        if (asservCardStarted_) {

            char readData[50] = { 0 };

            int err = serial_.readString(readData, '\n', 50, 1000);
            //                \return  >0 success, return the number of bytes read
            //                    \return  0 timeout is reached
            //                    \return -1 error while setting the Timeout
            //                    \return -2 error while reading the byte
            //                    \return -3 MaxNbBytes is reached
            if (err < 0) {
                printf("AsservDriver::execute() ERRROR serial_.readString error=%d", err);

            } else {

                std::string sdata(readData);            //sdata + *readData;

                int idx = sdata.find("#");
                //printf("---serial_.readString SUCCESS : %s idx=%d", readData, idx);
                if (idx >= 0) {
                    //on coupe la string à idx+2
//                    string str2analyse = sdata.substr(0, idx + 2);
//                    sdata = sdata.substr(idx + 2);

                    //logger().debug() << " => (" << sdata  << ") tobeanalysed:" << readData << "!!!"<< logs::end;
                    parseAsservPosition(sdata);

                }
            }

            this->yield();
        }
        utils::Thread::sleep_for_millis(100);

        this->yield();
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
    nucleo_writeSerial('h');

}
void AsservDriver::stopMotorRight()
{
//TODO
    nucleo_writeSerial('h');
}

int AsservDriver::getMotorLeftCurrent()
{
    return 0;
}
int AsservDriver::getMotorRightCurrent()
{
    return 0;
}

void AsservDriver::odo_SetPosition(float x_mm, float y_mm, float angle_rad)
{
    /*
     //TODO mutex ??????
     serialPort_.Write("p");
     //
     //    // Read some data back
     while (1) {
     string readData;
     serialPort_.Read(readData);
     cout << readData; //TODO timeout
     if (readData == "\r\n") break;
     }*/
    nucleo_flushSerial();
    nucleo_writeSerialSTR(
            "P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");
    nucleo_writeSerialSTR(
            "P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");
    nucleo_writeSerialSTR(
            "P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");
    nucleo_writeSerialSTR(
            "P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");

    usleep(100000);
}
RobotPosition AsservDriver::odo_GetPosition()
{
    return p_;
}

//TODO path_GetLastCommandStatus deprecated ? A supprimer
int AsservDriver::path_GetLastCommandStatus()
{
    return -1;
}

void AsservDriver::path_InterruptTrajectory()
{

    if (!asservCardStarted_)
        logger().debug() << "path_InterruptTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {

        //serialPort_.Write("h");
        nucleo_writeSerial('h');
        pathStatus_ = TRAJ_INTERRUPTED;
    }
}
void AsservDriver::path_CollisionOnTrajectory()
{

    if (!asservCardStarted_)
        logger().info() << "path_CollisionOnTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {
        logger().error() << " path_CollisionOnTrajectory() HALT " << asservCardStarted_ << logs::end;

        //serialPort_.Write("h");
        nucleo_writeSerial('h');
        pathStatus_ = TRAJ_NEAR_OBSTACLE;
    }
}
void AsservDriver::path_CollisionRearOnTrajectory()
{

    if (!asservCardStarted_)
        logger().info() << "path_CollisionRearOnTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_
                << logs::end;
    else {

        //serialPort_.Write("h");
        nucleo_writeSerial('h');
        pathStatus_ = TRAJ_NEAR_OBSTACLE;
    }
}
void AsservDriver::path_CancelTrajectory()
{

    if (!asservCardStarted_)
        logger().debug() << "path_CancelTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {
        //TODO ? p_.asservStatus = 3;
        //serialPort_.Write("h");
        nucleo_writeSerial('h');
        pathStatus_ = TRAJ_INTERRUPTED;
    }
}
void AsservDriver::path_ResetEmergencyStop()
{

    if (!asservCardStarted_)
        logger().debug() << "path_ResetEmergencyStop() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
    else {
        logger().error() << "path_ResetEmergencyStop() !! " << logs::end;

        m_pos.lock();
        p_.asservStatus = 0;
        m_pos.unlock();
        //serialPort_.Write("r");
        nucleo_writeSerial('r');
        pathStatus_ = TRAJ_OK;
    }
}

TRAJ_STATE AsservDriver::motion_DoLine(float dist_mm) //v4 +d
{

    if (!asservCardStarted_) {
        logger().debug() << "motion_DoLine() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {

        m_pos.lock();
        p_.asservStatus = 1;
        p_.direction = dist_mm > 0 ? MOVEMENT_DIRECTION::FORWARD : MOVEMENT_DIRECTION::BACKWARD;
        m_pos.unlock();

        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        //serialPort_.Write("v" + to_string((int) (dist_mm)) + "\n");
        nucleo_writeSerialSTR("v" + to_string((int) (dist_mm)) + "\n");
        return nucleo_waitEndOfTraj();
    }
}

//0 idle
//1 running
//2 emergency stop/halted
//3 blocked
TRAJ_STATE AsservDriver::nucleo_waitEndOfTraj()
{

    //on attend la fin de la queue et statut different de running
    while (!(p_.queueSize == 0 && p_.asservStatus != 1)) {
        //logger().error() << "nucleo_waitEndOfTraj statusCountDown_= " << statusCountDown_ << "  p_.asservStatus= " << p_.asservStatus << " p_.queueSize=" << p_.queueSize << logs::end;
        utils::Thread::sleep_for_millis(5);
    }
    if (p_.asservStatus == 3) {
        return TRAJ_COLLISION;
    } else if (p_.asservStatus == 0) {
        statusCountDown_--;
        if (statusCountDown_ <= 0)
            return TRAJ_FINISHED;
    } else if (p_.asservStatus == 2) {
        logger().error() << "_______________________waitEndOfTraj() EMERGENCY STOP OCCURRED  pathStatus_= "
                << pathStatus_ << logs::end;
        return pathStatus_;
    } else {
        logger().error() << "nucleo_waitEndOfTraj else ERROR !!! p_.asservStatus=" << p_.asservStatus << " pathStatus_="
                << pathStatus_ << logs::end;
        return TRAJ_ERROR;
    }
    logger().error() << "nucleo_waitEndOfTraj Never happened !!! p_.asservStatus=" << p_.asservStatus << " pathStatus_="
            << pathStatus_ << logs::end;
    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm)
{
    if (!asservCardStarted_) {
        logger().error() << "motion_DoFace() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        m_pos.lock();
        p_.asservStatus = 1;
        p_.direction = MOVEMENT_DIRECTION::TURN;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        //serialPort_.Write("f" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        nucleo_writeSerialSTR("f" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        return nucleo_waitEndOfTraj();
    }
}

TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians)
{

    logger().error() << "motion_DoRotate() angle_radians=" << angle_radians << logs::end;

    if (!asservCardStarted_) {
        logger().error() << "motion_DoRotate() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        m_pos.lock();
        p_.asservStatus = 1;
        p_.direction = MOVEMENT_DIRECTION::TURN;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        //serialPort_.Write("t" + to_string(AAsservDriver::degToRad(angle_radians)) + "\n");
        nucleo_writeSerialSTR("t" + to_string(AAsservDriver::radToDeg(angle_radians)) + "\n");
        return nucleo_waitEndOfTraj();
    }
}
TRAJ_STATE AsservDriver::motion_DoArcRotate(float angle_radians, float radius)
{
//TODO motion_DoArcRotate
    return TRAJ_ERROR;
}

TRAJ_STATE AsservDriver::motion_Goto(float x_mm, float y_mm)
{

    if (!asservCardStarted_) {
        logger().error() << "motion_Goto() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        m_pos.lock();
        p_.asservStatus = 1; //running
        p_.direction = MOVEMENT_DIRECTION::FORWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        //serialPort_.Write("g" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        nucleo_writeSerialSTR("g" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        return nucleo_waitEndOfTraj();
    }
}

TRAJ_STATE AsservDriver::motion_GotoReverse(float x_mm, float y_mm)
{

    if (!asservCardStarted_) {
        logger().error() << "motion_GotoReverse() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        m_pos.lock();
        p_.asservStatus = 1; //running
        p_.direction = MOVEMENT_DIRECTION::BACKWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        //serialPort_.Write("b" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        nucleo_writeSerialSTR("b" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        return nucleo_waitEndOfTraj();
    }
}

//add
TRAJ_STATE AsservDriver::motion_GotoChain(float x_mm, float y_mm)
{

    if (!asservCardStarted_) {
        logger().error() << "motion_GotoReverse() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        m_pos.lock();
        p_.asservStatus = 1; //running
        p_.direction = MOVEMENT_DIRECTION::FORWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        //serialPort_.Write("e" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        nucleo_writeSerialSTR("e" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        return nucleo_waitEndOfTraj();
        //return TRAJ_OK;
    }
}

TRAJ_STATE AsservDriver::motion_GotoReverseChain(float x_mm, float y_mm)
{

    if (!asservCardStarted_) {
        logger().error() << "motion_GotoReverse() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
        m_pos.lock();
        p_.asservStatus = 1; //running
        p_.direction = MOVEMENT_DIRECTION::BACKWARD;
        m_pos.unlock();
        m_statusCountDown.lock();
        statusCountDown_ = 2;
        m_statusCountDown.unlock();

        //serialPort_.Write("n" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        nucleo_writeSerialSTR("n" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
        return nucleo_waitEndOfTraj();
    }
}

void AsservDriver::motion_setLowSpeedBackward(bool enable, int percent)
{
//serialPort_.Write("S" + to_string(percent) + "\n");
    nucleo_writeSerialSTR("S" + to_string(percent) + "\n");

}
void AsservDriver::motion_setLowSpeedForward(bool enable, int percent)
{
//serialPort_.Write("S" + to_string(percent) + "\n");
    nucleo_writeSerialSTR("S" + to_string(percent) + "\n");

}


void AsservDriver::motion_FreeMotion(void)
{
//serialPort_.Write("M0\n");
    nucleo_writeSerialSTR("M0\n");
}
void AsservDriver::motion_DisablePID() //TODO deprecated  mm chose que Freemotion ???
{
    motion_FreeMotion();
}
void AsservDriver::motion_AssistedHandling(void)
{
//serialPort_.Write("M1\n");
    nucleo_writeSerialSTR("M1\n");
}
void AsservDriver::motion_ActivateManager(bool enable)
{

    if (enable) {
        //effectuer la position de depart apres
        nucleo_writeSerial('R'); //Reset

        //on demarre le check de positionnement...
        this->start("AsservDriver::AsservDriver()", 3);

        //TODO reset and  asservCardStarted_ = true; ????? en mettant a false par defaut ?

    } else {
        //stop the thread
        endWhatTodo();
    }
}

//------------------------------------------------------------------------
void AsservDriver::nucleo_flushSerial()
{
    int err = serial_.flushReceiver();
    if (err == 0)
        printf("nucleo_flushSerial ERROR !\n");
}
int AsservDriver::nucleo_writeSerial(char c)
{
    try {

        int err = serial_.writeChar(c);
        if (err < 0) {
            printf("nucleo_writeSerial() ERRROR serial_.writeChar error=%d on %c\n", err, c);
        }

    } catch (...) {
        printf("nucleo_writeSerial errorCount_=%d\n", errorCount_);
        errorCount_++;
    }
    return errorCount_;
}

int AsservDriver::nucleo_writeSerialSTR(string str)
{
    try {

        //string strr = str + "\n";
        int err = serial_.writeString(str.c_str());
        if (err < 0) {
            printf("nucleo_writeSerial() ERRROR serial_.writeString error=%d on %s\n", err, str);
        } //else printf("nucleo_writeSerial() SUCCESS on %s", strr);

    } catch (...) {
        printf("nucleo_writeSerialSTR errorCount_=%d\n", errorCount_);
        errorCount_++;
    }
    return errorCount_;
}




//Functions deprecated
TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_mm)
{
    logger().error() << "motion_DoDirectLine() NOT IMPLEMENTED " << asservCardStarted_ << logs::end;
    return TRAJ_ERROR;
    if (!asservCardStarted_) {
        logger().debug() << "motion_DoDirectLine() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
        return TRAJ_ERROR;
    } else {
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

//Functions deprecated
void AsservDriver::motion_ActivateReguDist(bool enable)
{
    logger().error() << "motion_ActivateReguDist() NOT IMPLEMENTED " << asservCardStarted_ << logs::end;
}
//Functions deprecated
void AsservDriver::motion_ActivateReguAngle(bool enable)
{
    logger().error() << "motion_ActivateReguAngle() NOT IMPLEMENTED " << asservCardStarted_ << logs::end;
}
//Functions deprecated
void AsservDriver::motion_ResetReguDist()
{

}
//Functions deprecated
void AsservDriver::motion_ResetReguAngle()
{

}


