/*!
 * \file
 * \brief Implémentation de la classe LoggerFactory.
 */

#include "LoggerFactory.hpp"

#include <utility>

#include "../Thread/Mutex.hpp"
#include "Level.hpp"

logs::LoggerFactory::LoggerFactory()
: appenders_(), loggers_(), rootLogger_()
{
    lock=new utils::Mutex();
    this->initialize();
    
    if(rootLogger() == NULL)
    {
        //! \todo Must throw an exception
    	//TODO Exception Must throw an exception
    }
}

logs::LoggerFactory::~LoggerFactory()
{
    lock->lock();
    std::map < std::string, logs::Logger * >::iterator i1 = loggers_.begin();
    for (; i1 != loggers_.end(); i1++)
    {
        delete i1->second;
        i1->second = NULL;
    }
    std::map < std::string, logs::Appender * >::iterator i2 = appenders_.begin();
    for (; i2 != appenders_.end(); i2++)
    {
        delete i2->second;
        i2->second = NULL;
    }
    lock->unlock();
    delete lock;
}

const logs::Logger &
logs::LoggerFactory::logger(const std::string & name)
{
    
    LoggerFactory & instance = logs::LoggerFactory::instance();
   // instance.lock();
    std::map < std::string, logs::Logger * >::iterator value = instance.loggers_.find(name);
    if (value == instance.loggers_.end())
    {
        Logger * logger = new Logger(* instance.rootLogger(), name);
        instance.add(logger);
     //   instance.unlock();
        return * logger;
    }
   // instance.unlock();
    return *(value->second);
}

logs::Appender *
logs::LoggerFactory::appender(const std::string & name)
{
  // lock->lock();
    std::map < std::string, logs::Appender * >::iterator value = this->appenders_.find(name);
    if (value == this->appenders_.end())
    {
 //      lock->unlock();
        return NULL;
    }
    else
    {
     //   lock->unlock();
        return value->second;
    }
}

void
logs::LoggerFactory::add(Logger * logger)
{
//  lock->lock();
    if (logger->name() == "")
    {
       // lock->lock();
        this->rootLogger_ = logger;
       // lock->unlock();
    }
    else
    {
        loggers_.insert(std::make_pair(logger->name(), logger));
    }
   // lock->unlock();
}

void
logs::LoggerFactory::add(const std::string & name, logs::Appender * appender)
{
//   lock->lock();
    appenders_.insert(std::make_pair(name, appender));
  // lock->unlock();
}

void
logs::LoggerFactory::add(const Level & level, const std::string & loggerName, const std::string & appenderName)
{
    Appender * appender = this->appender(appenderName);
    if (appender == NULL)
    {
        //! \todo Must throw an exception
    }
    else
    {
       Logger *log= new Logger(level, loggerName, * appender);
        this->add(log);
	}
}

