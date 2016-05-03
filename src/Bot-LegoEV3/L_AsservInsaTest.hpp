#ifndef LEGOEV3ASSERVINSATEST_HPP_
#define	LEGOEV3ASSERVINSATEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement.
 */
class L_AsservInsaTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_AsservInsaTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_AsservInsaTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_AsservInsaTest()
			: FunctionalTest("Asserv_Insa", "Find PID (angle+dist)")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_AsservInsaTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
