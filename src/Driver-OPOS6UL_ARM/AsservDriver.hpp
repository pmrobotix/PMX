/*!
 * \file
 * \brief Définition de la classe AsservDriver (OPOS6UL).
 */

#ifndef OPOS6UL_ASSERVDRIVER_HPP_
#define OPOS6UL_ASSERVDRIVER_HPP_

#include "../Common/Asserv.Driver/AAsservDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class AsservDriver: public AAsservDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriver(OPOS6UL).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriver.OPO");
		return instance;
	}

public:

	//commandes directes concernant les moteurs
	void setMotorLeftPosition(int power, long ticks);
	void setMotorRightPosition(int power, long ticks);
	void setMotorLeftPower(int power, int time);
	void setMotorRightPower(int power, int time);
	void stopMotorLeft();
	void stopMotorRight();
	int getMotorLeftCurrent();
	int getMotorRightCurrent();

	//commandes concernant les codeurs
	long getLeftExternalEncoder();
	long getRightExternalEncoder();
	long getLeftInternalEncoder();
	long getRightInternalEncoder();
	void resetEncoders();
	void resetInternalEncoders();
	void resetExternalEncoders();

	//commandes optionnelles de gestion externe
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
