/*!
 * \file
 * \brief Définition de la classe A_IATest.
 */

#ifndef APF9328IATEST_HPP_
#define	APF9328IATEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'ia.
 */
class A_IATest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_IATest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_IATest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_IATest()
			: FunctionalTest("IATest", "test l'ia")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_IATest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	void IASetup();

};

#endif
