/*!
 * \file
 * \brief Définition de la classe LedDriverTest.
 */

#ifndef TEST_LEDDRIVERTEST_HPP
#define TEST_LEDDRIVERTEST_HPP

#include "../Suite/DevUnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref LedDriverTest.
 */
class LedDriverTest: public DevUnitTest
{
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	LedDriverTest()
			: DevUnitTest("LedDriverTest")
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
