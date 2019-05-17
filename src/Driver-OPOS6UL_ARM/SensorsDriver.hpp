#ifndef OPOS6UL_SENSORSDRIVER_HPP_
#define OPOS6UL_SENSORSDRIVER_HPP_

#include <as_devices/cpp/as_i2c.hpp>

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "Gp2y0e02b.hpp"
#include "IrSensor.hpp"

using namespace std;

#define ADDRESS_gp2y0e02b       0x40 //0x80 >> 1  // Arduino uses 7 bit addressing so we shift address right one bit
#define DISTANCE_REG_gp2y0e02b  0x5E
#define SHIFT_gp2y0e02b         0x35

class SensorsDriver: public ASensorsDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(OPOS6UL).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsDriver.OPO");
        return instance;
    }

    Gp2y0e02b gp2_1_;
    Gp2y0e02b gp2_2_;

    bool connected_gp2y0e02b_;
    long shift_;

    IrSensor irLeft_;

    IrSensor irCenter_;

    IrSensor irRight_;

    IrSensor irRear_;

public:

    /*!
     * \brief Constructor.
     */
    SensorsDriver();

    /*!
     * \brief Destructor.
     */
    ~SensorsDriver();

    int left();
    int right();
    bool front();
    bool rear();
    bool frontVeryClosed();
    bool rearVeryClosed();

};

#endif
