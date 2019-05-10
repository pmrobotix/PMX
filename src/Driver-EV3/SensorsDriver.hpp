#ifndef EV3_SENSORSDRIVER_HPP_
#define EV3_SENSORSDRIVER_HPP_

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "LegoIRSensor.hpp"

using namespace std;
using namespace ev3dev;

class SensorsDriver: public ASensorsDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsDriver.EV3");
        return instance;
    }

    LegoIRSensor IR_1_AV_D;
    LegoIRSensor IR_2_AV_G;
    LegoIRSensor IR_3_AR_D;
    LegoIRSensor IR_4_AR_G;


public:

    /*!
     * \brief Constructor.
     */
    SensorsDriver();

    /*!
     * \brief Destructor.
     */
    ~SensorsDriver();

    bool front();
    bool rear();

    bool frontVeryClosed();
    bool rearVeryClosed();

};

#endif
