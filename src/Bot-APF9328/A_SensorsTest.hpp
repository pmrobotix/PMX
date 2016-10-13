#ifndef APF9328SENSORSTEST_HPP_
#define	APF9328SENSORSTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test avec les capteurs IR.
 */
class A_SensorsTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_SensorsTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_SensorsTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_SensorsTest()
			: FunctionalTest("Sensors", "Tester les capteurs IR")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_SensorsTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
