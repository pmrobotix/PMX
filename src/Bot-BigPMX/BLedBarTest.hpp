/*!
 * \file
 * \brief Définition de la classe LedBarTest.
 */

#ifndef BLEDBARTEST_HPP
#define	BLEDBARTEST_HPP

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de clignotement des LEDs du tableau d'affichage.
 */
class BLedBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("BLedBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	BLedBarTest() :
		FunctionalTest("BLedBarTest", "Blink Leds")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~BLedBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(Arguments *args);
};

#endif
