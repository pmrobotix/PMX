/*!
 * \file
 * \brief Définition de la classe ServoDriverTest.
 */

#ifndef EV3TEST_SERVODRIVERTEST_HPP
#define EV3TEST_SERVODRIVERTEST_HPP

#include "../../src/Common/Action.Driver/AServoDriver.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test {

/*!
 * \brief Teste la classe \ref ServoDriverTest.
 */
class ServoDriverTest: public UnitTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoDriverTest(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriverTest.EV3");
        return instance;
    }

    AServoDriver* servodriver_;

public:

    /*!
     * \brief Constructeur de la classe.
     */
    ServoDriverTest() :
            UnitTest("ServoDriverTest")
    {
        servodriver_ = AServoDriver::create(AServoDriver::SERVO_STANDARD);
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~ServoDriverTest()
    {
    }

    virtual void suite();

    void testServos();

};
}

#endif
