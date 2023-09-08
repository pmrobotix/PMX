/*!
 * \file
 * \brief Définition de la classe SensorDriverTest.
 */

#ifndef EV3TEST_SENSORDRIVERTEST_HPP
#define EV3TEST_SENSORDRIVERTEST_HPP

#include "../../src/Common/Interface.Driver/ARobotPositionShared.hpp"
#include "../../src/Common/Interface.Driver/ASensorsDriver.hpp"
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

    ASensorsDriver* sensordriver_;
    ARobotPositionShared * aRobotPositionShared_;

public:

    /*!
     * \brief Constructeur de la classe.
     */
    SensorDriverTest() :
            UnitTest("SensorsDriverTest")
    {

        aRobotPositionShared_ = ARobotPositionShared::create();
        sensordriver_ = ASensorsDriver::create("Bot_SensorDriverTest", aRobotPositionShared_);
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
