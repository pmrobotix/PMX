#ifndef ARM_SENSORSDRIVER_HPP_
#define ARM_SENSORSDRIVER_HPP_

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "IrSensor.hpp"

using namespace std;

class SensorsDriver: public ASensorsDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(ARM).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsDriver.ARM");
        return instance;
    }

    IrSensor frontIR_; //centre
    IrSensor frontIRRight_;
    IrSensor frontIRLeft_;

    IrSensor rearIR_;

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

};

#endif
