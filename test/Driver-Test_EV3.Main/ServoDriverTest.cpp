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
    int servo2 = 7;

    int rate0ms = 5000; //timing de 0 à 100%l
    int rate0msw = 1000;

    servodriver_->setRate(0, rate0ms);
    servodriver_->setRate(1, 5000);
    servodriver_->setRate(2, 5000);
    servodriver_->setRate(3, 5000);
    servodriver_->setRate(4, 5000);
    servodriver_->setRate(5, 5000);
    servodriver_->setRate(6, 5000);
    servodriver_->setRate(7, 5000);


    logger().info() << "pos = 0" << logs::end;
    servodriver_->setPosition(servo, 0);
    servodriver_->setPosition(servo2, 0);
    usleep(rate0msw * 2000);

    logger().info() << "pos = 100" << logs::end;
    servodriver_->setPosition(servo, 100);
    servodriver_->setPosition(servo2, 100);
    usleep(rate0msw * 2000);

    logger().info() << "pos = 0" << logs::end;
    servodriver_->setPosition(servo, 0);
    servodriver_->setPosition(servo2, 0);
    usleep(rate0msw * 2000);

    logger().info() << "pos = -100" << logs::end;
    servodriver_->setPosition(servo, -100);
    servodriver_->setPosition(servo2, -100);
    usleep(rate0msw * 2000);

    logger().info() << "pos = 0" << logs::end;
    servodriver_->setPosition(servo, 0);
    servodriver_->setPosition(servo2, 0);
    usleep(rate0msw * 2000);

    logger().info() << "release" << logs::end;
    servodriver_->release(servo);
    servodriver_->release(servo2);
    //usleep(rate0msw * 1000);


    this->assert(true, "OK");
}

