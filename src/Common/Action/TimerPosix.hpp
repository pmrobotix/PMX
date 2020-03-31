
#ifndef COMMON_ACTION_TIMERPOSIX_HPP_
#define COMMON_ACTION_TIMERPOSIX_HPP_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

/*
 * https://quirk.ch/2009/07/how-to-use-posix-timer-within-c-classes
 *l
 * https://www.softprayog.in/tutorials/alarm-sleep-and-high-resolution-timers
 *
 */
class TimerPosix
{
public:
//constructor
    TimerPosix() :
    memberVariable(0) {
// Define the timer specification
// One second till first occurrence
        this->timerSpecs.it_value.tv_sec = 1;
        this->timerSpecs.it_value.tv_nsec = 0;
// and then all 3 seconds a timer alarm
        this->timerSpecs.it_interval.tv_sec = 3;
        this->timerSpecs.it_interval.tv_nsec = 0;

// Clear the sa_mask
        sigemptyset(&this->SignalAction.sa_mask);
// set the SA_SIGINFO flag to use the extended signal-handler function
        this->SignalAction.sa_flags = SA_SIGINFO;

// Define sigaction method
// This function will be called by the signal
        this->SignalAction.sa_sigaction = TimerPosix::alarmFunction;

// Define sigEvent
// This information will be forwarded to the signal-handler function
        memset(&this->signalEvent, 0, sizeof(this->signalEvent));
// With the SIGEV_SIGNAL flag we say that there is sigev_value
        this->signalEvent.sigev_notify = SIGEV_SIGNAL;
// Now it's possible to give a pointer to the object
        this->signalEvent.sigev_value.sival_ptr = (void*) this;
// Declare this signal as Alarm Signal
        this->signalEvent.sigev_signo = SIGALRM;

// Install the Timer
        if (timer_create(CLOCK_REALTIME, &this->signalEvent, &this->timerID)
                != 0) { // timer id koennte mit private probleme geben
            perror("Could not creat the timer");
            exit(1);
        }

// Finally install tic as signal handler
        if (sigaction(SIGALRM, &this->SignalAction, NULL)) {
            perror("Could not install new signal handler");
        }
    }

    void start()
    {
// Set the timer and therefore it starts...
        if (timer_settime(this->timerID, 0, &this->timerSpecs, NULL) == -1) {
            perror("Could not start timer:");
        }
    }

    /**
     * The signal handler function with extended signature
     */
    static void alarmFunction(int sigNumb, siginfo_t *si, void *uc)
    {
// get the pointer out of the siginfo structure and asign it to a new pointer variable
        TimerPosix *ptrTimerPosix = reinterpret_cast<TimerPosix*>(si->si_value.sival_ptr);
// call the member function
        ptrTimerPosix->memberAlarmFunction();
    }

// Stored timer ID for alarm
    timer_t timerID;

// Signal blocking set
    sigset_t SigBlockSet;

// The according signal event containing the this-pointer
    struct sigevent signalEvent;

// Defines the action for the signal -> thus signalAction ;-)
    struct sigaction SignalAction;

// The itimerspec structure for the timer
    struct itimerspec timerSpecs;
private:
// demo member
    int memberVariable;

    void memberAlarmFunction()
    {
// of course we can access our object within this member function as we are use to it
        this->memberVariable++;
        std::cout << "Timer expired!! Signal occurred. memberValue=" << this->memberVariable << std::endl;
    }
};



#endif /* COMMON_ACTION_TIMERPOSIX_HPP_ */
