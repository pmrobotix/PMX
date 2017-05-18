#ifndef OPOS6UL_SERVOSTEPTEST_HPP_
#define	OPOS6UL_SERVOSTEPTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les servos.
 */
class O_ServoStepTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_ServoStepTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_ServoStepTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_ServoStepTest()
			: FunctionalTest("ServoStep", "Servo moving step by step.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_ServoStepTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);


	void configureConsoleArgs(int argc, char** argv);
};

#endif
