/*!
 * \file
 * \brief Définition de la classe LedDriverTest.
 */

#ifndef TEST_LEDDRIVERTEST_HPP
#define TEST_LEDDRIVERTEST_HPP

#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref LedDriverTest.
 */
class LedDriverTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedDriverTest.SIMU.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LedDriverTest.SIMU");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	LedDriverTest()
			: UnitTest("LedDriverTest")
	{

	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~LedDriverTest()
	{
	}

	virtual void suite();

	void testSet();

};
}

#endif
