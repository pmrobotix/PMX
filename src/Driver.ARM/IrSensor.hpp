/*!
 * \file
 * \brief Définition de la classe IrSensor.
 */

#ifndef ARM_IRSENSOR_HPP
#define	ARM_IRSENSOR_HPP

#include <sys/types.h>

#include "../Log/LoggerFactory.hpp"



/*!
 * \brief Cette classe représente un composant Infrarouge de type SHARP GP2.
 *
 */
class IrSensor
{

private:
	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref IrSensor.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("IrSensor");
		return instance;
	}


	/*!
	 * \brief Pin de l'ADC (0 à 6).
	 */
	ushort adcPin_;

	/*!
	 * \brief type de sensor (30cm, 80cm, 150cm).
	 */
	int type_;

	/*!
	 * \brief Voltage donné par le composant.
	 */
	int voltage_;

	/*!
	 * \brief Distance mm donné par la conversion.
	 */
	int distanceMm_;

public:


	/*!
	 * \brief Constructeur de la classe.
	 */
	IrSensor(ushort adcPin, int type);

	/*!
	 * \brief Destructeur de la classe.
	 */
	inline ~ IrSensor()
	{
	}

	void reset();

	int getVoltage();

	int getDistance();

	/*!
	 * \brief conversion en mm du voltage donné par l'adc du GP2.
	 */
	double gp2Convert(int type, int value);

};

#endif
