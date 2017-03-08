/*!
 * \file
 * \brief Implémentation de la classe Thread using linuxthreads.
 */

#include "Thread.hpp"

#include <sched.h>
#include <iostream>

//You can't do it the way you've written it because C++ class member functions have a hidden
//this parameter passed in.  pthread_create() has no idea what value of this to use.
//this is the favorite way to handle a thread is to encapsulate it inside a C++ object
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
 * \brief Donne la main à un autre thread.
 */
void utils::Thread::yield()
{
	pthread_yield();
}

/*!
 * \brief Start thread
 * return Returns false if the thread was successfully started, true if there was an error starting the thread
 */
bool utils::Thread::start(std::string name)
{
	logger().debug() << "start() with id=" << &threadId_ << " name=" << name << logs::end;
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
