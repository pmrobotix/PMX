/*!
 * \file
 * \brief Définition de la classe ConsoleAppender.
 */

#ifndef LOGS_CONSOLEAPPENDER_HPP_
#define	LOGS_CONSOLEAPPENDER_HPP_

#include <iostream>
#include <sstream>
#include <string>

#include "../../Log/Level.hpp"
#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"
#include "../Appender/MemoryAppender.hpp"

namespace logs
{
/*!
 * \brief Implémentation de Appender pour une écriture des traces
 * sur un flux de sortie de type std::ostream.
 *
 * Cette implémentation se base sur un thread secondaire pour l'écriture
 * dans le fichier des informations.
 *
 * Le constructeur par défaut de cette classe permet de créer un Appender
 * vers la sortie standard.
 */
class ConsoleAppender: public MemoryAppender, public utils::Thread
{
private:

	/*!
	 * Flux de sortie associé.
	 */
	std::ostream * out_;

	void printData();

public:

	/*!
	 * \brief Constructeur par défaut.
	 * L'appender sera associé au flux de sortie standard.
	 */
	ConsoleAppender() :
			out_(&std::cout)
	{
	}

	/*!
	 * \brief Constructeur initialisé de la classe.
	 * \param out Le flux de sortie associé à cet Appender.
	 */
	ConsoleAppender(std::ostream * out) :
			out_(out)
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~ConsoleAppender()
	{
	}

	virtual void writeMessage(const logs::Logger & logger, const logs::Level & level,
			const std::string & message);

	virtual void execute();

	virtual void flush();
};
}

#endif
