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
//while (1)

	//asservdriver->motion_ManagerActivate(false);
	//sleep(3);
	//asservdriver->motion_ManagerActivate(true);

	asservdriver->odo_SetPosition(2.999840, 0.5535, 45.023f);
	RobotPosition p = asservdriver->odo_GetPosition();

	logger().debug() << p.x << " " << p.y << " " << p.theta << logs::end;
	this->assert(true, "OK");
}

