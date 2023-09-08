#ifndef OPOS6UL_SENSORSDRIVER_HPP_
#define OPOS6UL_SENSORSDRIVER_HPP_

#include "../Common/Interface.Driver/ASensorsDriver.hpp"
#include "../Common/Utils/PointerList.hpp"
#include "../Log/LoggerFactory.hpp"
#include "BeaconSensors.hpp"
#include "Gp2y0e02b.hpp"

using namespace std;
#define ADDRESS_BeaconSensors   0x2D


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

    BeaconSensors beaconSensors_;
    Registers regs_;
    Settings settings_;

    bool beacon_connected_;

//    Gp2y0e02b gp2_1_;
//    Gp2y0e02b gp2_2_;

    bool connected_gp2y0e02b_;

    utils::Mutex msync_;
    bot_positions vadv_;  //tableau des pos des adv


//    int getFrontDistMmFromObject(int diagonal_dist_mm);
//    int getBackDistMmFromObject(int diagonal_dist_mm);


public:

    /*!
     * \brief Constructor.
     */
    SensorsDriver(ARobotPositionShared * robotpos);

    /*!
     * \brief Destructor.
     */
    ~SensorsDriver();

    bool is_connected();

    void displayNumber(int number);
    int getAnalogPinData();



    int sync(); //synchronise les données avec la balise, return 0 if success, -1 if error.
    ASensorsDriver::bot_positions getvPositionsAdv(); //retourne les dernieres positions connues

    int frontLeft(); //retourne la dernière distance minimum gauche (en mm) apres le sync
    int frontCenter();
    int frontRight(); //retourne la dernière distance minimum droite (en mm) en mm apres le sync

    int backLeft();
    int backCenter();
    int backRight();

    int rightSide();
    int leftSide();

};

#endif
