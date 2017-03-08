/*!
 * \file
 * \brief Définition de la classe OPOS6UL_LcdBoardTest.
 */

#ifndef OPOS6UL_LCDBOARDTEST_HPP
#define	OPOS6UL_LCDBOARDTEST_HPP

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test du LCD sur 2 lignes.
 */
class O_LcdBoardTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_LcdBoardTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_LcdBoardTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_LcdBoardTest() :
		FunctionalTest("LcdBoard", "Tester le LCD board")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_LcdBoardTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);
};

#endif
