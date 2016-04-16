/*!
 * \file
 * \brief Définition de la classe L_IATest.
 */

#ifndef LEGOEV3IATEST_HPP_
#define	LEGOEV3IATEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'ia.
 */
class L_IATest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_IATest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_IATest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_IATest()
			: FunctionalTest("IATest", "test ia")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_IATest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
