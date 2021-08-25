/*!
 * \file
 * \brief Définition de la classe ActionManagerTimerTest.
 */

#ifndef TEST_TIMERFACTORYTEST_HPP
#define TEST_TIMERFACTORYTEST_HPP

#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test {

/*!
 * \brief Teste la classe \ref pmx::TimerFactoryTest.
 */
class TimerFactoryTest: public UnitTest {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ActionManagerTimerTest.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("test::TimerFactoryTest");
        return instance;
    }

public:

    /*!
     * \brief Constructeur de la classe.
     */
    TimerFactoryTest() :
            UnitTest("TimerFactoryTest - Timers manager")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~TimerFactoryTest() {
    }

    virtual void suite();

    /*!
     * \brief Test.
     */
    void test();



};
}

#endif
