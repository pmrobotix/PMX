/*!
 * \file
 * \brief Définition de l'interface pour SDLTask.
 */

#ifndef SIMULAUNCHER_MAIN_ISDLTASK_HPP_
#define SIMULAUNCHER_MAIN_ISDLTASK_HPP_

class ISDLTask
{
public:

	ISDLTask()
	{
	}

	virtual ~ISDLTask()
	{
	}

	virtual void run() = 0;

};

#endif /* SIMULAUNCHER_MAIN_ISDLTASK_HPP_ */
