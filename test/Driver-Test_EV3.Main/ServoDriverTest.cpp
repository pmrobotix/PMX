/*!
 * \file
 * \brief Implémentation de la classe ServoDriverTest.
 */

#include "ServoDriverTest.hpp"

#include <unistd.h>

#include "../../src/Log/Logger.hpp"

void test::ServoDriverTest::suite()
{
    this->testServos();
}

void test::ServoDriverTest::testServos()
{
    logger().debug() << "testServos()" << logs::end;
    int servo = 0;

    int rate0 = 2000; //timing de 0 à 100%l

    servodriver_->setRate(0, rate0);

    logger().info() << "pos = 0" << logs::end;
    servodriver_->setPosition(servo, 0);

    logger().info() << "pos = 100" << logs::end;
    servodriver_->setPosition(servo, 100);

    logger().info() << "pos = 0" << logs::end;
    servodriver_->setPosition(servo, 0);

    logger().info() << "pos = -100" << logs::end;
    servodriver_->setPosition(servo, -100);

    logger().info() << "pos = 0" << logs::end;
    servodriver_->setPosition(servo, 0);

    logger().info() << "release" << logs::end;
    servodriver_->release(servo);


    this->assert(true, "OK");
}

