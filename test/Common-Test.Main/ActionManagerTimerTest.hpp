/*!
 * \file
 * \brief Définition de la classe ActionManagerTimerTest.
 */

#ifndef TEST_ACTIONMANAGERTEST_HPP
#define TEST_ACTIONMANAGERTEST_HPP

#include "../Suite/UnitTest.hpp"

namespace test {

/*!
 * \brief Teste la classe \ref pmx::ActionManagerTimerTest.
 */
class ActionManagerTimerTest: public UnitTest {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ActionManagerTimerTest.
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("test::ActionManagerTimerTest");
        return instance;
    }

public:

    /*!
     * \brief Constructeur de la classe.
     */
    ActionManagerTimerTest() :
            UnitTest("ActionManagerTimerTest - Actions + Timer")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~ActionManagerTimerTest() {
    }

    virtual void suite();

    /*!
     * \brief Teste la méthode \ref ActionManager::countActions() and countTimers().
     */
    void testCount();
    void testCountPosix();

    /*!
     * \brief Teste la méthode \ref ActionManager::execute.
     */
    void testExecute();
    void testExecutePosix();
    void testExecutePosixBig();

};
}

#endif
