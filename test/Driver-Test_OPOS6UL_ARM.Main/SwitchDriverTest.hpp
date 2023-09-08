/*!
 * \file
 * \brief Définition de la classe SwitchDriverTest.
 */

#ifndef OPOS6UL_SWITCHDRIVERTEST_HPP
#define OPOS6UL_SWITCHDRIVERTEST_HPP


#include "../../src/Common/Interface.Driver/ASwitchDriver.hpp"

#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref SwitchDriverTest.
 */
class SwitchDriverTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SwitchDriverTest(OPO).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"SwitchDriverTest.OPO");
		return instance;
	}

public:

	ASwitchDriver* switchdriver;

	/*!
	 * \brief Constructeur de la classe.
	 */
	SwitchDriverTest() :
			UnitTest("SwitchDriverTest")
	{
		switchdriver = ASwitchDriver::create("SwitchDriverTest");
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~SwitchDriverTest()
	{
	}

	virtual void suite();

	void firstTest();

};
}

#endif
