/*!
 * \file
 * \brief Définition de la classe LcdDriverTest.
 */

#ifndef EV3TEST_LCDDRIVERTEST_HPP
#define EV3TEST_LCDDRIVERTEST_HPP

#include "../../src/Common/Interface.Driver/ALcdShieldDriver.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test {

/*!
 * \brief Teste la classe \ref LcdDriverTest.
 */
class LcdDriverTest: public UnitTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LcdDriverTest(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LcdDriverTest.EV3");
        return instance;
    }

    ALcdShieldDriver* lcddriver_;

public:

    /*!
     * \brief Constructeur de la classe.
     */
    LcdDriverTest() :
            UnitTest("LcdDriverTest")
    {
        lcddriver_ = ALcdShieldDriver::create("Bot_LcdDriverTest");
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~LcdDriverTest()
    {
    }

    virtual void suite();

    void testLCD();

};
}

#endif
