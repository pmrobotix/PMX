#ifndef APF9328ASSERVSQUARETEST_HPP_
#define	APF9328ASSERVSQUARETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement sur plusieurs positions.
 */
class A_Asserv_SquareTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_Asserv_SquareTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_Asserv_SquareTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_Asserv_SquareTest()
			: FunctionalTest("Asserv_SquareTest", "effectue un carré")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_Asserv_SquareTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
