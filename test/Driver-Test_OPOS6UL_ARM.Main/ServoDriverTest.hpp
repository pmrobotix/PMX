/*!
 * \file
 * \brief Définition de la classe ServoDriverTest.
 */

#ifndef OPOS6UL_SERVODRIVERTEST_HPP
#define OPOS6UL_SERVODRIVERTEST_HPP

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
     * \brief Retourne le \ref Logger associé à la classe \ref ServoDriverTest(OPO).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoDriverTest.OPO");
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
        servodriver_ = AServoDriver::create();

    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~ServoDriverTest()
    {
    }

    virtual void suite();

    void firstTest();

};
}

#endif
