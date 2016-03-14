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
	this->lockMessages();
	for (std::list<std::string>::iterator it = messages_.begin(); it != messages_.end(); it++)
	{
		std::cout << *it << std::endl; //TODO not thread safe
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
}

void logs::MemoryAppender::writeMessageOnly(const std::string & message)
{
	std::ostringstream out;
	out << message;
	this->lockMessages();
	this->messages_.push_back(out.str());
	this->unlockMessages();
}
