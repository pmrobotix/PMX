#ifndef COMMON_ASSERV_EXTENCODERCONTROL_HPP_
#define COMMON_ASSERV_EXTENCODERCONTROL_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "AAsservElement.hpp"

class AAsservDriver;

class ExtEncoderControl//: public AAsservElement
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ExtEncoderControl.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("ExtEncoderControl");
		return instance;
	}

	AAsservDriver* asservdriver;

public:

	/*!
	 * \brief Constructor.
	 */
	ExtEncoderControl(std::string botid);

	/*!
	 * \brief Destructor.
	 */
	~ExtEncoderControl();

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
