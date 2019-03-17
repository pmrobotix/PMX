/*!
 * \file
 * \brief Implémentation de la classe SensorDriverTest.
 */

#include "SensorDriverTest.hpp"

#include <unistd.h>

#include "../../src/Log/Logger.hpp"

void test::SensorDriverTest::suite()
{
    this->testSensors();

}

void test::SensorDriverTest::testSensors()
{
    bool front = false;

    for (int i = 0; i < 10; i++) {
        front = sensordriver->front();
        logger().info() << "front=" << front << logs::end;
        //usleep(500000);
    }
    this->assert(true, "OK");
}

