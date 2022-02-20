/*!
 * \file
 * \brief Implementation de la classe MemoryAppender.
 */

#include "../Appender/MemoryAppender.hpp"
#include "../../Common/Utils/json.hpp"

#include <iostream>
#include <chrono>
using namespace std::chrono;

logs::MemoryAppender::MemoryAppender() :
        messages_()
{
    start_ = system_clock::now(); // = std::chrono::high_resolution_clock::now()
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
    system_clock::time_point t = system_clock::now();
    long duration = (duration_cast<microseconds>(t - start_).count());

    std::ostringstream out;
    out << duration << "| " << logger.name() << " " << level.name() << " " << message;
    this->lockMessages();
    this->messages_.push_back(out.str());
    this->unlockMessages();
}

void logs::MemoryAppender::writeMessageOnly(const std::string & message)
{
    std::ostringstream out;
    out << message;
    this->lockMessages();
    this->messages_.push_back(out.str());
    this->unlockMessages();

}
