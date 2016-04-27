#ifndef ASWITCHDRIVER_HPP_
#define ASWITCHDRIVER_HPP_


class ASwitchDriver
{

public:
	/*!
	 * \brief SwitchDriver instance creation.
	 */
	static ASwitchDriver * create();

	/*
	 *\brief pressed pour la tirette
	 */
	virtual int pressed() = 0;

	virtual int pressed(unsigned char pin) = 0;

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
