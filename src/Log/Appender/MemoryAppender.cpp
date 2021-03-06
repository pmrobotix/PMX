/*!
 * \file
 * \brief Implementation de la classe MemoryAppender.
 */

#include "../Appender/MemoryAppender.hpp"

#include <iostream>

logs::MemoryAppender::MemoryAppender() :
        messages_()
{
}

logs::MemoryAppender::~MemoryAppender()
{
    this->flush();
}

void logs::MemoryAppender::lockMessages()
{
    this->lock();
}
void logs::MemoryAppender::unlockMessages()
{
    this->unlock();
}

void logs::MemoryAppender::flush()
{
    //printf("logs::MemoryAppender::flush\n");
    this->lockMessages();
    for (std::list<std::string>::iterator it = messages_.begin(); it != messages_.end(); it++) {
        std::cout << *it << std::endl;
    }

    this->messages_.clear();
    this->unlockMessages();
}
void logs::MemoryAppender::writeMessage(const logs::Logger & logger, const logs::Level & level,
        const std::string & message)
{

    std::ostringstream out;
    out << logger.name() << " " << level.name() << " " << message;
    this->lockMessages();
    this->messages_.push_back(out.str());
    this->unlockMessages();
    // printf("logs::MemoryAppender::writeMessage %s\n", message.c_str());
}

void logs::MemoryAppender::writeMessageOnly(const std::string & message)
{

    std::ostringstream out;
    out << message;
    this->lockMessages();
    this->messages_.push_back(out.str());
    this->unlockMessages();

    //printf("logs::MemoryAppender::writeMessageOnly %s\n", message.c_str());
}
