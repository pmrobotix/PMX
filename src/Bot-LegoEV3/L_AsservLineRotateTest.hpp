#ifndef LEGOEV3ASSERVLINEROTATETEST_HPP_
#define	LEGOEV3ASSERVLINEROTATETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement sur une ligne droite et une rotation.
 */
class L_AsservLineRotateTest: public FunctionalTest, public utils::Thread
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3AsservRunTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_AsservLineRotateTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_AsservLineRotateTest()
			: FunctionalTest("Asserv_LineRotate", "Effectue une ligne droite et une rotation eventuelle.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_AsservLineRotateTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

	//execution du thread
	void execute();

};

#endif
