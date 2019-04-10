/*
 * LegoAngleSensor.cpp
 *
 *  Created on: 29 mars 2019
 *      Author: pmx
 */

#include "LegoAngleSensor.hpp"

#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>

constexpr char LegoAngleSensor::ht_angle[];

LegoAngleSensor::LegoAngleSensor(address_type address) :
        sensor(address, { ht_angle })
{
    if (this->connected()) {
        this->set_mode("ANGLE-ACC");
    }
    this->openOptimizedFile("value0");
}

void LegoAngleSensor::reset()
{
    if (this->connected())
        this->set_command("RESET");
}

long LegoAngleSensor::getValueDegrees()
{
    if (this->connected()) {

        //methode 1
        //long ticks = this->value(0);
        //methode 2
        //long ticks = this->get_attr_int("value0");

        //methode3
        long ticks = this->get_attr_int_optimized();

        return ticks;
    } else
        return -999999;
}

void LegoAngleSensor::openOptimizedFile(const std::string &name) const
{
    using namespace std;

//    if (_path.empty())
//        throw system_error(make_error_code(errc::function_not_supported),
//                "no device connected ; [get_attr_int] path=" + _path + " " + name);

    if (_path.empty()) {
        return;
    }

    _is = new ifstream();
    _is->open(_path + name);

}
int LegoAngleSensor::get_attr_int_optimized() const
{
    // _is = new ifstream();
    //   _is->open(_path + "value0");
//     if (!_is->is_open()) {
//
//            throw system_error(make_error_code(errc::no_such_device),
//                    "no device connected ; [open] path=" + _path + name);
//        }

    //_is->clear();
    _is->seekg(0, std::ios::beg);
    int result = 0;
    *_is >> result;
    // _is->close();

    return result;
}

uintmax_t LegoAngleSensor::wc(char const *fname)
{
    static const auto BUFFER_SIZE = 16 * 1024;
    int fd = open(fname, O_RDONLY);
    if (fd == -1) {
        printf("ERROR LegoAngleSensor::wc open");
        //handle_error("open");
        return -999;
    }

    /* Advise the kernel of our access pattern.  */
    posix_fadvise(fd, 0, 0, 1);  // FDADVICE_SEQUENTIAL

    char buf[BUFFER_SIZE + 1];
    uintmax_t lines = 0;

    while (size_t bytes_read = read(fd, buf, BUFFER_SIZE)) {
        if (bytes_read == (size_t) -1) {
            printf("ERROR LegoAngleSensor::wc read failed");
            //handle_error("read failed");
            return -999;
        }

        if (!bytes_read)
            break;

        for (char *p = buf; (p = (char*) memchr(p, '\n', (buf + bytes_read) - p)); ++p)
            ++lines;
    }

    return lines;
}
