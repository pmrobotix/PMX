/*!
 l * \file
 * \brief Implémentation de la classe LoggerFactory.
 */

#include "LoggerFactory.hpp"

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <utility>

#include "Level.hpp"

logs::LoggerFactory::LoggerFactory() :
        appenders_(), loggers_(), rootLogger_()
{
    this->initialize();

    if (rootLogger() == NULL) {
        printf("ERROR Exception logs::LoggerFactory::LoggerFactory() NO default rootLogger() \n Exit!\n");
        exit(1);
    }
}

logs::LoggerFactory::~LoggerFactory()
{
    std::map<std::string, logs::Logger *>::iterator i1 = loggers_.begin();
    for (; i1 != loggers_.end(); i1++) {
        delete i1->second;
        i1->second = NULL;
    }
    std::map<std::string, logs::Appender *>::iterator i2 = appenders_.begin();
    for (; i2 != appenders_.end(); i2++) {
        delete i2->second;
        i2->second = NULL;
    }
}

const logs::Logger &
logs::LoggerFactory::logger(const std::string & name)
{

    LoggerFactory & instance = logs::LoggerFactory::instance();

    std::map<std::string, logs::Logger *>::iterator value = instance.loggers_.find(name);
    if (value == instance.loggers_.end()) {
        Logger * logger = new Logger(*instance.rootLogger(), name);
        instance.add(logger);

        return *logger;
    }

    return *(value->second);
}

logs::Appender *
logs::LoggerFactory::appender(const std::string & name)
{

    std::map<std::string, logs::Appender *>::iterator value = this->appenders_.find(name);
    if (value == this->appenders_.end()) {

        return NULL;
    } else {

        return value->second;
    }
}

void logs::LoggerFactory::add(Logger * logger)
{

    if (logger->name() == "") {

        this->rootLogger_ = logger;
        this->start("LoggerFactory");

    } else {
        loggers_.insert(std::make_pair(logger->name(), logger));
    }
}

void logs::LoggerFactory::add(const std::string & name, logs::Appender * appender)
{
    appenders_.insert(std::make_pair(name, appender));
}

void logs::LoggerFactory::add(const Level & level, const std::string & loggerName, const std::string & appenderName)
{
    Appender * appender = this->appender(appenderName);
    if (appender == NULL) {
        printf("ERROR Exception logs::LoggerFactory::add() %s, %s\nExit!\n", loggerName.c_str(), appenderName.c_str());
        exit(1);
    } else {
        Logger *log = new Logger(level, loggerName, *appender);
        this->add(log);
    }
}

void logs::LoggerFactory::execute()
{
    while (1) {
        std::map<std::string, Appender *>::iterator it = appenders_.begin();
        for (it = appenders_.begin(); it != appenders_.end(); ++it) {
            //std::cout << it->first << " :: " << it->second << std::endl;
            it->second->flush();
            usleep(50000);
        }
    }
}

