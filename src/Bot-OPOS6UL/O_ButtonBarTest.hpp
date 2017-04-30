
#ifndef OPOS6UL_BUTTONBARTEST_HPP
#define	OPOS6UL_BUTTONBARTEST_HPP

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les buttons.
 */
class O_ButtonBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_ButtonBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_ButtonBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_ButtonBarTest() :
		FunctionalTest("ButtonBar","Tester les buttons un par un.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_ButtonBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
