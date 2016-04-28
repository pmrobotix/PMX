#ifndef APF9328ASSERVINSATEST_HPP_
#define	APF9328ASSERVINSATEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement.
 */
class A_AsservInsaTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_AsservInsaTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_AsservInsaTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_AsservInsaTest()
			: FunctionalTest("Asserv_Insa", "Asserv via PID sur un angle et distance.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_AsservInsaTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
