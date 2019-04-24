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
    int servo2 = 6;
    servodriver_->release(0);
    servodriver_->release(1);
    servodriver_->release(2);
    servodriver_->release(3);
    servodriver_->release(4);
    servodriver_->release(5);
    servodriver_->release(6);
    servodriver_->release(7);

    servodriver_->setRate(servo, 200);
    servodriver_->setPosition(servo, 0);
    sleep(3);
    servodriver_->setPosition(servo, 50);
    sleep(1);
    servodriver_->setPosition(servo, -50);
    sleep(1);
    servodriver_->setPosition(servo, 0);
    sleep(1);

    servodriver_->release(0);
    servodriver_->release(1);
    servodriver_->release(2);
    servodriver_->release(3);
    servodriver_->release(4);
    servodriver_->release(5);
    servodriver_->release(6);
    servodriver_->release(7);

    /*
     int rate0ms = 100; //timing de 0 à 100%l
     int rate0msw = 3000;

     servodriver_->setRate(0, rate0ms);
     servodriver_->setRate(1, 5000);
     servodriver_->setRate(2, 5000);
     servodriver_->setRate(3, 5000);
     servodriver_->setRate(4, 5000);
     servodriver_->setRate(5, 5000);
     servodriver_->setRate(6, 5000);
     servodriver_->setRate(7, rate0msw);


     logger().info() << "pos = 0" << logs::end;
     servodriver_->setPosition(servo, 0);
     servodriver_->setPosition(servo2, 0);
     usleep(rate0msw * 1000);

     logger().info() << "pos = 100" << logs::end;
     servodriver_->setPosition(servo, 100);
     servodriver_->setPosition(servo2, 100);
     usleep(rate0msw * 1000);

     logger().info() << "pos = 0" << logs::end;
     servodriver_->setPosition(servo, 0);
     servodriver_->setPosition(servo2, 0);
     usleep(rate0msw * 1000);

     logger().info() << "pos = -100" << logs::end;
     servodriver_->setPosition(servo, -100);
     servodriver_->setPosition(servo2, -100);
     usleep(rate0msw * 1000);

     logger().info() << "pos = 0" << logs::end;
     servodriver_->setPosition(servo, 0);
     servodriver_->setPosition(servo2, 0);
     usleep(rate0msw * 1000);

     logger().info() << "release" << logs::end;
     servodriver_->release(servo);
     servodriver_->release(servo2);
     servodriver_->release(servo2);
     usleep(10000);
     */

    this->assert(true, "OK");
}

