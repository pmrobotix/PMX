
#ifndef BOT_APF9328_ASSERV_SETRESOLUTIONTEST_HPP_
#define BOT_APF9328_ASSERV_SETRESOLUTIONTEST_HPP_


#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement pour trouver les valeurs pour les encoders.
 */
class A_Asserv_SetResolutionTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_Asserv_SetResolutionTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_Asserv_SetResolutionTest");
		return instance;
	}

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_Asserv_SetResolutionTest()
			: FunctionalTest("Asserv_SetResolutionTest", "Asservissement - trouver le nbre de ticks pour 1 metre.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_Asserv_SetResolutionTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};




#endif
