/*!
 l * \file
 * \brief Implémentation de la classe LoggerFactory.
 */

#include "LoggerFactory.hpp"

//#include <cstdio>
#include <unistd.h>
//#include <cstdlib>
#include <iostream>
#include <utility>

//#include "../Common/Utils/Chronometer.hpp"
#include "Level.hpp"

logs::LoggerFactory::LoggerFactory() :
        appenders_(), loggers_(), rootLogger_(), stop_(false)
{
    this->initialize();

    if (rootLogger() == NULL) {
        printf("ERROR Exception logs::LoggerFactory::LoggerFactory() NO default rootLogger() \n Exit!\n");
        exit(1);
    }
}

void logs::LoggerFactory::stopLog()
{
    stop_ = true;
    this->waitForEnd();
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
    stopLog();
    this->cancel();
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
    //on demarre le thread du logfactory sur le logger (ERROR) par defaut.
    if (logger->name() == "") {

        this->rootLogger_ = logger;
        this->start("LoggerFactory"); //Ne pas mettre de priorité FIFO sinon les lecture EV3 sont très lentes, la config est mise dans  logs::LoggerFactory::initialize() pour chaque robot
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
    this->setPriority();

    //utils::Chronometer chrono("LoggerFactory::execute()");
    //chrono.start();

    while (!stop_) {
        std::map<std::string, Appender *>::iterator it = appenders_.begin();
        for (it = appenders_.begin(); it != appenders_.end(); ++it) {
            //std::cout << it->first << " :: " << it->second << std::endl;

            //long t0 = chrono.getElapsedTimeInMicroSec();
            it->second->flush();
            //long t1 = chrono.getElapsedTimeInMicroSec();

            //std::cout << it->first << " ::flushtime = " << t1-t0<< std::endl;

            this->yield();
        }

        utils::Thread::sleep_for_millis(5); // 5ms? usleep necesaire pour laisser le temps au reste
        this->yield();
    }
    //std::cout << "stop !" << std::endl;
}

