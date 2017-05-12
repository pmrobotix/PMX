/*!
 * \file
 * \brief Définition de la classe L_IAByPathTest.
 */

#ifndef LEGOEV3IABYPATHTEST_HPP_
#define	LEGOEV3IABYPATHTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'IAByPath.
 */
class L_IAByPathTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_IAByPathTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_IAByPathTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_IAByPathTest()
			: FunctionalTest("IAbyPath", "test l'ia demo IAByPath")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_IAByPathTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	void IASetup();

};

#endif
