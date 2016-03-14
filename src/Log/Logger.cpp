/*!
 * \file
 * \brief Implémentation de la classe Logger.
 */

#include "Logger.hpp"

#include <iostream>

logs::Logger::Logger(const logs::Level & level, const std::string & name, logs::Appender & appender)
: level_(level), name_(name), appender_(appender)
{
}

logs::Logger::Logger(const logs::Logger & parent, const std::string & name)
: level_(parent.level_), name_(name), appender_(parent.appender_)
{
}

void
logs::Logger::writeMessage(const logs::Level & level, const std::string & message) const
{
    if (isActive(level))
    {
        appender_.writeMessage(*this, level, message);
    }
}

logs::Logger::LoggerBuffer::LoggerBuffer(const logs::Logger & logger, const logs::Level & level)
: logger_(logger), level_(level)
{
    if(&logger==NULL){
        std::cout<<"NULL logger"<<std::endl;
    }
    if (logger.isActive(level))
    {
        stream_ = new std::ostringstream();
    }
    else
    {
        stream_ = NULL;
    }
}

logs::Logger::LoggerBuffer::~LoggerBuffer()
{
    if (stream_ != NULL)
    {
        delete stream_;
        stream_ = NULL;
    }
}

void
logs::end(logs::Logger::LoggerBuffer& buffer)
{
    buffer.end();
}
