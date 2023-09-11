#ifndef DRIVER_EV3_EV3DEV_EXTEND_HPP_
#define DRIVER_EV3_EV3DEV_EXTEND_HPP_

#include <string>

#include "../Thread/Mutex.hpp"

class device_extend: public utils::Mutex
{
private:

public:
    int open_posix_in(std::string pathfile);
    int open_posix_out(std::string pathfile);
    void close_posix(int fd);

    void set_attr_int_posix(int fd, int value);
    int get_attr_int_posix(int fd);

    void set_attr_string_posix(int fd, std::string name);
    std::string get_attr_string_posix(int fd);

    int open_get_attr_int_close_posix(std::string name);
    std::string open_get_attr_string_close_posix(std::string pathfile);
};

#endif /* DRIVER_EV3_EV3DEV_EXTEND_HPP_ */
