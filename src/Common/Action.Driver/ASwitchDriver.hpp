#ifndef ASWITCHDRIVER_HPP_
#define ASWITCHDRIVER_HPP_

#include "../Utils/Macro.hpp"


class ASwitchDriver
{

public:
	/*!
	 * \brief SwitchDriver instance creation.
	 */
	static ASwitchDriver * create();

	/*
	 *\brief pressed.
	 */
	virtual int pressed() = 0;

	/*!
	 * \brief Destructor.
	 */
	virtual ~ASwitchDriver()
	{
	}

protected:

	/*!
	 * \brief Constructor.
	 */
	ASwitchDriver()
	{
	}

};

#endif
