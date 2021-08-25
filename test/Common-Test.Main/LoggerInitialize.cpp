/*!
 * \file
 * \brief Ce fichier contient l'initialisation du système de Log.
 */

#include "../../src/Log/Level.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "../../src/Thread/Thread.hpp"
#include "../Suite/UnitTestAppender.hpp"

void logs::LoggerFactory::initialize()
{
    //on ecrase la priorité du loggerFactory
    setPriority(1); //set priority for log per robot !!

    this->add("console", new UnitTestAppender());

    add(logs::Level::DEBUG, "", "console");

}

