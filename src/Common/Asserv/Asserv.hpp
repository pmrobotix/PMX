#ifndef ASSERV_HPP_
#define ASSERV_HPP_

#include "../../Log/Logger.hpp"
#include "../../Log/LoggerFactory.hpp"

/*!
 * Asservissement of the robot.It contains default elements.
 */
class Asserv
{
private:

	/*!
	 * \brief Return \ref Logger linked to \ref Asserv.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("Asserv");
		return instance;
	}

public:

	/*!
	 * \brief Constructor.
	 *
	 */
	Asserv()
	{
	}

	/*!
	 * \brief Destructor.
	 */
	~Asserv()
	{
		logger().debug() << "~Asserv()" << logs::end;
	}

	void startAsservInsa();

	void stopAsservInsa();

};

#endif
