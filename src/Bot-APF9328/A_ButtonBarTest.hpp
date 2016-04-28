
#ifndef APF9328BUTTONBARTEST_HPP
#define	APF9328BUTTONBARTEST_HPP

#include "../Common/Arguments.hpp"
#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les buttons.
 */
class A_ButtonBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_ButtonBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_ButtonBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_ButtonBarTest() :
		FunctionalTest("ButtonBarTest","test les buttons.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_ButtonBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
