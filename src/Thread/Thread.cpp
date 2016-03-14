/*!
 * \file
 * \brief Implémentation de la classe Thread.
 */

#include "Thread.hpp"

#include <sched.h>
#include <syscall.h>
#include <unistd.h>
#include <iostream>

#ifndef EAGAIN
#define EAGAIN 11
#endif

#ifndef EINVAL
#define EINVAL 22
#endif

void *
utils::Thread::entryPoint(void *pthis)
{
	utils::Thread * pt = (utils::Thread *) pthis;
	pt->setState(utils::STARTED);
	pt->execute();
	pt->setState(utils::STOPPED);

	return NULL;
}

utils::Thread::Thread() :
		threadId_(), state_(utils::CREATED)
{
}
/*!
 * \brief rend la main. Cette fonction ne doit pas être utilisée.
 * \deprecated
 */
void utils::Thread::yield()
{
	sched_yield(); //pthread_yield(); !!!!!!!!!!!!!!!!!!!!!!!!!!!!! pour arm-linux-g++
}

/*!
 * \brief Start thread
 * return false si aucun pb.
 */
bool utils::Thread::start(std::string name)
{
	//printf("    utils::Thread::start()\n");
	this->setState(utils::STARTING);

	int code = pthread_create(&threadId_, NULL, utils::Thread::entryPoint, (void *) this);

	if (code == 0)
	{
		//log
		/*
		pid_t tid;
		tid = syscall(SYS_gettid);
		std::cout << "utils::Thread::start::started:" << &threadId_ << " : " << tid << " name="
				<< name << std::endl;
		*/
		//end log
		return false;
	}
	else
	{
		std::cout << "utils::Thread::start::NOT started:" << &threadId_ << " : " << " name=" << name
				<< std::endl;
		std::cout << "utils::Thread::start::pthread_create: NOK \n" << std::endl;
		switch (code)
		{
		case EAGAIN:
			// The  system lacked the necessary resources to create another
			// thread, or the system-imposed limit on the total number of
			// threads in a process {PTHREAD_THREADS_MAX} would be exceeded.
			std::cout << "utils::Thread::start::ERROR:EAGAIN \n" << std::endl;
			break;

		case EINVAL:
			// The value specified by attr is invalid.
			std::cout << "utils::Thread::start::ERROR:EINVAL \n" << std::endl;
			break;
			/*
			 case EPERM :
			 // The caller does not have appropriate permission to set the
			 // required scheduling parameters or scheduling policy.
			 std::cout << "Thread::ERROR:EAGAIN \n" << &threadId_ << std::endl;
			 break;*/

		default:
			std::cout << "utils::Thread::start::ERROR:default \n" << std::endl;
		}
		return true;
	}
}
