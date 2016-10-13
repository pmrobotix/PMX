
#ifndef LEGOEV3BUTTONBARTEST_HPP
#define	LEGOEV3BUTTONBARTEST_HPP

#include "../Common/Arguments.hpp"
#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les buttons.
 */
class L_ButtonBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_ButtonBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_ButtonBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_ButtonBarTest() :
		FunctionalTest("ButtonBar","test les buttons.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_ButtonBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
