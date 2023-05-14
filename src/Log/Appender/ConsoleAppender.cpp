/*!
 * \file
 * \brief Implémentation de la classe ConsoleAppender.
 */

#include "ConsoleAppender.hpp"

#include <iostream>
#include <list>


logs::ConsoleAppender::~ConsoleAppender() {
    flush();
}

void logs::ConsoleAppender::flush() {
    lockMessages();

    while (this->messages_.size() > 0) {
        std::string message = this->messages_.front();
        //printf("%s\n", message.c_str());
        std::cout << message << std::endl;

        this->messages_.pop_front();
    }
    unlockMessages();
}

void logs::ConsoleAppender::writeMessage(const logs::Logger &logger, const logs::Level &level, const std::string &message) {
    if (!(level == logs::Level::TELEM)) //on ne prend pas en compte la telemetrie
        logs::MemoryAppender::writeMessage(logger, level, message);
}
