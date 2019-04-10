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

    LegoIRSensor IR_1;
    LegoIRSensor IR_2;
    LegoIRSensor IR_3;
//    LegoIRSensor IR_4;


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

};

#endif
