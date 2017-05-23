/*!
 * \file
 * \brief Implémentation de la classe IrSensor.
 */

#include "IrSensor.hpp"

#include <cmath>

#include "../Log/Exception.hpp"
#include "../Log/Logger.hpp"
#include "CCAx12Adc.hpp"

IrSensor::IrSensor(ushort adcPin, int type) :
		adcPin_(adcPin), type_(type), voltage_(0.0), distanceMm_(9999.0)
{
}

void IrSensor::reset()
{
	logger().debug() << "              !! reset !! " << logs::end;
	this->voltage_ = 0.0;
	this->distanceMm_ = 9999.0;
}

int IrSensor::getVoltage()
{
	int voltage_ = CCAx12Adc::instance().getADC(adcPin_);
	logger().debug() << "voltage_" << adcPin_ << " = " << voltage_ << logs::end;
	return voltage_;
}

int IrSensor::getDistance()
{

		voltage_ = getVoltage();
		distanceMm_ = gp2Convert(type_, voltage_);
		int nb = 5;
		float tab_[nb];
		float moy = 0.0;
		for (int i = 0; i < nb ; i++)
		{
			tab_[i] = gp2Convert(type_, getVoltage());
			moy += tab_[i];
		}
		moy = (moy + distanceMm_) / (nb+1);

		logger().debug() << "getDistance type=" << type_ << " " << adcPin_ << " dist="
				<< distanceMm_ << " v=" << voltage_ << logs::end;

	return (int) moy;
}

double IrSensor::gp2Convert(int type, int value)
{
	double distanceMillimetre = 0.0;
	if (type == 80)
	{
		//distanceMillimetre = 270.0 * pow(value * 2.5 / 128.0 * 3.3 / 5.0, -1.1); //pour le 80cm //a simplifier si le calcul est bon pour 3.3V

		//distanceMillimetre = pow((valeur * 2.5 / 128.0), -1.1) * 270.0; //version 5V

		if (value > 2500) //saturation
		{
			distanceMillimetre = 55;

		}
		else if (value < 200) //saturation
		{
			distanceMillimetre = 600;
		}
		else
		{
			distanceMillimetre = 845000 * pow(value, -1.15);
		}
	}
	if (type == 30)
	{

		if (value > 2500) //saturation
		{
			distanceMillimetre = 55;
		}
		else if (value < 150) //saturation
		{
			distanceMillimetre = 700;
		}
		else
		{

			//old
			//distanceMillimetre = 93620.1 * pow(value, -0.949);
			//y = 9362,1x-0,949

			distanceMillimetre = (1100.0 * 100.0) / (value - 11.0);
			////Distance (cm) = 2076/(SensorValue - 11)

		}

	}
	if (type == 150)
	{
		//!\todo version 150cm
		//y = 135300x-1,112
		distanceMillimetre = 1353000 * pow(value, -1.112);
	}

	return distanceMillimetre;
}

