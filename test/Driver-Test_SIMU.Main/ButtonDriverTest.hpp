/*!
 * \file
 * \brief Définition de la classe ButtonDriverTest.
 */

#ifndef TEST_BUTTONDRIVERTEST_HPP
#define TEST_BUTTONDRIVERTEST_HPP

#include "../../src/Common/Action.Driver/AButtonDriver.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref ButtonDriverTest.
 */
class ButtonDriverTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ButtonDriverTest.SIMU.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ButtonDriverTest.SIMU");
		return instance;
	}
public:

	AButtonDriver* buttondriver;

	/*!
	 * \brief Constructeur de la classe.
	 */
	ButtonDriverTest() :
			UnitTest("ButtonDriverTest")
	{
		buttondriver = AButtonDriver::create();
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~ButtonDriverTest()
	{
	}

	virtual void suite();

	void testSet();

};
}

#endif
