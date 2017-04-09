/*!
 * \file
 * \brief Définition de la classe AsservDriverTest.
 */

#ifndef OPOS6UL_ASSERVDRIVERTEST_HPP
#define OPOS6UL_ASSERVDRIVERTEST_HPP


#include "../../src/Common/Asserv.Driver/AAsservDriver.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref AsservDriverTest.
 */
class AsservDriverTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref AsservDriverTest(OPO).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"AsservDriverTest.OPO");
		return instance;
	}

public:

	AAsservDriver* asservdriver;

	/*!
	 * \brief Constructeur de la classe.
	 */
	AsservDriverTest() :
			UnitTest("AsservDriverTest")
	{
		asservdriver = AAsservDriver::create("AsservDriverTest");
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~AsservDriverTest()
	{
	}

	virtual void suite();

	void firstTest();

};
}

#endif
