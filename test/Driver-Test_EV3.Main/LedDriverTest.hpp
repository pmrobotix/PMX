/*!
 * \file
 * \brief Définition de la classe LedDriverTest.
 */

#ifndef EV3TEST_LEDDRIVERTEST_HPP
#define EV3TEST_LEDDRIVERTEST_HPP

#include "../../src/Common/Interface.Driver/ALedDriver.hpp"
#include "../Suite/UnitTest.hpp"

namespace test {

/*!
 * \brief Teste la classe \ref LedDriverTest.
 */
class LedDriverTest: public UnitTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LedDriverTest(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LedDriverTest.EV3");
        return instance;
    }

    ALedDriver* leddriver_;

public:

    /*!
     * \brief Constructeur de la classe.
     */
    LedDriverTest() :
            UnitTest("LedDriverTest")
    {
        leddriver_ = ALedDriver::create("LedDriverTest", 2);
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~LedDriverTest()
    {
    }

    virtual void suite();

    void testSet();

};
}

#endif
