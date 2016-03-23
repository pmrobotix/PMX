/*!
 * \file
 * \brief Définition de la classe LoggerTest.
 */

#ifndef TEST_LOGGER_TEST_HPP
#define TEST_LOGGER_TEST_HPP

#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref Logger.
 */
class LoggerTest: public UnitTest
{
public:

	/*!
	 * \brief Constructeur par défaut.
	 */
	LoggerTest()
			: UnitTest("test::LoggerTest")
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
	 * \brief Teste la classe \ref Logger (tests visuels svg).
	 */
	void testLoggerSvg();

	/*!
	 * \brief Execute les tests associés à la classe \ref LoggerTest.
	 */
	virtual void suite();
};

}
#endif
