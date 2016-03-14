/*!
 * \file
 * \brief Implémentation de la classe Chronometer.
 */

#include "Chronometer.hpp"

utils::Chronometer::Chronometer() : stopped_(1)
{
    startCount_.tv_sec = 0;
    startCount_.tv_usec = 0;
    endCount_.tv_sec = 0;
    endCount_.tv_usec = 0;
    endSet_.tv_sec = 0;
    endSet_.tv_usec = 0;
}

void
utils::Chronometer::start()
{
    stopped_ = 0; // reset stop flag
    gettimeofday(&startCount_, NULL);
}

void
utils::Chronometer::stop()
{
    stopped_ = 1; // set chronometer stopped_ flag
    gettimeofday(&endCount_, NULL);
}

long
utils::Chronometer::getElapsedTimeInMicroSec()
{
    if (!stopped_)
    {
        gettimeofday(&endCount_, NULL);
    }

    long seconds = endCount_.tv_sec - startCount_.tv_sec;
    return seconds * 1000000 + (endCount_.tv_usec - startCount_.tv_usec);
}

double
utils::Chronometer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

double
utils::Chronometer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

double
utils::Chronometer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}

timeval
utils::Chronometer::getTime()
{
    timeval result;
    gettimeofday(&result, NULL);
    return result;
}

int utils::Chronometer::checkTimer()
{
    struct timeval ctv;
    gettimeofday(&ctv, NULL);

    if ((ctv.tv_usec >= endSet_.tv_usec) || (ctv.tv_sec > endSet_.tv_sec))
    {
        this->stop();
        return 1;
    }else
        return 0;
}

int utils::Chronometer::setTimer(int usec)
{
    this->start();
    endSet_.tv_usec = startCount_.tv_usec + usec;

    return 1;
}
