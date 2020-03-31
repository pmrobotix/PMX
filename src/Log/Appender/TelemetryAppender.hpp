/*!
 * \file
 * \brief Définition de la classe TelemetryAppender.
 */

#ifndef LOGS_TELEMETRYAPPENDER_HPP_
#define	LOGS_TELEMETRYAPPENDER_HPP_

#include <iostream>
#include <string>
#include <chrono>

#include "MemoryAppender.hpp"
using namespace std::chrono;

namespace logs {
/*!
 * \brief Implémentation de Appender pour une écriture des traces
 * sur un flux de reseau.
 *
 */
class TelemetryAppender: public MemoryAppender
{
private:
    int t_fd; //static ??
    std::string id_;
public:

    /*!
     * \brief Constructeur par défaut.
     * L'appender sera associé au flux de sortie standard.
     */
    TelemetryAppender(std::string ID);

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~TelemetryAppender()
    {
    }

    virtual void writeMessage(const logs::Logger &logger, const logs::Level &level, const std::string &message);

    virtual void flush();
};
}

#endif
