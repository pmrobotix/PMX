
#ifndef LEGOEV3TIRETTETEST_HPP_
#define	LEGOEV3TIRETTETEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test sur la tirette.
 */
class LegoEV3TiretteTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3TiretteTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3TiretteTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	LegoEV3TiretteTest()
			: FunctionalTest("TiretteTest", "test de la tirette")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~LegoEV3TiretteTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
