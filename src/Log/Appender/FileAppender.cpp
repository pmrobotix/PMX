/*!
 * \file
 * \brief Implémentation de la classe FileAppender.
 */

#include "FileAppender.hpp"

#include <list>
#include <thread>

logs::FileAppender::~FileAppender()
{
    flush();
    lockMessages();
    ofs_.close();
    unlockMessages();
}

//void logs::FileAppender::execute()
//{
//    while (true) {
//        printData();
//        usleep(500000);
//
//        if (!ofs_) {
//            break;
//        }
//    }
//}

void logs::FileAppender::writeMessage(const logs::Logger & logger, const logs::Level & level,
        const std::string & message)
{
//    lockMessages();
//    if (this->state() == utils::CREATED) {
//        //std::cout << "Thread FileAppender Start : Name=" <<  logger.name() << " " << level.name() << std::endl;
//        //printf(" Thread ConsoleAppender Start. \n");
//
//        std::ostringstream msg;
//        msg << "FileAppender " << logger.name() << " " << level.name();
//        this->start(msg.str());
//    }
//    unlockMessages();
    logs::MemoryAppender::writeMessageOnly(message);

}

void logs::FileAppender::flush()
{
    lockMessages();
    while (this->messages_.size() > 0) {
        std::string message = this->messages_.front();
        ofs_ << message << std::endl;
        this->messages_.pop_front();
        std::this_thread::yield();
    }
    unlockMessages();
    ofs_.flush();
}

