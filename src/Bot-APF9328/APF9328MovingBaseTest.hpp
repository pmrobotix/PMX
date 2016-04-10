#ifndef APF9328MOVINGBASETEST_HPP_
#define	APF9328MOVINGBASETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test de la base roulante.
 */
class APF9328MovingBaseTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref APF9328MovingBaseTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("APF9328MovingBaseTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	APF9328MovingBaseTest()
			: FunctionalTest("MovingBaseTest", "test de la base roulante")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~APF9328MovingBaseTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
