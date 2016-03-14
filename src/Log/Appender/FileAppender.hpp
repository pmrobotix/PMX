/*!
 * \file
 * \brief Définition de la classe FileAppender.
 */

#ifndef LOGS_FILEAPPENDER_HPP
#define	LOGS_FILEAPPENDER_HPP

#include <sstream>
#include <string>
#include <fstream>

#include "../../Log/Level.hpp"
#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"
#include "../Appender/MemoryAppender.hpp"


namespace logs
{
/*!
 * \brief Implémentation de Appender pour une sortie sur fichier.
 *
 * Cette implémentation se base sur un thread secondaire pour l'écriture
 * dans le fichier des informations.
 */
class FileAppender: public MemoryAppender, public utils::Thread
{
private:

	/*!
	 * Flux de sortie associé.
	 */
	std::ofstream ofs_;

	void printData();

public:

	/*!
	 * \brief Constructeur initialisé de la classe.
	 * \param filename Le nom du fichier associé à cet Appender.
	 */
	FileAppender(const std::string & filename)
			: ofs_(filename.c_str())
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~FileAppender();

	/*!
	 * \brief Méthode générique de trace d'un message.
	 * \param logger
	 *        Logger de référence du message.
	 * \param level
	 *        Niveau de référence du message.
	 * \param message
	 *        Message à tracer.
	 */
	virtual void writeMessage(const logs::Logger & logger, const logs::Level & level,
			const std::string & message);

	virtual void execute();

	virtual void flush();
};
}

#endif
