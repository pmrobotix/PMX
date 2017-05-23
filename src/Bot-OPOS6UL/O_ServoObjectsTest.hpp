#ifndef OPOS6UL_SERVOOBJECTSTEST_HPP_
#define	OPOS6UL_SERVOOBJECTSTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur les servos.
 */
class O_ServoObjectsTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_ServoObjectsTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_ServoObjectsTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_ServoObjectsTest()
			: FunctionalTest("ServoObjects", "test Objects 2017")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_ServoObjectsTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);


	void configureConsoleArgs(int argc, char** argv);
};

#endif
