/*!
 * \file
 * \brief Définition de la classe O_IAByPathTest.
 */

#ifndef OPOS6UL_IABYPATHTEST_HPP_
#define	OPOS6UL_IABYPATHTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'IAByPath.
 */
class O_IAByPathTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_IAByPathTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_IAByPathTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_IAByPathTest()
			: FunctionalTest("IAbyPath", "test l'ia demo IAByPath")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_IAByPathTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	void IASetup();

};

#endif
