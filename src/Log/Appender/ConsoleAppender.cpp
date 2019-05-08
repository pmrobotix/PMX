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
    for (std::list<std::string>::iterator it = messages_.begin(); it != messages_.end(); it++) {
        (*this->out_) << *it << std::endl;
    }
    unlockMessages();
}

void logs::ConsoleAppender::execute()
{
    while (true) //TODO add a stop of the console appender //TODO merge all logs thread !
    {
        this->printData();
        usleep(100000);
    }
}

void logs::ConsoleAppender::printData()
{
    lockMessages();

    while (this->messages_.size() > 0) {
        std::string message = this->messages_.front();
        (*this->out_) << message << std::endl;
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
void logs::ConsoleAppender::writeMessage(const logs::Logger & logger, const logs::Level & level,
        const std::string & message)
{
    lockMessages();
    if (this->state() == utils::CREATED) {
        std::ostringstream msg;
        msg << "ConsoleAppender created by " << logger.name() << " " << level.name();
        this->start(msg.str());
    }
    unlockMessages();
    logs::MemoryAppender::writeMessage(logger, level, message);

}
