/*!
 * \file
 * \brief Définition de l'interface ITimerPosixListener.
 *
 *  LINKER Add -lrt to your link command. timer_create and timer_settime are not part of the C Standard library.
 *  reference
 *  https://quirk.ch/2009/07/how-to-use-posix-timer-within-c-classes
 */

#ifndef COMMON_UTILS_ITIMERPOSIXLISTENER_HPP
#define COMMON_UTILS_ITIMERPOSIXLISTENER_HPP

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include "../Utils/Chronometer.hpp"
#include "../../Thread/Mutex.hpp"

static utils::Mutex mAlarm_;
static utils::Mutex mfct_;
/*!
 * \brief Cette interface représente une action executée par un timer lorsqu'il
 * atteint son seuil d'execution.
 */
class ITimerPosixListener
{
public:

    // Stored timer ID for alarm
    timer_t timerID;

    /*!
     * \brief Actions à executer pour le timer.
     */
    virtual void onTimer(utils::Chronometer chrono) = 0;

    /*!
     * \brief Actions de fin à executer pour le timer.
     */
    virtual void onTimerEnd(utils::Chronometer chrono) = 0;

    /*!
     * \brief Getter sur les infos permettant d'identifier le timer.
     */
    virtual std::string name()
    {
        return name_;
    }

    /*!
     * \brief Getter sur le temps interval du timer.
     */
    inline int timeSpan_us()
    {
        return timeSpan_us_;
    }

    inline void startTimer()
    {
        mfct_.lock();
        if (!started_) {
            // Install the Timer
            //std::cout << "Create the timer, name:" << this->name() << std::endl;
            if (timer_create(CLOCK_REALTIME, &this->signalEvent, &this->timerID) != 0) { // timer id koennte mit private probleme geben
                std::cout << "ERROR ITimerPosixListener Could not create the timer name:" << this->name() << std::endl;
                perror("Could not create the timer");

                exit(1);
            }
            //std::cout << "install new signal handler, name:" << this->name() << std::endl;
            if (sigaction(SIGALRM, &this->SignalAction, NULL)) {
                std::cout << "ERROR ITimerPosixListener Could not install new signal handler, id:" << this->timerID
                        << " name:" << this->name() << std::endl;
                perror("Could not install new signal handler");
                exit(1);
            }
            //std::cout << "Set the timer and therefore it starts, name:" << this->name() << std::endl;
            // Set the timer and therefore it starts...

            if (timer_settime(this->timerID, 0, &this->timerSpecs, NULL) == -1) {

                std::cout << "ERROR ITimerPosixListener Could not start timer, id:" << this->timerID << " name:"
                        << this->name() << std::endl;
                perror("Could not start timer!");
                exit(1);
            }

            //std::cout << "Starting Chrono.., name:" << this->name() << std::endl;
            if (!chrono.started())
                chrono.start();
            started_ = true;
        } else {
            std::cout << "ERROR ITimerPosixListener timer already started, id:" << this->timerID << " name:"
                    << this->name() << std::endl;
        }
        mfct_.unlock();
    }

    inline bool getRunning()
    {
        return started_;
    }

    inline bool requestToStop()
    {
        return requestToStop_;
    }

    inline void setPause(bool paused)
    {
        paused_ = paused;
    }

    //deprecated
    inline void remove(timer_t thistimerID)
    {
        mfct_.lock();

        if (timer_delete(thistimerID) != 0) { // timer id koennte mit private probleme geben
            std::cout << "ERROR ITimerPosixListener remove - Could not delete the timerID: " << thistimerID
                    << std::endl;

        } else {
            started_ = false;
            requestToStop_ = false;
        }

        mfct_.unlock();
    }

    inline void remove()
    {
        mfct_.lock();

        if (timer_delete(this->timerID) != 0) { // timer id koennte mit private probleme geben
            std::cout << "ERROR ITimerPosixListener remove - Could not delete the timerID: " << this->timerID
                    << std::endl;
            //perror("ERROR ITimerPosixListener remove - Could not delete the timer");

        } else {
            started_ = false;
            requestToStop_ = false;
        }

        mfct_.unlock();
    }

    /**
     * The signal handler function with extended signature
     */
    //static void alarmFunction(int sigNumb, siginfo_t *si, void *uc) {
    static void alarmFunction(int sigNumb, siginfo_t *si, void *uc)
    {

        // lock mAlarm_
        mAlarm_.lock();

        //std::cout << "get the pointer out of the siginfo structure" << std::endl;
        mfct_.lock();
// get the pointer out of the siginfo structure and asign it to a new pointer variable
        ITimerPosixListener *ptrTimerPosix = reinterpret_cast<ITimerPosixListener*>(si->si_value.sival_ptr);
// call the member function
        //std::cout << "alarmFunction START, name:" << ptrTimerPosix->name() << " started=" << ptrTimerPosix->started_<< std::endl;
        if (!ptrTimerPosix->paused_ && ptrTimerPosix->started_) {
            //std::cout << "alarmFunction 1a, name:" << ptrTimerPosix->name() << " started=" << ptrTimerPosix->started_<< std::endl;

            ptrTimerPosix->onTimer(ptrTimerPosix->chrono);

        }
        mfct_.unlock();
        //std::cout << "alarmFunction 2a, name:" << ptrTimerPosix->name() << " started=" << ptrTimerPosix->started_                << std::endl;
        if (ptrTimerPosix->requestToStop()) {

            //std::cout << "alarmFunction 1b, name:" << ptrTimerPosix->name() << " started=" << ptrTimerPosix->started_<< std::endl;
            mfct_.lock();
            ptrTimerPosix->onTimerEnd(ptrTimerPosix->chrono);
            mfct_.unlock();

            //std::cout << "alarmFunction 2b, name:" << ptrTimerPosix->name() << " started=" << ptrTimerPosix->started_ << std::endl;



            ptrTimerPosix->remove(ptrTimerPosix->timerID);
            //std::cout << "alarmFunction 3b, name:" << ptrTimerPosix->name() << " started=" << ptrTimerPosix->started_ << std::endl;

        }
        //std::cout << "alarmFunction END, name:" << ptrTimerPosix->name() << " started=" << ptrTimerPosix->started_<< std::endl;

        // unlock
        mAlarm_.unlock();
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~ ITimerPosixListener()
    {
    }

protected:

    //static utils::Mutex mfct_;
    //static utils::Mutex mAlarm_;

    std::string name_;
    long timeSpan_us_;
    utils::Chronometer chrono;

    bool started_;
    bool paused_;
    bool requestToStop_;

    // Signal blocking set
    sigset_t SigBlockSet;

    // The according signal event containing the this-pointer
    struct sigevent signalEvent;

    // Defines the action for the signal -> thus signalAction ;-)
    struct sigaction SignalAction;

    // The itimerspec structure for the timer
    struct itimerspec timerSpecs;

    void init(std::string label, uint time_us)
    {
        mfct_.lock();

        name_ = label;
        timeSpan_us_ = time_us;
        chrono = utils::Chronometer("ITimerListener-" + name_);
        started_ = false;
        paused_ = false;
        timerID = 0;
        requestToStop_ = false;

        // One second till first occurrence
        this->timerSpecs.it_value.tv_sec = 0;  // first execution time
        this->timerSpecs.it_value.tv_nsec = 500000;
        // and then all 3 seconds a timer alarm
        if (timeSpan_us_ >= 1000000)
            this->timerSpecs.it_interval.tv_sec = (int) (timeSpan_us_ / 1000000.0);
        else
            this->timerSpecs.it_interval.tv_sec = 0;
        this->timerSpecs.it_interval.tv_nsec = (int) ((timeSpan_us_ % 1000000) * 1000.0);

//        std::cout << name()
//                << " timerSpecs it_interval.tv_sec="
//                << this->timerSpecs.it_interval.tv_sec
//                << " timerSpecs.it_interval.tv_nsec="
//                << this->timerSpecs.it_interval.tv_nsec
//                << std::endl;

        // Clear the sa_mask
        sigemptyset(&this->SignalAction.sa_mask);
        // set the SA_SIGINFO flag to use the extended signal-handler function
        this->SignalAction.sa_flags = SA_SIGINFO;

        // Define sigaction method
        // This function will be called by the signal
        this->SignalAction.sa_sigaction = &ITimerPosixListener::alarmFunction;

        // Define sigEvent
        // This information will be forwarded to the signal-handler function
        memset(&this->signalEvent, 0, sizeof(this->signalEvent));
        // With the SIGEV_SIGNAL flag we say that there is sigev_value
        this->signalEvent.sigev_notify = SIGEV_SIGNAL;
        // Now it's possible to give a pointer to the object
        this->signalEvent.sigev_value.sival_ptr = (void*) this;
        // Declare this signal as Alarm Signal

        this->signalEvent.sigev_signo = SIGALRM;

        mfct_.unlock();

    }

    /*!
     * \brief Constructeur de la classe.
     */
    ITimerPosixListener() :
            chrono("ITimerPosixListener")
    {
        timerID = 0;
        started_ = false;
        paused_ = false;
        requestToStop_ = false;
        timeSpan_us_ = 0;
        name_ = "ITimerPosixListener_default";
    }
};

#endif
