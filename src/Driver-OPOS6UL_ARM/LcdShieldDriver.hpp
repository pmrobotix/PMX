#ifndef OPOS6UL_LCDSHIELDDRIVER_HPP_
#define OPOS6UL_LCDSHIELDDRIVER_HPP_

#include <stddef.h>
#include <cstdint>

#include "../Common/Action.Driver/ALcdShieldDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class LcdShieldDriver: public ALcdShieldDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref LcdShieldDriver(OPOS6UL).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LcdShieldDriver.OPO");
        return instance;
    }

public:

    /*!
     * \brief Constructor.
     */
    LcdShieldDriver();

    /*!
     * \brief Destructor.
     */
    ~LcdShieldDriver();

    void clear();

    void home();

    void setBacklightOn();

    void setBacklightOff();

    void setCursor(uint8_t, uint8_t);

    size_t write(uint8_t value);

};

#endif
