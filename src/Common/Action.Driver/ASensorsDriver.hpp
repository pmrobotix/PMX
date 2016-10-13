#ifndef ASENSORSDRIVER_HPP_
#define ASENSORSDRIVER_HPP_


class ASensorsDriver
{

public:
	/*!
	 * \brief ASensorsDriver instance creation.
	 */
	static ASensorsDriver * create();


	virtual bool front() = 0;

	virtual bool rear() = 0;

	/*!
	 * \brief Destructor.
	 */
	virtual ~ASensorsDriver()
	{
	}

protected:

	/*!
	 * \brief Constructor.
	 */
	ASensorsDriver()
	{
	}

};

#endif
