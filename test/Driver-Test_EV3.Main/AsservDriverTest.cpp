/*!
 * \file
 * \brief Implémentation de la classe AsservDriverTest.
 */

#include "AsservDriverTest.hpp"

#include "../../src/Common/Utils/Chronometer.hpp"
#include "../../src/Log/Logger.hpp"

void test::AsservDriverTest::suite()
{
    this->test();

}

void test::AsservDriverTest::test()
{
    long left = 0;
    long currentL = 0;
    asservdriver_->resetExternalEncoders();
    utils::Chronometer chrono;
    chrono.start();
    while (1) {

        currentL = asservdriver_->getLeftExternalEncoder();
        if (currentL != left) {
            logger().info() << "currentL = " << currentL << logs::end;
            left = currentL;
        }

        if (chrono.getElapsedTimeInSec() >= 5)
            break;
    }
    /*
     //run forever with 360
     asservdriver->setMotorLeftPower(360, 0);
     sleep(4);
     asservdriver->setMotorLeftPower(200, 10000);

     asservdriver->setMotorLeftPower(800, 3000);
     sleep(2);

     asservdriver->setMotorLeftPower(800, 1000);
     asservdriver->setMotorLeftPower(800, 1000);
     asservdriver->setMotorLeftPower(800, 5000);
     */

    this->assert(true, "OK");
}

