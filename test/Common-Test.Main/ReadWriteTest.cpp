/*!
 * \file
 * \brief Implémentation de la classe ReadWriteTest.
 * Benchmark test
 * https://lemire.me/blog/2012/06/26/which-is-fastest-read-fread-ifstream-or-mmap/
 * https://github.com/lemire/Code-used-on-Daniel-Lemire-s-blog/blob/master/2012/06/26/ioaccess.cpp
 */

#include "ReadWriteTest.hpp"

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

#include <fcntl.h>

#if defined(__unix__) || defined (__CYGWIN__)
#include <unistd.h>
#else
    #include <io.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif
#include <chrono>
#include <iostream>
#include <functional>
#include <fstream>
#include <map>

#include <cmath>

using namespace std::chrono;

struct measure
{
    template<typename F, typename ...Args>
    static std::chrono::milliseconds::rep ms(F func, Args &&... args)
    {
        auto start = system_clock::now();
        func(std::forward<Args>(args)...);
        auto stop = system_clock::now();

        return duration_cast<milliseconds>(stop - start).count();
    }

    template<typename F, typename ...Args>
    static std::chrono::microseconds::rep us(F func, Args &&... args)
    {
        auto start = system_clock::now();
        func(std::forward<Args>(args)...);
        auto stop = system_clock::now();

        return duration_cast<microseconds>(stop - start).count();
    }
};

void testCFileIO(const char *inFile, const char *outFile, std::vector<char> &inBuffer)
{
    FILE *in = ::fopen(inFile, "rb");
    if (!in) {
        std::cout << "Can't open input file: " << inFile << std::endl;
        return;
    }

    FILE *out = ::fopen(outFile, "wb");
    if (!out) {
        std::cout << "Can't open output file: " << outFile << std::endl;
        return;
    }

    fseek(in, 0, SEEK_END);
    size_t inFileSize = ::ftell(in);
    fseek(in, 0, SEEK_SET);

    for (size_t bytesLeft = inFileSize, chunk = inBuffer.size(); bytesLeft > 0; bytesLeft -= chunk) {
        if (bytesLeft < chunk) {
            chunk = bytesLeft;
        }

        ::fread(&inBuffer[0], 1, chunk, in);
        ::fwrite(&inBuffer[0], 1, chunk, out);
    }

    ::fclose(out);
    ::fclose(in);
}

void testCppIO(const char *inFile, const char *outFile, std::vector<char> &inBuffer)
{
    std::ifstream in(inFile, std::ifstream::binary);
    if (!in.is_open()) {
        std::cout << "Can't open input file: " << inFile << std::endl;
        return;
    }

    std::ofstream out(outFile, std::ofstream::binary);
    if (!out.is_open()) {
        std::cout << "Can't open output file: " << outFile << std::endl;
        return;
    }

    in.seekg(0, std::ifstream::end);
    size_t inFileSize = in.tellg();
    in.seekg(0, std::ifstream::beg);

    for (size_t bytesLeft = inFileSize, chunk = inBuffer.size(); bytesLeft > 0; bytesLeft -= chunk) {
        if (bytesLeft < chunk) {
            chunk = bytesLeft;
        }

        in.read(&inBuffer[0], chunk);
        out.write(&inBuffer[0], chunk);
    }
}

void testPosixIO(const char *inFile, const char *outFile, std::vector<char> &inBuffer)
{
    int in = ::open(inFile, O_RDONLY | O_BINARY);
    if (in < 0) {
        std::cout << "Can't open input file: " << inFile << std::endl;
        return;
    }

    int out = ::open(outFile, O_CREAT | O_WRONLY | O_BINARY, 0666);
    if (out < 0) {
        std::cout << "Can't open output file: " << outFile << std::endl;
        return;
    }

    size_t inFileSize = ::lseek(in, 0, SEEK_END);
    ::lseek(in, 0, SEEK_SET);

    for (size_t bytesLeft = inFileSize, chunk = inBuffer.size(); bytesLeft > 0; bytesLeft -= chunk) {
        if (bytesLeft < chunk) {
            chunk = bytesLeft;
        }

        ::read(in, &inBuffer[0], chunk);
        //std::cout << "file read: " <<  inBuffer[0] << std::endl;

        ::write(out, &inBuffer[0], chunk);
    }

    ::close(out);
    ::close(in);

    //printf("buf: %s", inBuffer[0]);
    //std::cout << "buf: " <<  inBuffer[0] << std::endl;
}
//__________________________________________________________________________________________________________________
// write N * 512 integers
void fillFile(int N, char *name)
{
    FILE *fd = ::fopen(name, "wb");
    if (fd == NULL) {
        cerr << "problem" << endl;
        return;
    }
    int numbers[512]; //int numbers[512];
    for (int k = 0; k < 512; ++k) //for (int k = 0; k < 512; ++k)
        numbers[k] = k; // whatever
    for (int t = 0; t < N; ++t) {
        int size = 512; //int size = 512;
        size_t realsize;
        int err, error;
        if ((realsize = fwrite(&size, sizeof(int), 1, fd)) != 1) {
            err = errno;
            error = ferror(fd);
            if (error) {
                cout << "We written only " << realsize << " bytes" << endl;
                cout << "[ERROR] " << strerror(err) << endl;
                exit(EXIT_FAILURE);
            }
            return;
        }
        if ((realsize = fwrite(&numbers[0], sizeof(int), 512, fd)) != 512) {
            err = errno;
            error = ferror(fd);
            if (error) {
                cout << "We written only" << realsize << "bytes" << endl;
                cout << "[ERROR] " << strerror(err) << endl;
                exit(EXIT_FAILURE);
            }
            return;
        }
    }
    ::fclose(fd);
}

int doSomeComputation(int *numbers, size_t size)
{
    int answer = 0;
    // totally arbitrary
    for (size_t k = 0; k < size; k += 2) {
        answer += numbers[k];
    }
    for (size_t k = 1; k < size; k += 2) {
        answer += numbers[k] * 2;
    }
    return answer;
}

int testfread(char *name, int N)
{
    int answer = 0;
    FILE *fd = ::fopen(name, "rb");
    if (fd == NULL) {
        cerr << "problem" << endl;
        return -1;
    }
    vector<int> numbers(512); //vector<int> numbers(512);

    for (int t = 0; t < N; ++t) {
        int size = 0;
        if (fread(&size, sizeof(int), 1, fd) != 1) {
            cout << "Data can't be read???" << endl;
            return -1;
        }
        numbers.resize(size);
        if (fread(&numbers[0], sizeof(int), numbers.size(), fd) != numbers.size()) {
            cout << "Data can't be read???" << endl;
            return -1;
        }
        answer += doSomeComputation(&numbers[0], numbers.size());
    }
    ::fclose(fd);
    return answer;
}

int testwithCpp(char *name, int N)
{
    int answer = 0;
    ifstream in(name, ios::binary);
    vector<int> numbers(512); //vector<int> numbers(512);
    for (int t = 0; t < N; ++t) {
        int size = 0;
        in.read(reinterpret_cast<char*>(&size), sizeof(int));
        numbers.resize(size);
        in.read(reinterpret_cast<char*>(&numbers[0]), sizeof(int) * size);
        answer += doSomeComputation(&numbers[0], numbers.size());
    }
    in.close();
    return answer;
}

int testfreadwithsetbuffer(char *name, int N)
{
    int answer = 0;
    FILE *fd = ::fopen(name, "rb");
    setvbuf(fd, NULL, _IOFBF, 1024 * 4); // large buffer
    if (fd == NULL) {
        cerr << "problem" << endl;
        return -1;
    }
    vector<int> numbers(512); //vector<int> numbers(512);
    for (int t = 0; t < N; ++t) {
        int size = 0;
        if (fread(&size, sizeof(int), 1, fd) != 1) {
            cout << "Data can't be read???" << endl;
            return -1;
        }
        numbers.resize(size);
        if (fread(&numbers[0], sizeof(int), numbers.size(), fd) != numbers.size()) {
            cout << "Data can't be read???" << endl;
            return -1;
        }
        answer += doSomeComputation(&numbers[0], numbers.size());
    }
    ::fclose(fd);
    return answer;
}

int testfreadwithlargebuffer(char *name, int N)
{
    int answer = 0;
    FILE *fd = ::fopen(name, "rb");
    setvbuf(fd, NULL, _IOFBF, 1024 * 1024 * 32); // large buffer
    if (fd == NULL) {
        cerr << "problem" << endl;
        return -1;
    }
    vector<int> numbers(512); //vector<int> numbers(512);
    for (int t = 0; t < N; ++t) {
        int size = 0;
        if (fread(&size, sizeof(int), 1, fd) != 1) {
            cout << "Data can't be read???" << endl;
            return -1;
        }
        numbers.resize(size);
        if (fread(&numbers[0], sizeof(int), numbers.size(), fd) != numbers.size()) {
            cout << "Data can't be read???" << endl;
            return -1;
        }
        answer += doSomeComputation(&numbers[0], numbers.size());
    }
    ::fclose(fd);
    return answer;
}

int testwmmap(char *name, int N, bool advise, bool shared)
{
    int answer = 0;
    int fd = ::open(name, O_RDONLY);
    size_t length = N * (512 + 1) * 4; //size_t length = N * (512 + 1) * 4;

    // for Linux:
#ifdef __linux__
    int *addr = reinterpret_cast<int*>(mmap(NULL, length, PROT_READ,
    MAP_FILE | (shared ? MAP_SHARED : MAP_PRIVATE) | MAP_POPULATE, fd, 0));
#else
   int *  addr = reinterpret_cast<int *>(mmap(NULL, length, PROT_READ, MAP_FILE | (shared?MAP_SHARED:MAP_PRIVATE), fd, 0));
#endif
    int *initaddr = addr;
    if (addr == MAP_FAILED) {
        cout << "Data can't be mapped???" << endl;
        return -1;
    }
    if (advise)
        if (madvise(addr, length, MADV_SEQUENTIAL | MADV_WILLNEED) != 0)
            cerr << " Couldn't set hints" << endl;
    close(fd);
    for (int t = 0; t < N; ++t) {
        int size = *addr++;
        answer += doSomeComputation(addr, size);
        addr += size;
    }
    munmap(initaddr, length);
    return answer;
}

int testread(char *name, int N)
{
    int answer = 0;
    int fd = ::open(name, O_RDONLY);
    if (fd < 0) {
        cerr << "problem" << endl;
        return -1;
    }
    vector<int> numbers(512); //vector<int> numbers(512);
    for (int t = 0; t < N; ++t) {
        int size = 0;
        if (read(fd, &size, sizeof(int)) != sizeof(int)) {
            cout << "Data can't be read???" << endl;
            return -1;
        }
        numbers.resize(size);
        if (read(fd, &numbers[0], sizeof(int) * numbers.size()) != sizeof(int) * numbers.size()) {
            cout << "Data can't be read???" << endl;
            return -1;
        }
        answer += doSomeComputation(&numbers[0], numbers.size());
    }
    ::close(fd);
    return answer;
}

void test::ReadWriteTest::suite()
{

//    for (unsigned int i = 0; i < 20; ++i) {
//        do_io_benchmark_posix_readw("/sys/class/tacho-motor/motor0/position_sp", i);
//    }

    do_io_benchmark_posix_readw_optimised("/sys/class/lego-sensor/sensor1/value0", 10, 20);

    //do_io_benchmark_posix_readw_optimised("aa.txt", -500, 20);



    //do_benchmark_fastest_read_fread_ifstream_or_mmap(); //commenté a cause du warning: the use of `tmpnam' is dangerous, better use `mkstemp'
    do_io_benchmark("c", "aa.txt", "1");
    do_io_benchmark("c", "aa.txt", "10");
    do_io_benchmark("c", "aa.txt", "100");

    do_io_benchmark("posix", "aa.txt", "1");
    do_io_benchmark("posix", "aa.txt", "10");
    do_io_benchmark("posix", "aa.txt", "100");

    do_io_benchmark("c++", "aa.txt", "1");
    do_io_benchmark("c++", "aa.txt", "10");
    do_io_benchmark("c++", "aa.txt", "100");
}

void test::ReadWriteTest::testPosixIORead(const char *inFile, std::vector<char> &inBuffer)
{
    int in = ::open(inFile, O_RDONLY);
    if (in < 0) {
        std::cout << "Can't open input file: " << inFile << std::endl;
        return;
    }
    size_t inFileSize = ::lseek(in, 0, SEEK_END);
    ::lseek(in, 0, SEEK_SET);

    for (size_t bytesLeft = inFileSize, chunk = inBuffer.size(); bytesLeft > 0; bytesLeft -= chunk) {
        if (bytesLeft < chunk) {
            chunk = bytesLeft;
        }

        ::read(in, &inBuffer[0], chunk);
    }
    ::close(in);
}

void test::ReadWriteTest::testPosixIOR(int fd, std::vector<char> &inBuffer)
{

    size_t inFileSize = ::lseek(fd, 0, SEEK_END);
    ::lseek(fd, 0, SEEK_SET);

    for (size_t bytesLeft = inFileSize, chunk = inBuffer.size(); bytesLeft > 0; bytesLeft -= chunk) {
        if (bytesLeft < chunk) {
            chunk = bytesLeft;
        }

        ::read(fd, &inBuffer[0], chunk);
    }
}

int test::ReadWriteTest::testPosixIOOpenFile(const char *inFile)
{
    int in = ::open(inFile, O_RDONLY, 0666);
    if (in < 0) {
        std::cout << "Can't open output file: " << inFile << std::endl;
        return -1;
    }
    return in;
}

void test::ReadWriteTest::testPosixIOWInt(int fd, int x)
{
    char tmp[10] = { 0x0 };
    sprintf(tmp, "%d", x);
    ::lseek(fd, 0, SEEK_SET);
    ::write(fd, tmp, strlen(tmp));
}
void test::ReadWriteTest::testPosixIOWString(int fd, std::string data)
{
    ::write(fd, data.c_str(), sizeof(data));
}

void test::ReadWriteTest::testPosixIOWriteInt(const char *outFile, int data)
{

    int out = ::open(outFile, O_CREAT | O_RDWR, 0666);
    if (out < 0) {
        std::cout << "Can't open output file: " << outFile << std::endl;
        return;
    }

    testPosixIOWInt( out,  data);
}

void test::ReadWriteTest::testPosixIOWriteString(const char *outFile, std::string data)
{
    int out = ::open(outFile, O_CREAT | O_RDWR, 0666);
    if (out < 0) {
        std::cout << "Can't open output file: " << outFile << std::endl;
        return;
    }

    int err = ::write(out, data.c_str(), sizeof(data));
    std::cout << "err: " << err << std::endl;
    ::close(out);
}

void test::ReadWriteTest::do_io_benchmark_posix_readw(std::string filename, int wdata)
{
    std::vector<char> inBuffer(1024);
    auto time1 = system_clock::now();
    testPosixIOWriteInt(filename.c_str(), wdata);

    auto time2 = system_clock::now();

    testPosixIORead(filename.c_str(), inBuffer);

    auto time3 = system_clock::now();

    std::string s(inBuffer.begin(), inBuffer.end()); //fast for string
    int data = std::stoi(s, nullptr, 0);

    auto time4 = system_clock::now();

    std::cout << "POSIX Write: " << " Wtime: " << duration_cast<microseconds>(time2 - time1).count() << " us"
            << " Read: " << data << " Rtime: " << duration_cast<microseconds>(time3 - time2).count() << " us"
            << " Convert time: " << duration_cast<microseconds>(time4 - time3).count() << " us" << " Total time: "
            << duration_cast<microseconds>(time4 - time1).count() << " us" << std::endl;

}

void test::ReadWriteTest::do_io_benchmark_posix_readw_optimised(std::string filename, int wdata, int times)
{
    std::vector<char> inBuffer(1024);
    int fd = testPosixIOOpenFile(filename.c_str()); //open file

    for (int i = 0; i < times; ++i) {
        auto time1 = system_clock::now();
        testPosixIOWInt(fd, wdata + i);

        auto time2 = system_clock::now();

        testPosixIOR(fd, inBuffer);

        auto time3 = system_clock::now();

        std::string s(inBuffer.begin(), inBuffer.end()); //fast for string
        int data = std::stoi(s, nullptr, 0);

        auto time4 = system_clock::now();

        std::cout << i << " POSIX Write: " << " Wtime: " << duration_cast<microseconds>(time2 - time1).count() << " us"
                << " Read: " << data << " Rtime: " << duration_cast<microseconds>(time3 - time2).count() << " us"
                << " Convert time: " << duration_cast<microseconds>(time4 - time3).count() << " us" << " Total time: "
                << duration_cast<microseconds>(time4 - time1).count() << " us" << std::endl;
    }
    ::close(fd);
}

//https://cristianadam.eu/20160410/c-plus-plus-i-slash-o-benchmark/
void test::ReadWriteTest::do_io_benchmark(std::string type, std::string filename, std::string time)
{
    /*
     std::vector<std::string> args(argv, argv + argc);
     if (args.size() != 4)
     {
     std::cout << "Usage: " << args[0] << " copy_method (c, posix, c++) in_file number_of_times" << std::endl;
     exit(0);
     }*/

    typedef std::map<std::string, std::function<void(const char*, const char*, std::vector<char>&)>> FuncMap;
    FuncMap funcMap { { "c", testCFileIO }, { "posix", testPosixIO }, { "c++", testCppIO } };

    auto it = funcMap.find(type); //auto it = funcMap.find(args[1]);
    if (it != funcMap.end()) {
        std::vector<char> inBuffer(10); //std::vector<char> inBuffer(1024 * 1024);

        auto dest = filename; //+ ".copy"; //auto dest = args[2] + ".copy";

        const auto times = std::stoul(time); //const auto times = std::stoul(args[3]);

        microseconds::rep total = 0;
        for (unsigned int i = 0; i < times; ++i) {
            total += measure::us(it->second, filename.c_str(), dest.c_str(), inBuffer);
            //::unlink(dest.c_str());
        }
        std::cout << filename << " Average " << type << " I/O took: " << total / float(times) << " us" << std::endl;
    } else {
        std::cout << "Not supported copy method: " << type << std::endl;
    }
}

// WARNING :warning: the use of `tmpnam' is dangerous, better use `mkstemp'
/*
void test::ReadWriteTest::do_benchmark_fastest_read_fread_ifstream_or_mmap()
{

    const int N = 10; //const int N = 16384 * 32;
    int tot = 0;
    CPUTimer cput;
    WallClockTimer wct;
    for (int T = 0; T < 1; ++T) { //for (int T = 0; T < 30; ++T) {
        // WARNING :warning: the use of `tmpnam' is dangerous, better use `mkstemp'
    	char *name = tmpnam(NULL); // unsafe but for these purposes, ok

        fillFile(N, name);

        cout << endl;

        tot += testread(name, N);

        // fread
        cput.reset();
        wct.reset();
        auto start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testfread(name, N);
        auto stop = system_clock::now();
        cout << "fread\t\t\t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        // fread with set buffer
        cput.reset();
        wct.reset();
        start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testfreadwithsetbuffer(name, N);
        stop = system_clock::now();
        cout << "fread w sbuffer\t\t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        // fread with large buffer
        cput.reset();
        wct.reset();
        start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testfreadwithlargebuffer(name, N);
        stop = system_clock::now();
        cout << "fread w lbuffer\t\t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        // read
        cput.reset();
        wct.reset();
        start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testread(name, N);
        stop = system_clock::now();
        cout << "read2 \t\t\t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        // mmap
        cput.reset();
        wct.reset();
        start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testwmmap(name, N, false, false);
        stop = system_clock::now();
        cout << "mmap \t\t\t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        // fancy mmap
        cput.reset();
        wct.reset();
        start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testwmmap(name, N, true, false);
        stop = system_clock::now();
        cout << "fancy mmap \t\t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        // mmap
        cput.reset();
        wct.reset();
        start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testwmmap(name, N, false, true);
        stop = system_clock::now();
        cout << "mmap (shared) \t\t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        // fancy mmap
        cput.reset();
        wct.reset();
        start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testwmmap(name, N, true, true);
        stop = system_clock::now();
        cout << "fancy mmap (shared) \t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        // C++
        cput.reset();
        wct.reset();
        start = system_clock::now();
        for (int x = 0; x < 1; ++x)
            tot += testwithCpp(name, N);
        stop = system_clock::now();
        cout << "Cpp\t\t\t" << 512 * N * 1.0 / cput.split() << " " << 512 * N * 1.0 / wct.split() << " "
                << (duration_cast<microseconds>(stop - start).count()) << " us" << endl;

        ::remove(name);
    }
}
*/

