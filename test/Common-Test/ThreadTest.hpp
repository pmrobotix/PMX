/*
 * ThreadTest.hpp
 *
 *  Created on: 6 mars 2017
 *      Author: pmx
 */

#ifndef COMMON_TEST_THREADTEST_HPP_
#define COMMON_TEST_THREADTEST_HPP_

#include "../../src/Thread/Thread.hpp"

#include "../Suite/UnitTest.hpp"
# include <pthread.h>
# include <limits>
# include <cmath>
# include <cassert>

#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include <iostream>
#include <bitset>

//-----------------------------------pthreadAffinityTest

# define gettid() syscall(SYS_gettid)
//-----------------------------------pthreadAffinityTest

// define CPPAD_NULPTR
//# include <cppad/configure.hpp>
# if CPPAD_USE_CPLUSPLUS_2011
# define CPPAD_NULL nullptr
# else
# define CPPAD_NULL 0
# endif
//
# define NUMBER_THREADS 4

# ifdef NDEBUG
# define CHECK_ZERO(expression) expression
# else
# define CHECK_ZERO(expression) assert( expression == 0 );
# endif

namespace
{

// Beginning of Example A.1.1.1c of OpenMP 2.5 standard document ---------

void a1(int n, float *a, float *b)
{
	int i;
	// for some reason this function is missing on some systems
	// assert( pthread_is_multithreaded_np() > 0 );
	for (i = 1; i < n; i++)
		b[i] = (a[i] + a[i - 1]) / 2.0;
	return;
}
// End of Example A.1.1.1c of OpenMP 2.5 standard document ---------------
struct start_arg
{
	int n;
	float* a;
	float* b;
};

void* start_routine(void* arg_vptr)
{
	start_arg* arg = static_cast<start_arg*>(arg_vptr);
	a1(arg->n, arg->a, arg->b);

	void* no_status = CPPAD_NULL;
	pthread_exit(no_status);

	return no_status;
}
}

namespace test
{

class ClassThreadTest: public utils::Thread
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ClassThreadTest.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"ClassThreadTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur de la classe.
	 */
	ClassThreadTest()
	{

	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	inline virtual ~ClassThreadTest()
	{
	}

	virtual void execute();
};

/*!
 * \brief Teste la classe \ref ThreadTest.
 */
class ThreadTest: public UnitTest
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref ThreadTest.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger(
				"ThreadTest");
		return instance;
	}
public:

	/*!
	 * \brief Constructeur par défaut.
	 */
	ThreadTest() :
			UnitTest("test::ThreadTest")
	{
	}

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~ThreadTest()
	{
	}

	/*!
	 * \brief Execute les tests associés à la classe \ref LoggerTest.
	 */
	virtual void suite();

	void testSimpleThreads();

	void testWithOurThreadLib();

	void pthreadAffinityTest();

};

}
#endif /* COMMON_TEST_THREADTEST_HPP_ */
