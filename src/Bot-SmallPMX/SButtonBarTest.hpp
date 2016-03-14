/*!
 * \file
 * \brief Définition de la classe LedBarTest.
 */

#ifndef SBUTTONBARTEST_HPP
#define	SBUTTONBARTEST_HPP

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les buttons.
 */
class SButtonBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ButtonBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SButtonBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	SButtonBarTest() :
		FunctionalTest("SButtonBarTest","Button test")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~SButtonBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(Arguments *args);
};

#endif
