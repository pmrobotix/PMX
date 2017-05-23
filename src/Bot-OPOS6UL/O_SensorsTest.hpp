#ifndef OPOS6UL_SENSORSTEST_HPP_
#define	OPOS6UL_SENSORSTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test avec les capteurs IR.
 */
class O_SensorsTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_SensorsTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_SensorsTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_SensorsTest()
			: FunctionalTest("Sensors", "tests capteurs IR/US")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_SensorsTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
