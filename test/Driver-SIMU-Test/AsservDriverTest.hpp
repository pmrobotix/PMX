/*!
 * \file
 * \brief Définition de la classe LedDriverTest.
 */

#ifndef TEST_ASSERVDRIVERTEST_HPP
#define TEST_ASSERVDRIVERTEST_HPP

#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref AsservDriverTest.
 */
class AsservDriverTest: public UnitTest
{
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	AsservDriverTest()
			: UnitTest("AsservDriverTest")
	{

	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~AsservDriverTest()
	{
	}

	virtual void suite();

	void testSet();

};
}

#endif
