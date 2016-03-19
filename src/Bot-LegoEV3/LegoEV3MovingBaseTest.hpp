#ifndef LEGOEV3MOVINGBASETEST_HPP_
#define	LEGOEV3MOVINGBASETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de la base roulante.
 */
class LegoEV3MovingBaseTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3MovingBaseTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3MovingBaseTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	LegoEV3MovingBaseTest()
			: FunctionalTest("MovingBaseTest", "test de la base roulante")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~LegoEV3MovingBaseTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(Arguments *args);
};

#endif
