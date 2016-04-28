#ifndef APF9328TIRETTETEST_HPP_
#define	APF9328TIRETTETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur la tirette.
 */
class A_TiretteTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref A_TiretteTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("A_TiretteTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	A_TiretteTest()
			: FunctionalTest("Tirette", "Tester la tirette")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~A_TiretteTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
