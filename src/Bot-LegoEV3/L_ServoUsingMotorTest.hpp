/*!
 * \file
 * \brief Définition de la classe L_ServoUsingMotorTest.
 */

#ifndef LEGOEV3SERVOUSINGMOTORTEST_HPP_
#define	LEGOEV3SERVOUSINGMOTORTEST_HPP_

#include "../Common/FunctionalTest.hpp"
#include "../Log/LoggerFactory.hpp"

/*!
 * \brief Effectue un test du moteur en mode servo.
 */
class L_ServoUsingMotorTest: public FunctionalTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref L_ServoUsingMotorTest.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("L_ServoUsingMotorTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	L_ServoUsingMotorTest() :
		FunctionalTest("L_ServoUsingMotorTest", "motor as servo +/-")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~L_ServoUsingMotorTest()
	{
	}

	/*!
	 * \brief Execute le test.
	 */
	virtual void run(int argc, char** argv);

	virtual void configureConsoleArgs(int argc, char** argv);

};

#endif
