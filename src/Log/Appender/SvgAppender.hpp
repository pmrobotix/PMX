/*!
 * \file
 * \brief Définition de la classe SvgAppender.
 */

#ifndef LOGS_SVGAPPENDER_HPP_
#define	LOGS_SVGAPPENDER_HPP_

#include <fstream>
#include <string>

#include "../../Log/Logger.hpp"
#include "../../Thread/Mutex.hpp"

namespace logs
{
/*!
 * \brief Implémentation de Appender créant un fichier SVG.
 *
 * Les messages envoyés à cet appender sont considérés comme des éléments SVG.
 */
class SvgAppender: public Appender, public utils::Mutex
{
private:

	/*!
	 * Flux de sortie associé au fichier de résultat.
	 */
	std::ofstream ofs_;

public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	SvgAppender(const std::string & filename);

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~ SvgAppender();

	/*!
	 * \brief Méthode générique de trace d'un message.
	 * \param message
	 *        Message à tracer. Considéré comme un élément SVG valide.
	 */
	virtual void writeMessage(const logs::Logger &, const logs::Level &,
			const std::string & message);

	/*!
	 * \brief Cette méthode clos le fichier de référence.
	 *
	 * Cette méthode est appelée automatiquement par le destructeur de la classe
	 * mais peut être exploitée avant.
	 *
	 * Une fois cette méthode appelée aucun nouveau message ne doit être envoyé
	 * a cet Appender.
	 */
	virtual void flush();
};
}

#endif
