/*
 * ThreadTest.cpp
 *
 *  Created on: 6 mars 2017
 *      Author: pmx
 */

#include "ThreadTest.hpp"

#include <stddef.h>

#include "../../src/Log/Logger.hpp"

void test::ThreadTest::suite()
{

    testSimpleThreads();

    pthreadAffinityTest();

    testWithOurThreadLib();
}

//-----------------------------------pthreadAffinityTest

uint8_t get_affinity_mask()
{
    cpu_set_t cpu_set;
    if (sched_getaffinity(gettid(), sizeof(cpu_set_t), &cpu_set) == -1) {
        std::cout << "Failed: sched_getaffinity()" << std::endl;
        return -1;
    }

    uint8_t affinityMask = 0;
    for (int i = 0; i < 8; i++) {
        if (CPU_ISSET(i, &cpu_set)) {
            affinityMask |= (1 << i);
        }
    }

    return affinityMask;
}

void* thread_func(void *args)
{
    uint8_t affinityMask = get_affinity_mask();

    std::bitset<8> affinityMaskBin(affinityMask);
    std::cout << "CPU affinity mask is: " << affinityMaskBin << std::endl;

    pthread_exit(0);
}

void test::ThreadTest::pthreadAffinityTest()
{
    //reference https://gist.github.com/brandonto/b11f3b129c7360f8b7f6a70fee4224ef
    //std::cout << "pthreadAffinityTest" << std::endl;
    logger().info() << "pthreadAffinityTest" << logs::end;
    void *res;

    pthread_t thread1;
    pthread_t thread2;

    pthread_attr_t thread1_attr;
    pthread_attr_t thread2_attr;

    uint8_t affinityMask = get_affinity_mask();

    std::bitset<8> affinityMaskBin(affinityMask);
    //std::cout << "CPU affinity mask of main process is: " << affinityMaskBin
    //		<< std::endl;
    logger().info() << "CPU affinity mask of main process is: " << affinityMaskBin << logs::end;

    if ((pthread_attr_init(&thread1_attr) != 0) || (pthread_attr_init(&thread2_attr) != 0)) {
        std::cout << "Failed: pthread_attr_init()" << std::endl;
    }
    /*
     cpu_set_t cpu_set;
     CPU_ZERO(&cpu_set);
     CPU_SET(1, &cpu_set);
     CPU_SET(3, &cpu_set);

     if (pthread_attr_setaffinity_np(&thread1_attr, sizeof(cpu_set_t), &cpu_set)
     != 0)
     {
     std::cout << "Failed: pthread_attr_setaffinity_np()" << std::endl;
     }

     CPU_ZERO(&cpu_set);
     CPU_SET(1, &cpu_set);
     CPU_SET(2, &cpu_set);
     CPU_SET(3, &cpu_set);

     if (pthread_attr_setaffinity_np(&thread2_attr, sizeof(cpu_set_t), &cpu_set)
     != 0)
     {
     std::cout << "Failed: pthread_attr_setaffinity_np()" << std::endl;
     }
     */
    if (pthread_create(&thread1, &thread1_attr, thread_func, NULL) != 0) {
        std::cout << "Failed: pthread_create()" << std::endl;
    }

    if (pthread_join(thread1, &res) != 0) {
        std::cout << "Failed: pthread_join()" << std::endl;
    }

    if (pthread_create(&thread2, &thread2_attr, thread_func, NULL) != 0) {
        std::cout << "Failed: pthread_create()" << std::endl;
    }

    if (pthread_join(thread2, &res) != 0) {
        std::cout << "Failed: pthread_join()" << std::endl;
    }
    logger().info() << "pthreadAffinityTest - end" << logs::end;
}

//-----------------------------------pthreadAffinityTest

void test::ThreadTest::testSimpleThreads()
{
    //reference : https://www.coin-or.org/CppAD/Doc/a11c_pthread.cpp.htm

    logger().info() << "testSimpleThreads" << logs::end;

    bool ok = true;

    // Test setup
    int i, j, n_total = 10;
    float *a = new float[n_total];
    float *b = new float[n_total];
    for (i = 0; i < n_total; i++)
        a[i] = float(i);

    // number of threads
    int n_thread = NUMBER_THREADS;
    // the threads
    pthread_t thread[NUMBER_THREADS];
    // arguments to start_routine
    struct start_arg arg[NUMBER_THREADS];
    // attr
    pthread_attr_t attr;
    CHECK_ZERO(pthread_attr_init(&attr));
    CHECK_ZERO(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE));
    //
    // Break the work up into sub work for each thread
    int n = n_total / n_thread;
    arg[0].n = n;
    arg[0].a = a;
    arg[0].b = b;
    for (j = 1; j < n_thread; j++) {
        arg[j].n = n + 1;
        arg[j].a = arg[j - 1].a + n - 1;
        arg[j].b = arg[j - 1].b + n - 1;
        if (j == (n_thread - 1))
            arg[j].n = n_total - j * n + 1;
    }
    for (j = 0; j < n_thread; j++) {     // inform each thread of which block it is working on
        void *arg_vptr = static_cast<void*>(&arg[j]);
        CHECK_ZERO(pthread_create(&thread[j], &attr, start_routine, arg_vptr));
    }
    for (j = 0; j < n_thread; j++) {
        void *no_status = CPPAD_NULL;
        CHECK_ZERO(pthread_join(thread[j], &no_status));
    }

    // check the result
    float eps = 100. * std::numeric_limits<float>::epsilon();
    for (i = 1; i < n; i++)
        ok &= std::fabs((2. * b[i] - a[i] - a[i - 1]) / b[i]) <= eps;

    delete[] a;
    delete[] b;

    logger().info() << "testSimpleThreads - end" << logs::end;
}

void test::ClassThreadTest::execute()
{
    logger().info() << "test::ClassThreadTest::execute()" << logs::end;
}

void test::ThreadTest::testWithOurThreadLib()
{
    logger().info() << "testWithOurThreadLib" << logs::end;

    test::ClassThreadTest *test = new test::ClassThreadTest();
    test->start("ClassThreadTest");
    logger().info() << "thread launched" << logs::end;
    test->waitForEnd();

    logger().info() << "testWithOurThreadLib - end" << logs::end;
}
