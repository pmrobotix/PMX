#ifndef ALCDSHIELDDRIVER_HPP_
#define ALCDSHIELDDRIVER_HPP_

#include <stddef.h>
#include <cstdint>
#include <string>

class ALcdShieldDriver
{

public:
    /*!
     * \brief LedDriver instance creation.
     * \param botName
     * 			Name of the robot (for simulator).
     *
     */
    static ALcdShieldDriver * create(std::string botName);

    /*!
     * \brief Destructor.
     */
    virtual ~ALcdShieldDriver()
    {
    }

    virtual void clear() = 0;

    virtual void home() = 0;

    virtual void setBacklightOn() = 0;

    virtual void setBacklightOff() = 0;

    virtual void setCursor(uint8_t, uint8_t) = 0;

    virtual size_t write(uint8_t value) = 0; //for Print

    virtual void print_content_string(std::string str, int row, int col=1) = 0;

    virtual void print_content_integer(int value, int row, int col=1) = 0;


protected:

    /*!
     * \brief Constructor.
     */
    ALcdShieldDriver()
    {
    }

};

#endif
