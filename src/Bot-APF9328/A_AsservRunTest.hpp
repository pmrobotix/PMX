#ifndef APF9328ASSERVRUNTEST_HPP_
#define	APF9328ASSERVRUNTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement sur plusieurs positions.
 */
class A_AsservRunTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_AsservRunTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_AsservRunTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_AsservRunTest()
			: FunctionalTest("AsservRunTest", "Asservissement avant arrière avec sensors.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_AsservRunTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
