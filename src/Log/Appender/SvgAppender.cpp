/*!
 * \file
 * \brief Implémentation de la classe SvgAppender.
 */

#include "SvgAppender.hpp"

#include <list>

logs::SvgAppender::SvgAppender(const std::string & filename) :
        ofs_(filename.c_str())
{
}

logs::SvgAppender::~SvgAppender()
{
    this->flush();
    lockMessages();
    ofs_.close();
    unlockMessages();
}

void logs::SvgAppender::writeMessage(const logs::Logger &, const logs::Level &, const std::string & message)
{
    //ECRITURE DIRECT
	this->lockMessages();
	std::ostringstream out;
	this->ofs_ << message << std::endl;
	this->unlockMessages();

    //EN MEMOIRE
    //logs::MemoryAppender::writeMessageOnly(message);
}

//void logs::SvgAppender::flush()
//{
//	this->ofs_.close();
//}
void logs::SvgAppender::flush()
{
    lockMessages();
    while (this->messages_.size() > 0) {
        std::string message = this->messages_.front();
        ofs_ << message << std::endl;
        this->messages_.pop_front();
    }
    unlockMessages();

}
