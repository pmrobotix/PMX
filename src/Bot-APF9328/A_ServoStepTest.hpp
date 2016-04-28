#ifndef APF9328SERVOSTEPTEST_HPP_
#define	APF9328SERVOSTEPTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les servos.
 */
class A_ServoStepTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_ServoStepTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_ServoStepTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_ServoStepTest()
			: FunctionalTest("A_ServoStepTest", "Configuration des servomoteurs manuellement")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_ServoStepTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
