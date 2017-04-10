#ifndef ACOLORDRIVER_HPP_
#define ACOLORDRIVER_HPP_

#include <string>

class AColorDriver
{

public:

	/*!
	 * \brief instance creation.
	 */
	static AColorDriver * create(std::string botName);

	/*!
	 * \brief Destructor.
	 */
	virtual ~AColorDriver()
	{
	}

	virtual void readRGB() = 0;
	virtual float getTX() = 0;
	virtual float getTY() = 0;

protected:

	/*!
	 * \brief Constructor.
	 */
	AColorDriver()
	{

	}

};

#endif
