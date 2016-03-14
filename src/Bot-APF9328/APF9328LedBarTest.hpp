/*!
 * \file
 * \brief Définition de la classe LedBarTest.
 */

#ifndef APF9328_LEDBARTEST_HPP
#define	APF9328_LEDBARTEST_HPP

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de clignotement des LEDs du tableau d'affichage.
 */
class APF9328LedBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LedBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("APF9328LedBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	APF9328LedBarTest() :
		FunctionalTest("APF9328LedBarTest", "Blink Leds")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~APF9328LedBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(Arguments *args);
};

#endif
