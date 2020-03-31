/*!
 * \file
 * \brief Définition de la classe ReadWriteTest.
 */

#ifndef TEST_READWRITETEST_HPP
#define TEST_READWRITETEST_HPP

#include <vector>
#include <sys/resource.h>
#include <sys/time.h>
#include <string>

#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/UnitTest.hpp"

namespace test {

/*!
 * \brief Teste la classe \ref pmx::ReadWriteTest.
 */
class ReadWriteTest: public UnitTest
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ReadWriteTest.
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("test::ReadWriteTest");
        return instance;
    }

public:

    /*!
     * \brief Constructeur de la classe.
     */
    ReadWriteTest() :
            UnitTest("ReadWriteTest - Read + write")
    {
    }

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~ReadWriteTest()
    {
    }

    virtual void suite();

    /*!
     * \brief Teste la méthode \ref do_benchmark_fastest_read_fread_ifstream_or_mmap.
     */
    void do_benchmark_fastest_read_fread_ifstream_or_mmap();
    void do_io_benchmark(std::string type, std::string f, std::string t);
    void do_io_benchmark_posix_readw(std::string filename, int data);
    void do_io_benchmark_posix_readw_optimised(std::string filename, int wdata, int times);

    int testPosixIOOpenFile(const char *outFile);
    void testPosixIOWInt(int fd, int data);
    void testPosixIOWString(int fd, std::string data);
    void testPosixIOR(int fd, std::vector<char> &inBuffer);

    void testPosixIORead(const char *inFile, std::vector<char> &inBuffer);
    void testPosixIOWriteInt(const char *outFile, int data);
    void testPosixIOWriteString(const char *outFile, std::string data);
};

class WallClockTimer
{
public:
    struct timeval t1, t2;
    WallClockTimer() :
            t1(), t2()
    {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    void reset()
    {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    unsigned long long elapsed()
    {
        return ((t2.tv_sec - t1.tv_sec) * 1000ULL * 1000ULL) + ((t2.tv_usec - t1.tv_usec));
    }
    unsigned long long split()
    {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};

class CPUTimer
{
public:
    //clock_t t1, t2;
    struct rusage t1, t2;

    CPUTimer() :
            t1(), t2()
    {
        getrusage(RUSAGE_SELF, &t1);
        //t1 = clock();
        t2 = t1;
    }
    void reset()
    {
        getrusage(RUSAGE_SELF, &t1);
        //t1 = clock();
        t2 = t1;
    }
    // proxy for userelapsed
    unsigned long long elapsed()
    {
        return totalelapsed(); //userelapsed();
    }

    unsigned long long totalelapsed()
    {
        return userelapsed() + systemelapsed();
    }
    // returns the *user* CPU time in micro seconds (mu s)
    unsigned long long userelapsed()
    {
        return ((t2.ru_utime.tv_sec - t1.ru_utime.tv_sec) * 1000ULL * 1000ULL)
                + ((t2.ru_utime.tv_usec - t1.ru_utime.tv_usec));
    }

    // returns the *system* CPU time in micro seconds (mu s)
    unsigned long long systemelapsed()
    {
        return ((t2.ru_stime.tv_sec - t1.ru_stime.tv_sec) * 1000ULL * 1000ULL)
                + ((t2.ru_stime.tv_usec - t1.ru_stime.tv_usec));
    }

    unsigned long long split()
    {
        getrusage(RUSAGE_SELF, &t2);
        return elapsed();
    }
};

}

#endif
