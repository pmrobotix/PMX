/*!
 * \file
 * \brief Définition de la classe LedDriverTest.
 */

#ifndef TEST_ASSERVDRIVERTEST_HPP
#define TEST_ASSERVDRIVERTEST_HPP
#include "../../src/Common/Interface.Driver/ARobotPositionShared.hpp"
#include "../../src/Common/Interface.Driver/AAsservDriver.hpp"

#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test {

/*!
 * \brief Teste la classe \ref AsservDriverTest.
 */
class AsservDriverTest: public UnitTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref AsservDriverTest.SIMU.
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("AsservDriverTest.SIMU");
        return instance;
    }

    ARobotPositionShared * aRobotPositionShared_;
    AAsservDriver *asservdriver;
public:

    /*!
     * \brief Constructeur de la classe.
     */
    AsservDriverTest() ;

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~AsservDriverTest()
    {
    }

    virtual void suite();

    void testSet();

};
}

#endif
