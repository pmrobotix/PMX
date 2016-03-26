#ifndef LEGOEV3ASSERVTEST_HPP_
#define	LEGOEV3ASSERVTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de l'asservissement.
 */
class LegoEV3AsservInsaTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3AsservInsaTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3AsservInsaTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	LegoEV3AsservInsaTest()
			: FunctionalTest("AsservInsaTest", "test de l'asservissement")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~LegoEV3AsservInsaTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(Arguments *args);
};

#endif