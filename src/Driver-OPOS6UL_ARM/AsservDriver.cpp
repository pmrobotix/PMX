//drivers...OPOS nucleo serial

#include "AsservDriver.hpp"

#include <stddef.h>
#include <cmath>
#include <string>

#include "../Log/Logger.hpp"

//using namespace mn::CppLinuxSerial;
using namespace std;

AAsservDriver* AAsservDriver::create(string, ARobotPositionShared*)
{
	static AsservDriver *instance = new AsservDriver();
	return instance;
}

AsservDriver::AsservDriver() :
		// //OPOS6UL_UART5=>1 ; OPOS6UL_UART4=>0
		//serialPort_(SERIAL_ADDRESS, BaudRate::B_115200),
		connected_(true), pathStatus_(TRAJ_OK), p_( { 0.0, 0.0, 0.0, -1, 0 })		//, asservCardStarted_(false)
{
	pp_ = p_;
	errorCount_ = 0;
	statusCountDown_ = 0;
	read_error_ = -1;
	asservCardStarted_ = true;

	// Create serial port object and open serial port
	char errorOpening = serial_.openDevice(SERIAL_PORT, 115200);
	// If connection fails, return the error code otherwise, display a success message
	if ((int) errorOpening != 1)
	{

		logger().error() << ">> Error connection to " << SERIAL_PORT << " errorOpening=" << (int) errorOpening
				<< logs::end;
		//printf("Error connection to %s errorOpening=%c\n", SERIAL_PORT, errorOpening);
		asservCardStarted_ = false;
	} else
	{

		//logger().info() << ">>  connection to " << SERIAL_PORT << " errorOpening=" << (int)errorOpening << logs::end;

		// Set DTR
		serial_.DTR(true);
		// Clear RTS
		serial_.RTS(false);

		nucleo_flushSerial();
	}

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
	if (!this->isFinished()) this->cancel();
	/*cancel peut causer ca a surveiller!!!!!!!
	 * nucleo_writeSerialSTR errorCount_=0
	 FATAL: exception not rethrown
	 Stack trace (most recent call last) in thread 394:
	 #4    Object "/lib/libc.so.6", at 0xb6b3b6a1, in gsignal
	 #3    Object "/lib/libc.so.6", at 0xb6b2ce05, in
	 #2    Object "/lib/libc.so.6", at 0xb6b3c1cf, in
	 #1    Object "./Bot_ArmadeusOPOS6UL_ARM", at 0x4d28df, in
	 #0    Object "./Bot_ArmadeusOPOS6UL_ARM", at 0x4d2607, in
	 Aborted (Signal sent by tkill() 392 0)
	 Aborted
	 */

}

void AsservDriver::parseAsservPosition(string str)
{

	if ((str.rfind("#", 0) == 0))
	{
		str = str.substr(1);

		const char delim = ';';
		vector<string> out;
		tokenize(str, delim, out);

		if (out.size() == 8) //check de la size pour eviter les erreur stoi ou stoul
		{
			try
			{
				int x = std::stoi(out.operator[](0));
				int y = std::stoi(out.operator[](1));
				float a_rad = std::stof(out.operator[](2));
				int CommandStatus = std::stoi(out.operator[](3));
				int PendingCommandCount = std::stoi(out.operator[](4));
				int lSpeed = std::stoi(out.operator[](5));
				int rSpeed = std::stoi(out.operator[](6));
				unsigned int debg = std::stoul(out.operator[](7));

				if (p_.debug_nb != debg - 1)
				{
					read_error_++;
				} else
					read_error_ = -1;
				if (read_error_ > 0) logger().error() << ">> parseAsservPosition : debug number missed !!" << logs::end;

				//test de depart de trajectoire
				if (CommandStatus == 0)
				{
					m_statusCountDown.lock();
					statusCountDown_--;

					if (statusCountDown_ <= 0)
					{
						CommandStatus = 0; //idle

					} else
					{
						CommandStatus = 1; //running
					}
					m_statusCountDown.unlock();
				}

				m_pos.lock();
				p_.x = (float) x; //mm
				p_.y = (float) y; //mm
				p_.theta = a_rad;
				p_.asservStatus = CommandStatus;
				p_.debug_nb = debg;
				m_pos.unlock();

				//si different du precedent, on affiche
				if (!(p_.x == pp_.x && p_.y == pp_.y))
				{
					loggerSvg().info() << "<circle cx=\"" << p_.x << "\" cy=\"" << -p_.y
							<< "\" r=\"1\" fill=\"blue\" />" << "<line x1=\"" << p_.x << "\" y1=\"" << -p_.y
							<< "\" x2=\"" << p_.x + cos(p_.theta) * 25 << "\" y2=\"" << -p_.y - sin(p_.theta) * 25
							<< "\" stroke-width=\"0.1\" stroke=\"grey\"  />" << logs::end;
				}

				pp_ = p_;

				if (asservCardStarted_ == false)
				{
					asservCardStarted_ = true;
					odo_SetPosition((float) x, (float) y, (float) a_rad);

				}
				logger().debug() << " ok# " << x << " " << y << " " << a_rad * 180.0 / M_PI << " status="
						<< CommandStatus << " " << PendingCommandCount << " " << lSpeed << " " << rSpeed << " " << debg
						<< logs::end;
			} catch (const std::exception &e)
			{
				std::cout << e.what(); // information from length_error printed
				std::cout << "parseAsservPosition !!!!!!!ERROR msg is: " << str << std::endl;

			}
		} else
		{
			logger().error() << " >> parseAsservPosition out BAD LINE : " << str << logs::end;
		}
	} else
	{
		logger().error() << " >> parseAsservPosition BAD LINE : " << str << logs::end;
	}
}

void AsservDriver::tokenize(std::string const &str, const char delim, vector<string> &out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}

void AsservDriver::execute()
{
	//printf("execute() START ASSERV POSITION!!\n");
	std::string readData = "";

	nucleo_flushSerial();
	while (1)
	{
//		if (asservCardStarted_)
//		{

		char readData[100] = { 0 };

		int err = serial_.readString(readData, '\n', 100, 150);
		//                \return  >0 success, return the number of bytes read
		//                    \return  0 timeout is reached
		//                    \return -1 error while setting the Timeout
		//                    \return -2 error while reading the byte
		//                    \return -3 MaxNbBytes is reached
		if (err <= 0)
		{
			//printf("AsservDriver::execute() ERRROR serial_.readString error=%d \n", err);
			asservCardStarted_ = 0;

		} else
		{

			std::string sdata(readData);            //sdata + *readData;

			int idx = sdata.find("#");
			//printf("---serial_.readString SUCCESS : %s idx=%d", readData, idx);
			if (idx >= 0)
			{

				//on coupe la string à idx+2
//                    string str2analyse = sdata.substr(0, idx + 2);
//                    sdata = sdata.substr(idx + 2);

				logger().debug() << " => (" << sdata << ") tobeanalysed:" << readData << "!!!" << logs::end;
				parseAsservPosition(sdata);

			}
		}

		this->yield();
//		}
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
	if (!asservCardStarted_)
		logger().error() << "stopMotorLeft() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
	else
	{
		nucleo_writeSerial('h');
	}

}
void AsservDriver::stopMotorRight()
{
	if (!asservCardStarted_)
		logger().error() << "stopMotorRight() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
	else
	{
		nucleo_writeSerial('h');
	}
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

	 //    // Read some data back
	 while (1) {
	 string readData;
	 serialPort_.Read(readData);
	 cout << readData; //TODO timeout
	 if (readData == "\r\n") break;
	 }*/

//	if (!asservCardStarted_)
//	{
//		logger().error() << "odo_SetPosition() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
	//On met tout de meme a jour pour la valeur initiale et les tests meme si la carte asserv est eteinte
	m_pos.lock();
	p_.x = x_mm; //mm
	p_.y = y_mm; //mm
	p_.theta = angle_rad;
	p_.asservStatus = 0;
	p_.debug_nb = 0;
	pp_ = p_;
	m_pos.unlock();

//	} else
//	{
	nucleo_flushSerial();
	nucleo_writeSerialSTR(
			"P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");

	/*nucleo_writeSerialSTR(
	 "P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");
	 nucleo_writeSerialSTR(
	 "P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");
	 nucleo_writeSerialSTR(
	 "P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");
	 */
	//usleep(100000); //TODO Thread sleep ??? pour etre sur d'attendre la prochaine valeur ? ATTENDRE jusquà la validation de la valeur
//		while()
//		{
//		nucleo_flushSerial();
//		nucleo_writeSerialSTR(
//				"P" + to_string((int) x_mm) + "#" + to_string((int) y_mm) + "#" + to_string(angle_rad) + "\n");
//		}
//	}
}
ROBOTPOSITION AsservDriver::odo_GetPosition()
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
	else
	{
		nucleo_writeSerial('h');
		nucleo_writeSerial('h');
		pathStatus_ = TRAJ_INTERRUPTED;
	}
}
void AsservDriver::path_CollisionOnTrajectory()
{

	if (!asservCardStarted_)
		logger().debug() << "path_CollisionOnTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
	else
	{
		//logger().debug() << " path_CollisionOnTrajectory() HALT " << asservCardStarted_ << logs::end;

		nucleo_writeSerial('h');
		nucleo_writeSerial('h');

//		while(p_.asservStatus == 2)
//
//		{
//
//			logger().debug() << " path_CollisionOnTrajectory() HALT " << asservCardStarted_ << logs::end;
//
//
//		}
		pathStatus_ = TRAJ_NEAR_OBSTACLE;
	}
}
void AsservDriver::path_CollisionRearOnTrajectory()
{

	if (!asservCardStarted_)
		logger().debug() << "path_CollisionRearOnTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_
				<< logs::end;
	else
	{

		nucleo_writeSerial('h');
		pathStatus_ = TRAJ_NEAR_OBSTACLE;
	}
}
void AsservDriver::path_CancelTrajectory()
{

	if (!asservCardStarted_)
		logger().debug() << "path_CancelTrajectory() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
	else
	{

		nucleo_writeSerial('h');
		pathStatus_ = TRAJ_INTERRUPTED;
	}
}
void AsservDriver::path_ResetEmergencyStop()
{

	if (!asservCardStarted_)
		logger().debug() << "path_ResetEmergencyStop() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
	else
	{
		logger().debug() << "path_ResetEmergencyStop() !! " << logs::end;

		m_pos.lock();
		p_.asservStatus = 0;
		m_pos.unlock();

		nucleo_writeSerial('r');
		pathStatus_ = TRAJ_OK;
	}
}

TRAJ_STATE AsservDriver::motion_DoLine(float dist_mm) //v4 +d
{

	if (!asservCardStarted_)
	{
		logger().debug() << "motion_DoLine() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
		return TRAJ_ERROR;
	} else
	{

		m_pos.lock();
		p_.asservStatus = 1;
		m_pos.unlock();

		m_statusCountDown.lock();
		statusCountDown_ = 5;
		m_statusCountDown.unlock();

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

	if (!asservCardStarted_)
	{
		logger().error() << "nucleo_waitEndOfTraj() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
		return TRAJ_ERROR;
	} else
	{
		//while (!(p_.asservStatus != 1))
		while (p_.asservStatus == 1)
		{
//			logger().error() << "_______________________waitEndOfTraj() statusCountDown_=" <<statusCountDown_ << "  pathStatus_= "
//																	<< pathStatus_ << " p_.asservStatus=" << p_.asservStatus << logs::end;

			utils::Thread::sleep_for_millis(50);
		}

		//logger().error() << "__nucleo_waitEndOfTraj  p_.asservStatus= "	<< p_.asservStatus << logs::end;

		if (p_.asservStatus == 3)
		{
			return TRAJ_COLLISION;
		} else if (p_.asservStatus == 0)
		{
			statusCountDown_--;

			if (statusCountDown_ <= 0)
			{
				statusCountDown_ = 5;
				return TRAJ_FINISHED;
			}
		} else if (p_.asservStatus == 2)
		{
//			logger().error() << "_______________________waitEndOfTraj() EMERGENCY STOP OCCURRED  pathStatus_= "
//					<< pathStatus_ << " p_.asservStatus=" << p_.asservStatus << logs::end;
			return pathStatus_;
			//return TRAJ_INTERRUPTED;
		} else
		{
			logger().error() << "____nucleo_waitEndOfTraj else ERROR STATUT IMPOSSIBLE !!! p_.asservStatus=" << p_.asservStatus
					<< " pathStatus_=" << pathStatus_ << " statusCountDown_=" << statusCountDown_<< logs::end;
			return TRAJ_ERROR;
		}
		logger().error() << "nucleo_waitEndOfTraj INTERRUPTED !!! p_.asservStatus=" << p_.asservStatus
				<< " pathStatus_=" << pathStatus_  << " statusCountDown_=" << statusCountDown_<< logs::end;
		return TRAJ_INTERRUPTED;
	}
}

TRAJ_STATE AsservDriver::motion_DoFace(float x_mm, float y_mm)
{
	if (!asservCardStarted_)
	{
		logger().debug() << "motion_DoFace() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
		return TRAJ_ERROR;
	} else
	{
		m_pos.lock();
		p_.asservStatus = 1;
		m_pos.unlock();

		m_statusCountDown.lock();
		statusCountDown_ = 5;
		m_statusCountDown.unlock();

		nucleo_writeSerialSTR("f" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
		return nucleo_waitEndOfTraj();
	}
}

TRAJ_STATE AsservDriver::motion_DoRotate(float angle_radians)
{

	//logger().error() << "motion_DoRotate() angle_radians=" << angle_radians << logs::end;

	if (!asservCardStarted_)
	{
		logger().debug() << "motion_DoRotate() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
		return TRAJ_ERROR;
	} else
	{
		m_pos.lock();
		p_.asservStatus = 1;
		m_pos.unlock();

		m_statusCountDown.lock();
		statusCountDown_ = 5;
		m_statusCountDown.unlock();

		nucleo_writeSerialSTR("t" + to_string(radToDeg(angle_radians)) + "\n");
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

	if (!asservCardStarted_)
	{
		logger().debug() << "motion_Goto() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
		return TRAJ_ERROR;
	} else
	{
		m_pos.lock();
		p_.asservStatus = 1; //running
		m_pos.unlock();

		m_statusCountDown.lock();
		statusCountDown_ = 5;
		m_statusCountDown.unlock();

		nucleo_writeSerialSTR("g" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
		return nucleo_waitEndOfTraj();
	}
}

TRAJ_STATE AsservDriver::motion_GotoReverse(float x_mm, float y_mm)
{

	if (!asservCardStarted_)
	{
		logger().debug() << "motion_GotoReverse() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
		return TRAJ_ERROR;
	} else
	{
		m_pos.lock();
		p_.asservStatus = 1; //running
		m_pos.unlock();

		m_statusCountDown.lock();
		statusCountDown_ = 5;
		m_statusCountDown.unlock();

		nucleo_writeSerialSTR("b" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
		return nucleo_waitEndOfTraj();
	}
}

//add
TRAJ_STATE AsservDriver::motion_GotoChain(float x_mm, float y_mm)
{

	if (!asservCardStarted_)
	{
		logger().debug() << "motion_GotoChain() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
		return TRAJ_ERROR;
	} else
	{
		m_pos.lock();
		p_.asservStatus = 1; //running
		m_pos.unlock();

		m_statusCountDown.lock();
		statusCountDown_ = 5;
		m_statusCountDown.unlock();

		nucleo_writeSerialSTR("e" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
		return nucleo_waitEndOfTraj();
	}
}

TRAJ_STATE AsservDriver::motion_GotoReverseChain(float x_mm, float y_mm)
{

	if (!asservCardStarted_)
	{
		logger().debug() << "motion_GotoReverseChain() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;
		return TRAJ_ERROR;
	} else
	{
		m_pos.lock();
		p_.asservStatus = 1; //running
		m_pos.unlock();

		m_statusCountDown.lock();
		statusCountDown_ = 5;
		m_statusCountDown.unlock();

		nucleo_writeSerialSTR("n" + to_string((int) (x_mm)) + "#" + to_string((int) (y_mm)) + "\n");
		return nucleo_waitEndOfTraj();
	}
}

void AsservDriver::motion_setLowSpeedBackward(bool enable, int percent)
{
	if (!asservCardStarted_)
	{
		logger().debug() << "motion_setLowSpeedBackward() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;

	} else
	{
		nucleo_writeSerialSTR("S" + to_string(percent) + "\n");
	}

}
void AsservDriver::motion_setLowSpeedForward(bool enable, int percent)
{
	if (!asservCardStarted_)
	{
		logger().debug() << "motion_setLowSpeedForward() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;

	} else
	{
		nucleo_writeSerialSTR("S" + to_string(percent) + "\n");
	}

}

void AsservDriver::motion_setMaxSpeed(bool enable, int speed_dist_m_sec, int speed_angle_rad_sec)
{
	if (!asservCardStarted_)
	{
		logger().debug() << "motion_setMaxSpeed() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;

	} else
	{
		if (enable)
		{
			nucleo_writeSerialSTR("N" + to_string(speed_dist_m_sec) + "#" + to_string(speed_angle_rad_sec) + "\n");
		} else
		{
			nucleo_writeSerial('!');
		}
	}

}

void AsservDriver::motion_FreeMotion(void)
{
	if (!asservCardStarted_)
	{
		logger().debug() << "motion_FreeMotion() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;

	} else
	{
		nucleo_writeSerialSTR("M0\n");
	}
}
void AsservDriver::motion_DisablePID() //TODO deprecated  mm chose que Freemotion ???
{

	motion_FreeMotion();

}
void AsservDriver::motion_AssistedHandling(void)
{
	if (!asservCardStarted_)
	{
		logger().debug() << "motion_AssistedHandling() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;

	} else
	{
		nucleo_writeSerialSTR("M1\n");
	}
}
void AsservDriver::motion_ActivateManager(bool enable) //TODO enable pas utile ?
{

	if (enable)
	{
		if (!asservCardStarted_)
		{
			logger().debug() << "motion_ActivateManager() ERROR NUCLEO NOT STARTED " << asservCardStarted_ << logs::end;

		} else
		{
			//effectuer la position de depart apres
			nucleo_writeSerial('R'); //Reset

		}
		//on demarre le check de positionnement...
		this->start("AsservDriver::AsservDriver()", 3);

	} else
	{
		//stop the thread
		endWhatTodo();
	}
}

void AsservDriver::motion_ActivateReguDist(bool enable)
{
	if (!asservCardStarted_)
	{
		logger().debug() << "motion_DoDirectLine() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;

	} else
	{

		if (enable)
			nucleo_writeSerialSTR("D1\n");
		else
			nucleo_writeSerialSTR("D0\n");
	}
}

void AsservDriver::motion_ActivateReguAngle(bool enable)
{
	if (!asservCardStarted_)
	{
		logger().debug() << "motion_DoDirectLine() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;

	} else
	{

		if (enable)
			nucleo_writeSerialSTR("A1\n");
		else
			nucleo_writeSerialSTR("A0\n");
	}
}

//------------------------------------------------------------------------
int AsservDriver::nucleo_flushSerial()
{
	int err = serial_.flushReceiver();
	if (err == 0) printf("nucleo_flushSerial ERROR !\n");
	return err;
}
int AsservDriver::nucleo_writeSerial(char c)
{
	try
	{

		int err = serial_.writeChar(c);
		if (err < 0)
		{
			printf("nucleo_writeSerial() ERRROR serial_.writeChar error=%d on %c\n", err, c);
		}
	} catch (const std::exception &e)
	{
		std::cout << e.what(); // information from length_error printed
		std::cout << "nucleo_writeSerial errorCount_= " << errorCount_ << std::endl;
	}
//	} catch (...)
//	{
//		printf("nucleo_writeSerial errorCount_=%d\n", errorCount_);
//		errorCount_++;
//	}
	return errorCount_;
}

int AsservDriver::nucleo_writeSerialSTR(string str)
{
	try
	{

		//string strr = str + "\n";
		int err = serial_.writeString(str.c_str());
		if (err < 0)
		{
			printf("nucleo_writeSerialSTR() ERRROR serial_.writeString error=%d on %s\n", err, str.c_str());
		} //else printf("nucleo_writeSerial() SUCCESS on %s", strr);
	} catch (const std::exception &e)
	{
		std::cout << e.what(); // information from length_error printed
		std::cout << "nucleo_writeSerialSTR errorCount_= " << errorCount_ << std::endl;
	}
//	} catch (...)
//	{
//		printf("nucleo_writeSerialSTR errorCount_=%d\n", errorCount_);
//		errorCount_++;
//	}
	return errorCount_;
}

/*
 //Functions deprecated
 TRAJ_STATE AsservDriver::motion_DoDirectLine(float dist_mm)
 {
 logger().error() << "motion_DoDirectLine() NOT IMPLEMENTED " << asservCardStarted_ << logs::end;
 return TRAJ_ERROR;
 if (!asservCardStarted_) {
 logger().debug() << "motion_DoDirectLine() ERROR MBED NOT STARTED " << asservCardStarted_ << logs::end;
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

 return nucleo_waitEndOfTraj();
 }
 }


 //Functions deprecated
 void AsservDriver::motion_ResetReguDist()
 {

 }
 //Functions deprecated
 void AsservDriver::motion_ResetReguAngle()
 {

 }
 */

