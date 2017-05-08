/*!
 * \file
 * \brief Implémentation de la classe AsservDriverTest.
 */

#include "AsservDriverTest.hpp"

#include <unistd.h>

#include "../../src/Log/Logger.hpp"

void test::AsservDriverTest::suite()
{
	this->firstTest();

}

void test::AsservDriverTest::firstTest()
{
	asservdriver->motion_ActivateManager(true);
	for (int n = 0; n < 5; n++)
	{
		asservdriver->odo_SetPosition(0.10, 0.55, 45.0);
		//sleep(1);
		RobotPosition p = asservdriver->odo_GetPosition();
		logger().info() << p.x << " " << p.y << " " << p.theta << logs::end;
		//sleep(1);

	}

	asservdriver->motion_DoLine(0.200);
	sleep(3);

	asservdriver->motion_FreeMotion();
	asservdriver->motion_ActivateManager(false);
	this->assert(true, "OK");
}

