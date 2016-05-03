#ifndef LEGOEV3MOVINGBASETEST_HPP_
#define	LEGOEV3MOVINGBASETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de la base roulante.
 */
class L_MovingBaseTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_MovingBaseTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_MovingBaseTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_MovingBaseTest()
			: FunctionalTest("MovingBase", "base roulante")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_MovingBaseTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
