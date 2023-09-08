/*!
 * \file
 * \brief Définition de la classe AsservDriverTest.
 */

#ifndef OPOS6UL_ASSERVDRIVERTEST_HPP
#define OPOS6UL_ASSERVDRIVERTEST_HPP

#include "../../src/Common/Interface.Driver/AAsservDriver.hpp"
#include "../../src/Common/Interface.Driver/ARobotPositionShared.hpp"
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
     * \brief Retourne le \ref Logger associé à la classe \ref AsservDriverTest(OPO).
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("AsservDriverTest.OPO");
        return instance;
    }

    ARobotPositionShared * aRobotPositionShared_;
    AAsservDriver *asservdriver;
public:

    /*!
     * \brief Constructeur de la classe.
     */
    AsservDriverTest() :
            UnitTest("AsservDriverTest")
    {
        aRobotPositionShared_ = ARobotPositionShared::create();
        asservdriver = AAsservDriver::create("AsservDriverTest", aRobotPositionShared_);
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~AsservDriverTest()
    {
    }

    virtual void suite();

    void firstTest();

};
}

#endif
