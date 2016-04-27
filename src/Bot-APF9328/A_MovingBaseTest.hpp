#ifndef APF9328MOVINGBASETEST_HPP_
#define	APF9328MOVINGBASETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de la base roulante.
 */
class A_MovingBaseTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_MovingBaseTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_MovingBaseTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_MovingBaseTest()
			: FunctionalTest("MovingBase", "test de la base roulante")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_MovingBaseTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
