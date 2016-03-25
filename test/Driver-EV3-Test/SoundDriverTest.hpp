/*!
 * \file
 * \brief Définition de la classe LedDriverTest.
 */

#ifndef EV3TEST_SOUNDDRIVERTEST_HPP
#define EV3TEST_SOUNDDRIVERTEST_HPP

#include "../../src/Common/Action.Driver/ASoundDriver.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{

/*!
 * \brief Teste la classe \ref SoundDriverTest.
 */
class SoundDriverTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SoundDriverTest(EV3).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SoundDriverTest.EV3");
		return instance;
	}

public:

	ASoundDriver* sounddriver;

	/*!
	 * \brief Constructeur de la classe.
	 */
	SoundDriverTest()
			: UnitTest("SoundDriverTest")
	{
		sounddriver = ASoundDriver::create();
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~SoundDriverTest()
	{
	}

	virtual void suite();

	void testSet();

};
}

#endif
