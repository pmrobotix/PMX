#ifndef OPOS6UL_ASSERVTEST_HPP_
#define	OPOS6UL_ASSERVTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test avec la carte d'asserv.
 */
class O_AsservTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_AsservTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_AsservTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_AsservTest()
			: FunctionalTest("Asserv", "Tester la carte d'asserv")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_AsservTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
