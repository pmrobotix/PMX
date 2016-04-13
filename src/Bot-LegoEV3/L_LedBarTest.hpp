
#ifndef LEGOEV3LEDBARTEST_HPP
#define	LEGOEV3LEDBARTEST_HPP

#include "../Common/Arguments.hpp"
#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de clignotement des LEDs du tableau d'affichage.
 */
class L_LedBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_LedBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_LedBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_LedBarTest() :
		FunctionalTest("LedBarTest", "Blink Leds")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_LedBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);


};

#endif
