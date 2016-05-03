#ifndef LEGOEV3ASSERVFINDPIDTEST_HPP_
#define	LEGOEV3ASSERVFINDPIDTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test pour trouver les aprametres de PID.
 */
class L_Asserv_FindPIDTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_Asserv_FindPIDTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_Asserv_FindPIDTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_Asserv_FindPIDTest()
			: FunctionalTest("Asserv_FindPID", "find param PID")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_Asserv_FindPIDTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);
};

#endif
