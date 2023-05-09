#ifndef EV3_SENSORSDRIVER_HPP_
#define EV3_SENSORSDRIVER_HPP_

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"
#include "BeaconSensors.hpp"

#define ADDRESS_BeaconSensors   0x2D

using namespace std;
//using namespace ev3dev;

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

    int connected_;
    BeaconSensors beaconSensors_;
    Registers regs_;

//    LegoIRSensor IR_1_AV_D;
//    LegoIRSensor IR_2_AV_G;
//    LegoIRSensor IR_3_AR_D;
//    LegoIRSensor IR_4_AR_G;

    utils::Mutex msync_;
    bot_positions vadv_;  //tableau des pos des adv

public:

    /*!
     * \brief Constructor.
     */
    SensorsDriver();

    /*!
     * \brief Destructor.
     */
    ~SensorsDriver();

    bool is_connected();

    int sync(); //synchronise les données avec la balise
    ASensorsDriver::bot_positions getvPositionsAdv(); //retourne les dernieres positions connues

    int rightSide();
    int leftSide();

    int frontLeft();
    int frontCenter();
    int frontRight();

    int backLeft();
    int backCenter();
    int backRight();

    void displayNumber(int number);

    int getAnalogPinData();

};

#endif
