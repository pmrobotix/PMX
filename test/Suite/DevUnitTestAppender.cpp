/*!
 * \file
 * \brief Implémentation de la classe UnitTestAppender.
 */

#include "DevUnitTestAppender.hpp"

#include <list>
#include <sstream>

#include "../../src/Log/Level.hpp"

DevUnitTestAppender::DevUnitTestAppender()
{
	this->indent_ = 0;
}

DevUnitTestAppender::~DevUnitTestAppender()
{
}

void DevUnitTestAppender::writeMessage(const logs::Logger & logger,
		const logs::Level & level,
		const std::string & message)
{
	if (level == logs::Level::INFO)
	{
		this->lock();
		std::ostringstream out;
		for (int i = 0; i < this->indent_; i++)
		{
			out << "   ";
		}
		out << message;
		this->messages_.push_back(out.str());
		this->unlock();
	}
	else
	{
		logs::MemoryAppender::writeMessage(logger, level, message);
	}
}

void DevUnitTestAppender::increaseIndent()
{
	this->lock();
	this->indent_++;
	this->unlock();
}

void DevUnitTestAppender::decreaseIndent()
{
	this->lock();
	this->indent_--;
	if (this->indent_ < 0)
	{
		this->indent_ = 0;
	}
	this->unlock();
}

bool DevUnitTestAppender::expectedError(const std::string& message)
{
	this->lock();
	for (std::list<std::string>::iterator it = messages_.begin(); it != messages_.end(); it++)
	{
		if (*it == message)
		{
			this->messages_.erase(it);
			this->unlock();
			return true;
		}
	}
	this->unlock();
	return false;
}

void DevUnitTestAppender::cleanMessages()
{
	this->lock();
	this->messages_.clear();
	this->unlock();
}
