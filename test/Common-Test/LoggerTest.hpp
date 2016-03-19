/*!
 * \file
 * \brief Définition de la classe LoggerTest.
 */

#ifndef TEST_LOGGER_TEST_HPP
#define TEST_LOGGER_TEST_HPP

#include "../Suite/DevUnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref Logger.
 */
class LoggerTest: public DevUnitTest
{
public:

	/*!
	 * \brief Constructeur par défaut.
	 */
	LoggerTest()
			: DevUnitTest("test::LoggerTest")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~LoggerTest()
	{
	}

	/*!
	 * \brief Teste la classe \ref Logger (tests visuels).
	 */
	void testLogger();

	/*!
	 * \brief Execute les tests associés à la classe \ref LoggerTest.
	 */
	virtual void suite();
};

}
#endif
