/*
 * ch.cpp
 *
 *  Created on: Jun 28, 2022
 *      Author: pmx
 */

#include "../../Thread/Mutex.hpp"
#include "ch.h"

utils::Mutex m_asserv_mutex_n;



void chDbgAssert(bool condition, std::string msg)
{

}

void chSysLock()
{
    m_asserv_mutex_n.lock();
}

void chSysUnlock()
{
    m_asserv_mutex_n.unlock();
}
