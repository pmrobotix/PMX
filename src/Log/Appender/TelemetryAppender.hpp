/*!
 * \file
 * \brief Définition de la classe TelemetryAppender.
 */

#ifndef LOGS_TELEMETRYAPPENDER_HPP_
#define LOGS_TELEMETRYAPPENDER_HPP_


#include "../LoggerFactory.hpp"
#include "MemoryAppender.hpp"

#include <iostream>
#include <list>
#include <string>
#include <chrono>

#include <netinet/in.h>

#include "../../Common/Utils/json.hpp"

using namespace std::chrono;

namespace logs {
/*!
 * \brief Implémentation de Appender pour une écriture des traces
 * sur un flux de reseau.
 *
 */
class TelemetryAppender: public MemoryAppender {
private:

    /*!
     * \brief ID du ROBOT.
     */
    std::string id_;

    /*!
     * \brief Liste des messages json enregistrés.
     */
    std::list<std::string> messagesjson_;

    /*!
     * \brief IP reseau du plotjuggler.
     */
    char ip_[100];
    /*!
     * \brief Address socket UDP.
     */
    struct sockaddr_in addr_;
    /*!
     * \brief socket UDP.
     */
    int t_fd; //todo static or not ?? peut-etre dans le cas de plusieurs telemetry?

    /*!
     * \brief Resolution du hostname pour obtenir l'IP.
     * \return 1 if error
     */
    int hostname_to_ip(char * hostname, char* ip);

public:

    /*!
     * \brief Constructeur par défaut.
     * L'appender sera associé au flux de sortie standard.
     */
    TelemetryAppender(std::string Id_Robot, std::string PlotJuggler_hostname);

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~TelemetryAppender() {
        flush();
    }

    void writeMessage(const logs::Logger &logger, const logs::Level &level, const std::string &message);

    void writeMessageWithJsonTime(std::string id, const logs::Logger & logger, const logs::Level &level, const std::string & message);

    void flush();

};
}

#endif
