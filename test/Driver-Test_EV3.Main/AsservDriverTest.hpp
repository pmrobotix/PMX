/*!
 * \file
 * \brief Définition de la classe AsservDriverTest.
 */

#ifndef EV3TEST_ASSERVDRIVERTEST_HPP
#define EV3TEST_ASSERVDRIVERTEST_HPP

#include "../../src/Common/Interface.Driver/AAsservDriver.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

class ALedDriver;

namespace test {

/*!
 * \brief Teste la classe \ref AsservDriverTest.
 */
class AsservDriverTest: public UnitTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref AsservDriverTest(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("AsservDriverTest.EV3");
        return instance;
    }

    ARobotPositionShared * aRobotPositionShared_;

    AAsservDriver* asservdriver_;

public:

    /*!
     * \brief Constructeur de la classe.
     */
    AsservDriverTest() :
            UnitTest("AsservDriverTest")
    {
        aRobotPositionShared_ = ARobotPositionShared::create();
        asservdriver_ = AAsservDriver::create("AsservDriverTest", aRobotPositionShared_);
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~AsservDriverTest()
    {
    }

    virtual void suite();

    void benchmarkSpeedMotor();

    void benchmarkPosix();

    void testcodeurs();
    void testcodeurs2();



};
}

#endif
