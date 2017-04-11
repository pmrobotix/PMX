#ifndef ARM_ASSERVDRIVER_HPP_
#define ARM_ASSERVDRIVER_HPP_

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "ExtEncoder.hpp"
#include "Md25.hpp"

using namespace std;

class AsservDriver: public AAsservDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(ARM).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.ARM");
		return instance;
	}

	Md25 md25_;

	ExtEncoder extRightEncoder_;
	ExtEncoder extLeftEncoder_;

public:

	void setMotorLeftPosition(int power, long ticks);
	void setMotorRightPosition(int power, long ticks);

	void setMotorLeftPower(int power, int time);
	void setMotorRightPower(int power, int time);

	long getLeftExternalEncoder();
	long getRightExternalEncoder();

	long getLeftInternalEncoder();
	long getRightInternalEncoder();

	void resetEncoders();
	void resetInternalEncoders();
	void resetExternalEncoders();

	void stopMotorLeft();
	void stopMotorRight();

	int getMotorLeftCurrent();
	int getMotorRightCurrent();

	void enableHardRegulation(bool enable);

	/*!
	 * \brief Constructor.
	 */
	AsservDriver();

	/*!
	 * \brief Destructor.
	 */
	~AsservDriver();

};

#endif
