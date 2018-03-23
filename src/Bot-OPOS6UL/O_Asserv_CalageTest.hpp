#ifndef OPOS6UL_ASSERVCALAGETEST_HPP_
#define	OPOS6UL_ASSERVCALAGETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test avec la carte d'asserv.
 */
class O_Asserv_CalageTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_Asserv_CalageTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_Asserv_CalageTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_Asserv_CalageTest()
			: FunctionalTest("Asserv recalage", "calage arriere")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_Asserv_CalageTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
