/*!
 * \file
 * \brief Implémentation de la classe ConsoleAppender.
 */

#include "../Appender/ConsoleAppender.hpp"

#include <unistd.h>
#include <list>

void logs::ConsoleAppender::flush()
{
    lockMessages();

    while (this->messages_.size() > 0) {
        std::string message = this->messages_.front();
        //printf("%s\n", message.c_str());
        std::cout << message << std::endl;

        this->messages_.pop_front();
    }
    unlockMessages();
}

/*!
 * \brief Méthode générique de trace d'un message.
 * \param logger
 *        Logger de référence du message.
 * \param level
 *        Niveau de référence du message.
 * \param message
 *        Message à tracer.
 */
void logs::ConsoleAppender::writeMessage(const logs::Logger &logger, const logs::Level &level,
        const std::string &message)
{
    logs::MemoryAppender::writeMessage(logger, level, message);
}
