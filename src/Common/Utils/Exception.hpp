/*!
 * \file
 * \brief Définition de la classe Exception.
 */

#ifndef COMMON_EXCEPTION_HPP_
#define	COMMON_EXCEPTION_HPP_

#include <string>
#include <exception>

namespace utils
{
/*!
 * \brief Implémentation d'une gestion des exceptions.
 */
class Exception: public std::exception
{
private:

	/*!
	 * \brief Message associé à l'exception.
	 */
	std::string message_;

public:

	/*!
	 * \brief Initialise l'exception avec un message.
	 *
	 * \param message
	 *        Message associé à l'exception.
	 */
	Exception(const std::string & message) :
			std::exception(), message_(message)
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~ Exception() throw ()
	{
	}

	inline const std::string & message() const
	{
		return message_;
	}

	inline void message(const std::string & message)
	{
		this->message_ = message;
	}

	virtual const char* what()
	{
		return this->message().c_str();
	}
};
}

#endif	/* UTILS_EXCEPTION_H */
