
#ifndef BOT_LEGOEV3_LEGOEV3ASSERVSETRESOLUTIONTEST_HPP_
#define BOT_LEGOEV3_LEGOEV3ASSERVSETRESOLUTIONTEST_HPP_


#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement pour trouver les valeurs pour les encoders.
 */
class LegoEV3AsservSetResolutionTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3AsservSetResolutionTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3AsservSetResolutionTest");
		return instance;
	}

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	LegoEV3AsservSetResolutionTest()
			: FunctionalTest("AsservSetResolutionTest", "Asservissement - trouver le nombre de ticks pour 1 metre.")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~LegoEV3AsservSetResolutionTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};




#endif
