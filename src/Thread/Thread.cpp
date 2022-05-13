/*!
 * \file
 * \brief Implémentation de la classe Thread using linuxthreads.
 */

#include "Thread.hpp"
#include <thread>
#include <sched.h>
#include <iostream>
#include <cerrno>

#include <cstring>
#include <clocale>

//You can't do it the way you've written it because C++ class member functions have a hidden
//this parameter passed in.  pthread_create() has no idea what value of this to use.
//this is the favorite way to handle a thread is to encapsulate it inside a C++ object
void*
utils::Thread::entryPoint(void *pthis)
{
    utils::Thread *pt = (utils::Thread*) pthis;
    pt->setState(utils::STARTED);
    //A supprimer
    //std::cout << "utils::Thread::entryPoint " << pt->name() << " p=" << pt->priority_ << std::endl;
//    if (pt->priority_ > 0) {
//        int err = set_realtime_priority(pt->priority_);
//        if (err < 0) {
//            std::cout << "THREAD " << " entryPoint-set_realtime_priority FAILED with priority=" << " p=" << priority << std::endl;
//        }
//    }
    pt->execute();
    pt->setState(utils::STOPPED);

    return NULL;
}

utils::Thread::Thread() :
        threadId_(), state_(utils::CREATED), priority_(0)
{
}
/*!
 * \brief Donne la main à un autre thread.
 */
void utils::Thread::yield() {
    pthread_yield();
}
/*!
 * \brief Donne la main à un autre thread de meme priorité
 * \The sched_yield() function checks to see if other processes at the same priority as that of the calling process are READY to run.
 * If so, the calling process yields to them and places itself at the end of the READY process queue. The sched_yield() function never yields to a lower priority process.
 *
 */
void utils::Thread::sched_yield() {
    sched_yield();
}

void utils::Thread::sleep_for_micros(int64_t usec) {
    std::this_thread::sleep_for(std::chrono::microseconds(usec));
}

void utils::Thread::sleep_for_millis(int64_t msec) {
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}

void utils::Thread::sleep_for_secs(int64_t sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

/*!
 * \brief Start thread
 *
 * name of the thread
 *
 * priority :0 is low priority ; 99 is high priority ;  default 0
 *
 * return Returns false if the thread was successfully started, true if there was an error starting the thread
 */
bool utils::Thread::start(std::string name, int priority) {
    priority_ = priority;
    name_ = name;
    //logger().debug() << "start() with id=" << &threadId_ << " name=" << name << logs::end;

    this->setState(utils::STARTING);

    int code = pthread_create(&threadId_, NULL, utils::Thread::entryPoint, (void*) this);
    if (code == 0) {
        //LOG
        //std::cout << "THREAD " << threadId_ << " is started name=" << name << " p=" << priority << std::endl;
        //log
        /*
         pid_t tid;
         tid = syscall(SYS_gettid);
         std::cout << "utils::Thread::start::started:" << &threadId_ << " : " << tid << " name="
         << name << std::endl;
         */
        //end log
        if (priority != 0) {
            utils::set_realtime_priority(priority, name, threadId_);
        }
        return false;
    }
    else {
        std::cout << "utils::Thread::start::NOT started:" << threadId_ << " : " << " name=" << name << std::endl;
        std::cout << "utils::Thread::start::pthread_create: NOK \n" << std::endl;
        switch (code) {
            case EAGAIN:
                // The  system lacked the necessary resources to create another
                // thread, or the system-imposed limit on the total number of
                // threads in a process {PTHREAD_THREADS_MAX} would be exceeded.
                std::cout << "utils::Thread::start::ERROR: EAGAIN \n" << std::endl;
                break;

            case EINVAL:
                // The value specified by attr is invalid.
                std::cout << "utils::Thread::start::ERROR: EINVAL \n" << std::endl;
                break;

            case EPERM:
                // The caller does not have appropriate permission to set the
                // required scheduling parameters or scheduling policy.
                std::cout << "utils::Thread::start::ERROR:: EPERM \n" << &threadId_ << std::endl;
                break;

            default:
                std::cout << "utils::Thread::start::ERROR: default \n" << std::endl;
        }
        return true;
    }
}
//chrt -h


// To be able to run the program with a user, got to
// https://stackoverflow.com/questions/10704983/operation-not-permitted-while-setting-new-priority-for-thread
// Edit /etc/security/limits.conf then add 2 lines
// robot hard rtprio 99
// robot soft rtprio 99
//then reboot.
//verify by:
// > ulimit -Hr # show hard limit
//99
//> ulimit -r # show soft limit
//99
//ulimit -Sr 99 # set soft limit
/*
 * sudo nano /etc/systemd/system/console-runner@.service
 * Nice=-20
 * LimitRTPRIO=90
 */
int utils::set_realtime_priority(int p, std::string name, ThreadId thread) {
    if (p >= 0) {
        //priority test
        int ret;
        // We'll operate on the currently running thread.
        //pthread_t this_thread = pthread_self();
        // struct sched_param is used to store the scheduling priority
        struct sched_param params;

        if (p >= sched_get_priority_max(SCHED_FIFO)) p = sched_get_priority_max(SCHED_FIFO);

        //LOG
        //std::cout << "THREAD " << thread << " " << name << " priority changed to " << p << std::endl;

        // We'll set the priority to the maximum.
        params.sched_priority = p;
        //std::cout << "Trying to set thread realtime prio = " << params.sched_priority << std::endl;

        // Attempt to set thread real-time priority to the SCHED_FIFO policy
        ret = pthread_setschedparam(thread, SCHED_FIFO, &params);
        if (ret != 0) {
            // Print the error
            std::cout << "THREAD " << thread << " " << name << " ERROR prio" << std::strerror (errno)<< std::endl;
            //std::cout << "Unsuccessful in setting thread realtime prio" << std::endl;
//while(1){}
            return -2;
        }
        // Now verify the change in thread priority
        int policy = 0;
        ret = pthread_getschedparam(thread, &policy, &params);
        if (ret != 0) {
            std::cout << "THREAD " << thread << " " << name << " ERROR get parameters" << std::endl;
            //std::cout << "Couldn't retrieve real-time scheduling paramers" << std::endl;
//while(1){}
            return -3;
        }

        // Check the correct policy was applied
        if (policy != SCHED_FIFO) {
            std::cout << "Scheduling is NOT SCHED_FIFO!" << std::endl;
//while(1){}
        }
        else {
            //std::cout << "SCHED_FIFO OK" << std::endl;
        }

        // Print thread scheduling priority
        //std::cout << "Thread priority is " << params.sched_priority << std::endl;
 //while(1){}
        return params.sched_priority;
    }
    else return -1;
}

void utils::sleep_for_micros(int64_t usec) {
    std::this_thread::sleep_for(std::chrono::microseconds(usec));
}

void utils::sleep_for_millis(int64_t msec) {
    std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}

void utils::sleep_for_secs(int64_t sec) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

