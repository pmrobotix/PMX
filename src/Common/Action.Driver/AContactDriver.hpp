#ifndef ACONTACTDRIVER_HPP_
#define ACONTACTDRIVER_HPP_

#include "../Utils/Macro.hpp"


class AContactDriver
{

public:
	/*!
	 * \brief ContactDriver instance creation.
	 */
	static AContactDriver * create();

	/*
	 *\brief pressed.
	 */
	virtual int pressed() = 0;

	/*!
	 * \brief Destructor.
	 */
	virtual ~AContactDriver()
	{
	}

protected:

	/*!
	 * \brief Constructor.
	 */
	AContactDriver()
	{
	}

};

#endif
