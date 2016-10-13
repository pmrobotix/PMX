
#ifndef BOT_LEGOEV3_L_ASSERV_SETRESOLUTIONTEST_HPP_
#define BOT_LEGOEV3_L_ASSERV_SETRESOLUTIONTEST_HPP_


#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement pour trouver les valeurs pour les encoders.
 */
class L_Asserv_SetResolutionTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_Asserv_SetResolutionTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_Asserv_SetResolutionTest");
		return instance;
	}

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_Asserv_SetResolutionTest()
			: FunctionalTest("Asserv_SetResol", "trouver nbr ticks/m")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_Asserv_SetResolutionTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};




#endif
