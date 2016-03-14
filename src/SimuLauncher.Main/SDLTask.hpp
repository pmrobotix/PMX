#ifndef SIMULAUNCHER_MAIN_SDLTASK_HPP_
#define SIMULAUNCHER_MAIN_SDLTASK_HPP_

class SDLTask
{
public:

	SDLTask()
	{
	}

	virtual ~SDLTask()
	{
	}

	virtual void run() = 0;

};

#endif /* SIMULAUNCHER_MAIN_SDLTASK_HPP_ */
