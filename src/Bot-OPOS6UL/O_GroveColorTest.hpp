#ifndef OPOS6UL_GROVECOLORTEST_HPP_
#define	OPOS6UL_GROVECOLORTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test avec les capteurs coleurs.
 */
class O_GroveColorTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref O_GroveColorTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("O_GroveColorTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	O_GroveColorTest()
			: FunctionalTest("Grove Color", "Tester le capteur coleur")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~O_GroveColorTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
