/*!
 * \file
 * \brief Implémentation de la classe ColorDriverTest.
 */

#include "ColorDriverTest.hpp"

#include <unistd.h>

#include "../../src/Log/Logger.hpp"

void test::ColorDriverTest::suite()
{
	this->firstTest();

}

void test::ColorDriverTest::firstTest()
{

	for (int i = 0; i < 10; i++)
	{
		bool connected = colordriver->readRGB();
		if (connected)
		{
			//logger().info() << "x: " << colordriver->getTX() << " \ty: " << colordriver->getTY()
			//	<< logs::end;
			printf(" %f %f\n", colordriver->getTX(), colordriver->getTY());
			//usleep(100000);
		}
	}

	this->assert(true, "OK");
}

