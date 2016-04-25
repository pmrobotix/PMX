#ifndef APF9328ASSERVFINDPIDTEST_HPP_
#define	APF9328ASSERVFINDPIDTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test pour trouver les aprametres de PID.
 */
class A_Asserv_FindPIDTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_Asserv_FindPIDTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_Asserv_FindPIDTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_Asserv_FindPIDTest()
			: FunctionalTest("Asserv_FindPIDTest", "trouver les PID")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_Asserv_FindPIDTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);
};

#endif
