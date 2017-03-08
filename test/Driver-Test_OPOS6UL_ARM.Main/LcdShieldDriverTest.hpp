/*!
 * \file
 * \brief Définition de la classe LcdShieldTest.
 */

#ifndef OPOS6UL_LCDSHIELDDRIVERTEST_HPP
#define OPOS6UL_LCDSHIELDDRIVERTEST_HPP


#include <stddef.h>

#include "../../src/Common/Action.Driver/ALcdShieldDriver.hpp"
#include "../../src/Common/Utils/WString.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref LcdShieldDriver.
 */
class LcdShieldDriverTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LcdShieldDriverTest(OPO).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LcdShieldDriverTest.OPO");
		return instance;
	}

public:

	ALcdShieldDriver* lcdshielddriver;

	/*!
	 * \brief Constructeur de la classe.
	 */
	LcdShieldDriverTest()
			: UnitTest("LcdShieldDriverTest")
	{
		lcdshielddriver = ALcdShieldDriver::create("LcdShieldDriverTest");
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~LcdShieldDriverTest()
	{
	}

	virtual void suite();

	void test();
	size_t print(const String &s);

};
}

#endif
