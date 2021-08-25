/*!
 * \file
 * \brief Définition de l'interface ITimerPosixListener.
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

/*!
 * \brief Cette interface représente une action executée par un timer lorsqu'il
 * atteint son seuil d'execution.
 */
class ITimerPosixListener {
public:

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
    virtual std::string name() {
        return name_;
    }

    /*!
     * \brief Getter sur le temps interval du timer.
     */
    inline int timeSpan_us() {
        return timeSpan_us_;
    }

    void startTimer() {
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
                std::cout << "ERROR ITimerPosixListener Could not install new signal handler, id:"
                        << this->timerID
                        << " name:"
                        << this->name()
                        << std::endl;
                perror("Could not install new signal handler");
                exit(1);
            }
            //std::cout << "Set the timer and therefore it starts, name:" << this->name() << std::endl;
            // Set the timer and therefore it starts...
            if (this->timerID != 0) {
                if (timer_settime(this->timerID, 0, &this->timerSpecs, NULL) == -1) {

                    std::cout << "ERROR ITimerPosixListener Could not start timer, id:" << this->timerID << " name:" << this->name() << std::endl;
                    perror("Could not start timer!");
                    exit(1);
                }
            }
            else {
                std::cout << "ERROR ITimerPosixListener timerID is 0, id:" << this->timerID << " name:" << this->name() << std::endl;
                perror("timerID is 0");
                exit(1);
            }
            //std::cout << "Starting Chrono.., name:" << this->name() << std::endl;
            if (!chrono.started()) chrono.start();
            started_ = true;
        }
        else {
            std::cout << "ERROR ITimerPosixListener timer already started, id:" << this->timerID << " name:" << this->name() << std::endl;
        }
    }

    bool getRunning() {
        return started_;
    }

    bool requestToStop() {
        return requestToStop_;
    }

    void setPause(bool paused) {
        paused_ = paused;
    }

    //deprecated
    void remove(timer_t timerID) {
        started_ = false;
        timer_delete(timerID);
        requestToStop_=false;
    }

    void remove() {
        started_ = false;
        timer_delete(this->timerID);
    }

    /**
     * The signal handler function with extended signature
     */
    //static void alarmFunction(int sigNumb, siginfo_t *si, void *uc) {
    static void alarmFunction(int sigNumb, siginfo_t *si, void *uc) {
        //std::cout << "get the pointer out of the siginfo structure" << std::endl;
// get the pointer out of the siginfo structure and asign it to a new pointer variable
        ITimerPosixListener *ptrTimerPosix = reinterpret_cast<ITimerPosixListener*>(si->si_value.sival_ptr);
// call the member function
        //std::cout << "call the member function, name:" << ptrTimerPosix->name() << " started=" << ptrTimerPosix->started_<< std::endl;
        if (!ptrTimerPosix->paused_ && ptrTimerPosix->started_) ptrTimerPosix->onTimer(ptrTimerPosix->chrono);
        if (ptrTimerPosix->requestToStop()) {
            ptrTimerPosix->onTimerEnd(ptrTimerPosix->chrono);
            ptrTimerPosix->remove();
        }
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~ ITimerPosixListener() {
    }

    // Stored timer ID for alarm
    timer_t timerID;

protected:

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

    void init(std::string label, uint time_us) {
        name_ = label;
        timeSpan_us_ = time_us;
        chrono = utils::Chronometer("ITimerListener-" + name_);
        started_ = false;
        paused_ = false;
        timerID = 0;

        // One second till first occurrence
        this->timerSpecs.it_value.tv_sec = 0;
        this->timerSpecs.it_value.tv_nsec = 1000000; //lancement de la tache dans 1000ms
        // and then all 3 seconds a timer alarm
        if (timeSpan_us_ >= 1000000) this->timerSpecs.it_interval.tv_sec = (int) (timeSpan_us_ / 1000000.0);
        else this->timerSpecs.it_interval.tv_sec = 0;
        this->timerSpecs.it_interval.tv_nsec = (timeSpan_us_ % 1000000) * 1000;

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
}
;

#endif
