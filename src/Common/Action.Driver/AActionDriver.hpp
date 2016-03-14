#ifndef AACTIONDRIVER_HPP_
#define AACTIONDRIVER_HPP_

class AActionDriver
{

public:

	/*!
	 * \brief instance creation.
	 */
	static AActionDriver * create(int nb);

	int example1;

#ifdef SIMU
	int exampleSimu;
#endif

	virtual void function(int value);

	/*!
	 * \brief Destructor.
	 */
	virtual ~AActionDriver()
	{
	}

protected:

	/*!
	 * \brief Constructor.
	 */
	AActionDriver()
	{
		example1 = 0;
#ifdef SIMU
		exampleSimu = 0;
#endif
	}

};

#endif
