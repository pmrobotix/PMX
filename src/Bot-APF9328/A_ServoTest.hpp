#ifndef APF9328SERVOTEST_HPP_
#define	APF9328SERVOTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les servos.
 */
class A_ServoTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_ServoTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_ServoTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_ServoTest()
			: FunctionalTest("Servo", "test des servomoteurs")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_ServoTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
