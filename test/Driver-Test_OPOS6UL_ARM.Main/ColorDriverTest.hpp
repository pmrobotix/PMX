/*!
 * \file
 * \brief Définition de la classe ColorDriverTest.
 */

#ifndef OPOS6UL_COLORDRIVERTEST_HPP
#define OPOS6UL_COLORDRIVERTEST_HPP


#include "../../src/Common/Interface.Driver/AColorDriver.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref ColorDriverTest.
 */
class ColorDriverTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ColorDriverTest(OPO).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"ColorDriverTest.OPO");
		return instance;
	}

public:

	AColorDriver* colordriver;

	/*!
	 * \brief Constructeur de la classe.
	 */
	ColorDriverTest() :
			UnitTest("ColorDriverTest")
	{
		colordriver = AColorDriver::create("ColorDriverTest");
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~ColorDriverTest()
	{
	}

	virtual void suite();

	void firstTest();

};
}

#endif
