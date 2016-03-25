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
class LegoEV3SoundBarTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref LegoEV3SoundBarTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3SoundBarTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	LegoEV3SoundBarTest() :
		FunctionalTest("LegoEV3SoundBarTest", "Play sounds")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~LegoEV3SoundBarTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(Arguments *args);
};

#endif
