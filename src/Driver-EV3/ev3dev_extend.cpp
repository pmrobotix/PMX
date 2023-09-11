#include "ev3dev_extend.hpp"

#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

int device_extend::open_get_attr_int_close_posix(std::string pathfile)
{

    int in = open_posix_in(pathfile);
    int data = get_attr_int_posix(in);
    close_posix(in);

    return data;
}

std::string device_extend::open_get_attr_string_close_posix(std::string pathfile)
{

    int in = open_posix_in(pathfile);
    try {
        std::string data = get_attr_string_posix(in);
        close_posix(in);

        return data;
    } catch (...) {
        close_posix(in);
        throw;
    }
}

void device_extend::set_attr_int_posix(int fd, int value)
{
    lock();
    try {
        char tmp[10] = { 0x0 }; //maximum digits
        sprintf(tmp, "%d", value);
        ::lseek(fd, 0, SEEK_SET);
        ::write(fd, tmp, strlen(tmp));
        unlock();
        if (errno == ENODEV) {
            printf("\n[set_attr_int_posix] ENODEV !!!\n");
        }
    } catch (...) {
        unlock();
        printf("\n[set_attr_int_posix] catch throw !!!\n");
        throw;
    }
}

void device_extend::set_attr_string_posix(int fd, std::string name)
{
    lock();
    try {
        const char *tmp = name.c_str();
        ::lseek(fd, 0, SEEK_SET);
        ::write(fd, tmp, strlen(tmp));
        unlock();
        if (errno == ENODEV) {
            printf("\n[set_attr_string_posix] ENODEV !!!\n");
        }
    } catch (...) {
        unlock();
        throw;
    }
}

std::string device_extend::get_attr_string_posix(int fd)
{
    lock();
    try {
        std::vector<char> inBuffer(1024);
        size_t inFileSize = ::lseek(fd, 0, SEEK_END);
        ::lseek(fd, 0, SEEK_SET);

        for (size_t bytesLeft = inFileSize, chunk = inBuffer.size(); bytesLeft > 0; bytesLeft -= chunk) {
            if (bytesLeft < chunk) {
                chunk = bytesLeft;
            }
            ::read(fd, &inBuffer[0], chunk);
        }
        std::string s(inBuffer.begin(), inBuffer.end());
        unlock();
        return s;
    } catch (...) {
        unlock();
        throw;
    }
}

int device_extend::get_attr_int_posix(int fd)
{
    lock();
    std::string s = get_attr_string_posix(fd);
    int data = std::stoi(s);//, nullptr, 0); //convert to int
    unlock();
    return data;
}

int device_extend::open_posix_in(std::string pathfile)
{
    lock();
    const char *inFile = pathfile.c_str();
    int in = ::open(inFile, O_RDONLY, 0666);
    if (in < 0) {
        std::cout << "open_posix_in :: Can't open output file: " << inFile << std::endl;
        unlock();
        return -1;
    }
    unlock();
    return in;
}

int device_extend::open_posix_out(std::string pathfile)
{
    lock();
    const char *outFile = pathfile.c_str();
    int out = ::open(outFile, O_WRONLY, 0666);
    if (out < 0) {
        std::cout << "open_posix_out :: Can't open output file: " << outFile << std::endl;
        unlock();
        return -1;
    }
    unlock();
    return out;
}

void device_extend::close_posix(int fd)
{
    lock();
    ::close(fd);
    unlock();
}

