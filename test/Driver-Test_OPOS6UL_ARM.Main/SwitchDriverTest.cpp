/*!
 * \file
 * \brief Implémentation de la classe SwitchDriverTest.
 */

#include "SwitchDriverTest.hpp"

#include <unistd.h>
#include <cstdint>

void test::SwitchDriverTest::suite()
{
	this->firstTest();

}

void test::SwitchDriverTest::firstTest()
{

	for (int i = 0; i < 10; i++)
	{
		int s = switchdriver->tirettePressed();

		//logger().info() << "switch tirette = " << s << logs::end;
		printf("switch tirette = %d\n", s);
		usleep(400000);
	}
	this->assert(true, "OK");
}

