#ifndef OPOS6UL_ASSERVSQUARETEST_HPP_
#define	OPOS6UL_ASSERVSQUARETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement sur plusieurs positions.
 */
class O_Asserv_SquareTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_Asserv_SquareTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_Asserv_SquareTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_Asserv_SquareTest()
			: FunctionalTest("Asserv_Square", "effectue un carré")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_Asserv_SquareTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
