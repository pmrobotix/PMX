/*!
 * \file
 * \brief Implémentation de la classe SensorDriverTest.
 */

#include "SensorDriverTest.hpp"

#include "../../src/Log/Logger.hpp"

void test::SensorDriverTest::suite()
{
    this->testSensors();

}

void test::SensorDriverTest::testSensors()
{
    logger().debug() << "testSensors()" << logs::end;
    bool front = false;
    bool back = false;

    for (int i = 0; i < 10; i++) {
        front = sensordriver_->front();
        back = sensordriver_->rear();
        logger().info() << "front=" << front<< "  back=" << back << logs::end;
        //usleep(500000);
    }
    this->assert(true, "OK");
}

