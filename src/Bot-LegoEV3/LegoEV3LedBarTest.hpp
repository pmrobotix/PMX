
#ifndef LEGOEV3LEDBARTEST_HPP
#define	LEGOEV3LEDBARTEST_HPP

#include "../Common/Arguments.hpp"
#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de clignotement des LEDs du tableau d'affichage.
 */
class LegoEV3LedBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3LedBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3LedBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	LegoEV3LedBarTest() :
		FunctionalTest("LegoEV3LedBarTest", "Blink Leds")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~LegoEV3LedBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);


};

#endif
