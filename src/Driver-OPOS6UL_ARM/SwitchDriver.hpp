#ifndef OPOS6UL_SWITCHDRIVER_HPP_
#define OPOS6UL_SWITCHDRIVER_HPP_

#include <string>

#include "../Common/Interface.Driver/ASwitchDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class SwitchDriver: public ASwitchDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SwitchDriver(OPOS6UL).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("SwitchDriver.OPO");
        return instance;
    }

public:

    /*!
     * \brief Constructor.
     */
    SwitchDriver();

    /*!
     * \brief Destructor.
     */
    ~SwitchDriver();

    bool is_connected();

    int pressed(unsigned char pin);
    int tirettePressed();
    int backLeftPressed();
    int backRightPressed();
    void setGPIO(int gpio, bool activate);

};

#endif
