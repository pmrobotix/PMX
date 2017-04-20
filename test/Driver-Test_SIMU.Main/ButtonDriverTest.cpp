/*!
 * \file
 * \brief Implémentation de la classe ButtonDriverTest.
 */

#include "ButtonDriverTest.hpp"

#include <unistd.h>
#include <cstdio>

#include "../../src/Common/Utils/ConsoleKeyInput.hpp"
#include "../../src/Log/Logger.hpp"

void test::ButtonDriverTest::suite()
{
	this->testSet();

}

void test::ButtonDriverTest::testSet()
{
	logger().info() << "Starting test with ConsoleKeyInput..." << logs::end;
	char cInput;
	do
	{
		cInput = ConsoleKeyInput::mygetch();
		switch (cInput)
		{
		case 65:
			printf("Up arrow key!\n");
			break;
		case 66:
			printf("Down arrow key!\n");
			break;

		case 67:
			printf("Right arrow key!\n");
			break;
		case 68:
			printf("Left arrow key!\n");
			break;
		case 10:
			printf("Enter key!\n");
			break;
		case 127:
			printf("BACK key!\n");
			break;
		}

		usleep(1000);
	} while (cInput != 10);

	logger().info() << "Starting test with Driver..." << logs::end;

	while (!buttondriver->pressed(BUTTON_ENTER_KEY))
	{
		if (buttondriver->pressed(BUTTON_UP_KEY))
		{
			logger().info() << "UP" << logs::end;
		}
		if (buttondriver->pressed(BUTTON_BACK_KEY))
		{
			logger().info() << "BACK" << logs::end;
		}
	}

	/*
	 ButtonTouch touch = BUTTON_NONE;

	 while (touch != BUTTON_BACK_KEY)
	 {
	 touch =

	 //logger().info() << "touch = " << touch << logs::end;
	 if (touch == BUTTON_UP_KEY)
	 {
	 pos += step;
	 if (pos >= 100) pos = 100;
	 logger().info() << "+" << step << " pos=" << pos << logs::end;

	 //robot.actions().servoObjects().leftDeploy(pos, true);

	 }

	 if (touch == BUTTON_DOWN_KEY)
	 {
	 pos -= step;
	 if (pos <= -100) pos = -100;
	 logger().info() << "-" << step << " pos=" << pos << logs::end;

	 //robot.actions().servoObjects().leftDeploy(pos, true);

	 }

	 if (touch == BUTTON_ENTER_KEY)
	 {
	 logger().info() << "-" << step << " pos=" << pos << logs::end;
	 //robot.actions().servoObjects().release()

	 }

	 if (touch == BUTTON_LEFT_KEY)
	 {
	 num = num + 1;

	 logger().info() << "SERVO " << num << " pos=" << pos << logs::end;

	 }

	 if (touch == BUTTON_RIGHT_KEY)
	 {
	 num = num - 1;

	 logger().info() << "SERVO " << num << " pos=" << pos << logs::end;

	 }

	 }*/

	this->assert(true, "OK");
}

