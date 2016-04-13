/*!
 * \file
 * \brief Définition de la classe LegoEV3SoundBarTest.
 */

#ifndef LEGOEV3SOUNDBARTEST_HPP_
#define	LEGOEV3SOUNDBARTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test du haut parleur.
 */
class L_SoundBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_SoundBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_SoundBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_SoundBarTest() :
		FunctionalTest("SoundBarTest", "Play sounds")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_SoundBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
