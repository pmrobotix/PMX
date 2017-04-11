#ifndef SIMUCOCOS_BUTTONDRIVER_HPP_
#define SIMUCOCOS_BUTTONDRIVER_HPP_

#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <thread>

#include "../Common/Action.Driver/AButtonDriver.hpp"
#include "../Log/LoggerFactory.hpp"

struct msgform
{
	long mtype;
	char mtext[512];
} msg;

class ButtonDriver: public AButtonDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ButtonDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"ButtonDriver.SIMU");
		return instance;
	}

	int lindex;

	std::thread tbutton_;

public:

	bool back_;
	bool enter_;
	bool up_;
	bool down_;
	bool left_;
	bool right_;

	bool stop_;

	int thread_created_;

	/*!
	 * \brief Constructor.
	 */
	ButtonDriver();

	/*!
	 * \brief Destructor.
	 */
	~ButtonDriver();

	bool pressed(ButtonTouch button);

};

/*
 // http://stackoverflow.com/questions/10673585/start-thread-with-member-function
 Wrapper *w = new Wrapper();
 //std::thread tw1 = w->member1Thread();
 std::thread tw2 = w->member2Thread("hello", 100);
 //tw1.join();
 tw2.join();
 */
class ButtonDriverWrapper
{
public:
	ButtonDriverWrapper(ButtonDriver * buttondriver)
	{
		buttondriver_ = buttondriver;
	}
	~ButtonDriverWrapper()
	{
	}

	ButtonDriver * buttondriver_;

	void checkButton(const char *arg1, int)
	{

		int res;
		int frequete;
		int CLEF_REQUETES = 0x00012345;

		frequete = msgget(CLEF_REQUETES, 0700 | IPC_CREAT);
		if (frequete == -1)
		{
			perror("checkButton() msgget");
			exit(0);

		}

		while (!buttondriver_->stop_)
		{

			buttondriver_->enter_ = false;
			buttondriver_->back_ = false;
			buttondriver_->up_ = false;
			buttondriver_->down_ = false;
			buttondriver_->left_ = false;
			buttondriver_->right_ = false;

			res = msgrcv(frequete, &msg, 512, 0, 0);
			if (res == -1)
			{
				perror("checkButton() msgrcv");
				exit(0);
			}
			else
			{

				//printf("msg=%s\n", msg.mtext);

				std::string str(msg.mtext);
				if (str == "enter")
					buttondriver_->enter_ = true;

				if (str == "back")
					buttondriver_->back_ = true;
				if (str == "up")
					buttondriver_->up_ = true;
				if (str == "down")
					buttondriver_->down_ = true;
				if (str == "right")
					buttondriver_->right_ = true;
				if (str == "left")
					buttondriver_->left_ = true;

			}
			usleep(2000);
		}
	}

	std::thread buttonThread(const char *arg1, int args)
	{
		return std::thread([=]
		{	this->checkButton(arg1, args);});
	}

};

#endif
