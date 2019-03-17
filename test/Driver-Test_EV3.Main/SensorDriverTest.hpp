/*!
 * \file
 * \brief Définition de la classe SensorDriverTest.
 */

#ifndef EV3TEST_SENSORDRIVERTEST_HPP
#define EV3TEST_SENSORDRIVERTEST_HPP

#include "../../src/Common/Action.Driver/ASensorsDriver.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test {

/*!
 * \brief Teste la classe \ref SensorDriverTest.
 */
class SensorDriverTest: public UnitTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorDriverTest(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("SensorDriverTest.EV3");
        return instance;
    }

public:

    ASensorsDriver* sensordriver;

    /*!
     * \brief Constructeur de la classe.
     */
    SensorDriverTest() :
            UnitTest("SensorsDriverTest")
    {
        sensordriver = ASensorsDriver::create("Bot_SensorDriverTest");
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~SensorDriverTest()
    {
    }

    virtual void suite();

    void testSensors();

};
}

#endif
