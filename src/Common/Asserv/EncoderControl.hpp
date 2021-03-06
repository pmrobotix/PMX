#ifndef COMMON_ASSERV_ENCODERCONTROL_HPP_
#define COMMON_ASSERV_ENCODERCONTROL_HPP_

#include <string>

#include "../../Log/LoggerFactory.hpp"


class AAsservDriver;

class EncoderControl
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref EncoderControl.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("EncoderControl");
		return instance;
	}

	AAsservDriver* asservdriver;

public:

	/*!
	 * \brief Constructor.
	 */
	EncoderControl(std::string botid); //Asserv & asserv

	/*!
	 * \brief Destructor.
	 */
	~EncoderControl();

	/*!
	 * \brief getEncoderLeft.
	 */
	long getLeftEncoder();

	/*!
	 * \brief getEncoderRight.
	 */
	long getRightEncoder();

	/*!
	 * \brief reset encoders.
	 */
	void reset();

};

#endif
