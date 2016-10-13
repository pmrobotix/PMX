/*!
 * \file
 * \brief Définition de la classe L_ServoObjectTest.
 */

#ifndef LEGOEV3SERVOOBJECTTEST_HPP_
#define	LEGOEV3SERVOOBJECTTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test du haut parleur.
 */
class L_ServoObjectTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_ServoObjectTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_ServoObjectTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_ServoObjectTest() :
		FunctionalTest("ServoObject", "position servo")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_ServoObjectTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

};

#endif
