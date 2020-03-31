/*!
 * \file
 * \brief Définition de la classe LedDriverTest.
 */

#ifndef TEST_ASSERVDRIVERTEST_HPP
#define TEST_ASSERVDRIVERTEST_HPP

#include "../../src/Common/Asserv.Driver/AAsservDriver.hpp"
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

public:

    AAsservDriver *asservdriver;

    /*!
     * \brief Constructeur de la classe.
     */
    AsservDriverTest() :
            UnitTest("AsservDriverTest")
    {
        //test LegoEV3Robot ID
        asservdriver = AAsservDriver::create("LegoEV3Robot");
    }

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
