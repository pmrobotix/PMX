/*!
 * \file
 * \brief Définition de la classe APF9328LcdBoardTest.
 */

#ifndef APF9328_LCDBOARDTEST_HPP
#define	APF9328_LCDBOARDTEST_HPP

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de clignotement des LEDs du tableau d'affichage.
 */
class A_LcdBoardTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref APF9328LcdBoardTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_LcdBoardTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_LcdBoardTest() :
		FunctionalTest("LcdBoard", "Tester le LCD board")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_LcdBoardTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);
};

#endif
