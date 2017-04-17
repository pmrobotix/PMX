/*!
 * \file
 * \brief Définition de la classe SensorDriverTest.
 */

#ifndef OPOS6UL_SENSORDRIVERTEST_HPP
#define OPOS6UL_SENSORDRIVERTEST_HPP


#include "../../src/Common/Action.Driver/ASensorsDriver.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref SensorDriverTest.
 */
class SensorDriverTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SensorDriverTest(OPO).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"SensorDriverTest.OPO");
		return instance;
	}

public:

	ASensorsDriver* sensordriver;

	/*!
	 * \brief Constructeur de la classe.
	 */
	SensorDriverTest() :
			UnitTest("SensorDriverTest")
	{
		sensordriver = ASensorsDriver::create("SensorDriverTest");
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~SensorDriverTest()
	{
	}

	virtual void suite();

	void firstTest();

};
}

#endif
