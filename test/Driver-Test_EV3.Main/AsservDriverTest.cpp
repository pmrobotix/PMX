/*!
 * \file
 * \brief Implémentation de la classe AsservDriverTest.
 */

#include "AsservDriverTest.hpp"

#include <unistd.h>

void test::AsservDriverTest::suite()
{
    this->test();

}

void test::AsservDriverTest::test()
{

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

