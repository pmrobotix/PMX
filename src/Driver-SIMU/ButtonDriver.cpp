//drivers...SIMU

#include "ButtonDriver.hpp"

using namespace std;

AButtonDriver * AButtonDriver::create()
{
	return new ButtonDriver();
}

ButtonDriver::ButtonDriver()
{
	lindex = 0;
	back_ = 0;
	enter_ = 0;
	up_ = 0;
	down_ = 0;
	left_ = 0;
	right_ = 0;

	stop_ = false;
	thread_created_ = 0;
}

ButtonDriver::~ButtonDriver()
{

	stop_ = true;
	if (tbutton_.joinable())
		tbutton_.join();

}

//must to be an Non blocking function !!!
bool ButtonDriver::pressed(ButtonTouch button)
{
	if (thread_created_ == 0)
	{
		ButtonDriverWrapper *w_ = new ButtonDriverWrapper(this);
			tbutton_ = w_->buttonThread("ButtonDriver", 0);
			thread_created_ = 1;
	}

	//logger().info() << "pressed() button=" << button << logs::end;

	switch (button)
	{
	case BUTTON_ENTER_KEY:
		if (enter_)
		{
			logger().debug() << "Enter key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_BACK_KEY:
		if (back_)
		{
			logger().debug() << "BACK key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_UP_KEY:
		if (up_)
		{
			logger().debug() << "UP arrow key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_DOWN_KEY:
		if (down_)
		{
			logger().debug() << "DOWN arrow key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_LEFT_KEY:
		if (left_)
		{
			logger().debug() << "LEFT arrow key!" << logs::end;
			return true;
		}
		break;
	case BUTTON_RIGHT_KEY:
		if (right_)
		{
			logger().debug() << "RIGHT arrow key!" << logs::end;
			return true;
		}
		break;
	default:
		return false;
	}




	/*
	 cInput = ConsoleKeyInput::mygetch(); //wait a user action

	 int k = 0;
	 cInput = ConsoleKeyInput::mygetch();
	 logger().debug() << " pressed key=" << (int) cInput << " button= "
	 << (int) button << logs::end;
	 if (cInput == 27) // if ch is the escape sequence with num code 27, k turns 1 to signal the next
	 k = 1;
	 cInput = ConsoleKeyInput::mygetch();
	 if (cInput == 91 && k == 1) // if the previous char was 27, and the current 91, k turns 2 for further use
	 k = 2;
	 cInput = ConsoleKeyInput::mygetch();

	 logger().debug() << " after  key=" << (int) cInput << " button= "
	 << (int) button << logs::end;

	 switch (button)
	 {
	 case BUTTON_ENTER_KEY:
	 if (cInput == 10)
	 {
	 logger().info() << "Enter key!" << logs::end;
	 return true;
	 }
	 break;
	 case BUTTON_BACK_KEY:
	 if (cInput == 127)
	 {
	 logger().info() << "BACK key!" << logs::end;
	 return true;
	 }
	 break;
	 case BUTTON_UP_KEY:
	 if (cInput == 65 && k == 2)
	 {
	 logger().info() << "UP arrow key!" << logs::end;
	 return true;
	 }
	 break;
	 case BUTTON_DOWN_KEY:
	 if (cInput == 66 && k == 2)
	 {
	 logger().info() << "DOWN arrow key!" << logs::end;
	 return true;
	 }
	 break;
	 case BUTTON_LEFT_KEY:
	 if (cInput == 68 && k == 2)
	 {
	 logger().info() << "LEFT arrow key!" << logs::end;
	 return true;
	 }
	 break;
	 case BUTTON_RIGHT_KEY:
	 if (cInput == 67 && k == 2)
	 {
	 logger().info() << "RIGHT arrow key!" << logs::end;
	 return true;
	 }
	 break;
	 default:
	 return false;
	 }
	 */
	return false;
}
