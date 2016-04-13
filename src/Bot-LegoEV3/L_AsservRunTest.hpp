#ifndef LEGOEV3ASSERVRUNTEST_HPP_
#define	LEGOEV3ASSERVRUNTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement sur plusieurs positions.
 */
class L_AsservRunTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3AsservRunTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_AsservRunTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_AsservRunTest()
			: FunctionalTest("AsservRunTest", "test de l'asservissement sur plusieurs positions")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_AsservRunTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
