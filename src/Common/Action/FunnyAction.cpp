#include "FunnyAction.hpp"

#include "../Action.Driver/AServoDriver.hpp"

using namespace std;

FunnyAction::FunnyAction(Actions & actions) :
		AActionsElement(actions)

{
	servodriver = AServoDriver::create();
}

FunnyAction::~FunnyAction()
{
	servodriver->release(SERVO_FUNNY_ACTION);
}

void FunnyAction::reset()
{
	servodriver->setPosition(SERVO_FUNNY_ACTION, 0);
}

void FunnyAction::activate(int pos)
{
	servodriver->setPosition(SERVO_FUNNY_ACTION, 50);
}

void FunnyAction::release()
{
	servodriver->release(SERVO_FUNNY_ACTION);
}
