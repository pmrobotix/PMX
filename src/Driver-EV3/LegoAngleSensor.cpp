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
#include <cstring>
#include <fstream>
#include <string>

#include "../Log/Logger.hpp"

constexpr char LegoAngleSensor::ht_angle[];

LegoAngleSensor::LegoAngleSensor(address_type address) :
        i2c_sensor(address, { ht_angle })
{
    address_ = address;
    if (this->connected()) {
        this->set_mode("ANGLE-ACC");

        //Mise à jour du polling pour mettre a jour en i2c toutes les 20ms
        this->set_poll_ms(20);
    }
    usleep(10000);
    _ifs_value = NULL;
    this->openIfstreamFile("value0");
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
        //logger().info() << address_ << " " << ticks << logs::end;
        return ticks;
    } else
        return -999999;
}

void LegoAngleSensor::openIfstreamFile(const std::string &name) const
{
    if (_path.empty()) {
        logger().error() << "ERROR - LegoAngleSensor::openIfstreamFile() path empty !!!!\n" << logs::end;
        //printf("ERROR - LegoAngleSensor::openIfstreamFile() path empty !!!!\n");
        return;
    }


    _ifs_value = new std::ifstream();
    _ifs_value->open(_path + name);
    _ifs_value->clear();

}
int LegoAngleSensor::get_attr_int_optimized() const
{
    for (int attempt = 0; attempt < 2; ++attempt) {
        try {
            _ifs_value->seekg(0, std::ios::beg);
            int result = -999999;
            *_ifs_value >> result;

            if (result == -999999)
            {
                logger().error() << "ERROR - get_attr_int_optimized - BAD RESULT!! " << address_ << logs::end;
                _ifs_value->seekg(0, std::ios::beg);
                *_ifs_value >> result;

                if (result == -999999)
                            {
                    logger().error() << "!!!!!!!!!!!!!!!!!!!!!!!!ERROR - get_attr_int_optimized - BAD RESULT!! " << address_ << logs::end;
                    exit(0);
                            }
            }
            return result;
        } catch (...) {
            // This could mean the sysfs attribute was recreated and the
            // corresponding file handle got stale. Lets close the file and try
            // again (once):
            if (attempt != 0) {
                logger().error() << "ERROR - get_attr_int_optimized !!!! attempt=" << attempt << logs::end;
                throw;
            }
            _ifs_value->clear();
        }
    }

}

uintmax_t LegoAngleSensor::wc(char const *fname)
{
    static const auto BUFFER_SIZE = 16 * 1024;
    int fd = open(fname, O_RDONLY);
    if (fd == -1) {
        logger().error() << "ERROR LegoAngleSensor::wc open" << logs::end;
        //printf("ERROR LegoAngleSensor::wc open");
        //handle_error("open");
        return -999;
    }

    /* Advise the kernel of our access pattern.  */
    posix_fadvise(fd, 0, 0, 1);  // FDADVICE_SEQUENTIAL

    char buf[BUFFER_SIZE + 1];
    uintmax_t lines = 0;

    while (size_t bytes_read = read(fd, buf, BUFFER_SIZE)) {
        if (bytes_read == (size_t) -1) {
            logger().error() << "ERROR LegoAngleSensor::wc read failed" << logs::end;
            //printf("ERROR LegoAngleSensor::wc read failed");
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
