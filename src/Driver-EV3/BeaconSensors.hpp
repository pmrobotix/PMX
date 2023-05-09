#ifndef DRIVER_EV3_BEACONSENSORS_HPP_
#define DRIVER_EV3_BEACONSENSORS_HPP_

#include "Ev3i2c.hpp"

#include "../Log/LoggerFactory.hpp"

#define DATA_BeaconSensors      0x04 //addresse des données à recupérer
#define NUMOFBOTS_BeaconSensors 0x00

//WEBSITE REFERENCE : convert float to byte array  source: http://mbed.org/forum/helloworld/topic/2053/

union float2bytes_t // union consists of one variable represented in a number of different ways
{
    float f;
    unsigned char bytes[sizeof(float)];
};

// Registers that the caller can both read and write
struct Settings {
    int8_t numOfBots;         // Register 0. Number of Robot which may to be detected, default 3.
    int8_t ledDisplay;        // Register 1. Writable. Sets the mode for led display. 0 => OFF. 100 => FULL ON. 50 => Half luminosity.
    int8_t tempNumber;        // Register 2.//TODO shift_rad angle en parametre ? //TODO ajouter le decalage d'angle en settings ?
    int8_t reserved = 0;      //NOT use yet
};

// Registers that the caller can only read
struct Registers {
    uint8_t flags = 0;        // Register 4. bit 0 => new data available
    //TODO FLAGS
    //overflow more than 3 beacons
    //more than 6 contigus
    //default config settings is changed by master or not. in case of reset, the master can know and reconfigure

    uint8_t nbDetectedBots = 0; //Register 5.Nombre de balises détectées.
    int16_t c1_mm = 0;        // Register 6. //AV GAUCHE BAS
    int16_t c2_mm = 0;        // Register 8. //AV GAUCHE HAUT
    int16_t c3_mm = 0;        // Register 10. //AV DROIT BAS
    int16_t c4_mm = 0;        // Register 12. //AV DROIT HAUT
    int16_t c5_mm = 0;        // Register 14. //AR GAUCHE BAS
    int16_t c6_mm = 0;        // Register 16. //AR GAUCHE HAUT
    int16_t c7_mm = 0;        // Register 18. //AR DROIT BAS
    int16_t c8_mm = 0;        // Register 20. //AR DROIT HAUT

    int16_t reserved_analog = 0;        // Register 22.

    int16_t x1_mm = 0;        // Register 24.
    int16_t y1_mm = 0;        // Register 26.
    float a1_deg = 0.0;       // Register 28.

    int16_t x2_mm = 0;        // Register 32.
    int16_t y2_mm = 0;        // Register 34.
    float a2_deg = 0.0;       // Register 36.

    int16_t x3_mm = 0;        // Register 40.
    int16_t y3_mm = 0;        // Register 42.
    float a3_deg = 0.0;       // Register 44.

    int16_t x4_mm = 0;        // Register 48
    int16_t y4_mm = 0;        // Register 50
    float a4_deg = 0.0;       // Register 52

    int16_t d1_mm = 0;        // Register 56.    centre à centre
    int16_t d2_mm = 0;        // Register 58.
    int16_t d3_mm = 0;        // Register 60.
    int16_t d4_mm = 0;        // Register 62.

//TODO decaler de 2
    int8_t z1_p = 0;          // Register 62. position de la zone z1_1 (entre 0 et 71).
    int8_t z1_n = 0;          // Register 63. nombre de zones detectées pour la balise z1 (entre 1 et 7) afin d'economiser les données.
    int16_t z1_1 = 0;         // Register 64.
    int16_t z1_2 = 0;         // Register 66.
    int16_t z1_3 = 0;         // Register 68.
    int16_t z1_4 = 0;         // Register 70.
    int16_t z1_5 = 0;         // Register 72.
    int16_t z1_6 = 0;         // Register 74.
    int16_t z1_7 = 0;         // Register 76.

    int8_t z2_p = 0;          // Register 78.position de la zone z2_1 (entre 0 et 71).
    int8_t z2_n = 0;          // Register 79.nombre de zones detectées pour la balise z2 (entre 1 et 7) afin d'economiser les données.
    int16_t z2_1 = 0;         // Register 80.
    int16_t z2_2 = 0;         // Register 82.
    int16_t z2_3 = 0;         // Register 84.
    int16_t z2_4 = 0;         // Register 86.
    int16_t z2_5 = 0;         // Register 88.
    int16_t z2_6 = 0;         // Register 90.
    int16_t z2_7 = 0;         // Register 92.

    int8_t z3_p = 0;          // Register 94.position de la zone z3_1 (entre 0 et 71).
    int8_t z3_n = 0;          // Register 95.nombre de zones detectées pour la balise z3 (entre 1 et 7) afin d'economiser les données.
    int16_t z3_1 = 0;         // Register 96.
    int16_t z3_2 = 0;         // Register 98.
    int16_t z3_3 = 0;         // Register 100.
    int16_t z3_4 = 0;         // Register 102.
    int16_t z3_5 = 0;         // Register 104.
    int16_t z3_6 = 0;         // Register 106.
    int16_t z3_7 = 0;         // Register 108.

    int8_t z4_p = 0;          // Register 110.position de la zone z4_1 (entre 0 et 71).
    int8_t z4_n = 0;          // Register 111.nombre de zones detectées pour la balise z4 (entre 1 et 7) afin d'economiser les données.
    int16_t z4_1 = 0;         // Register 112.
    int16_t z4_2 = 0;         // Register 114.
    int16_t z4_3 = 0;         // Register 116.
    int16_t z4_4 = 0;         // Register 118.
    int16_t z4_5 = 0;         // Register 120.
    int16_t z4_6 = 0;         // Register 122.
    int16_t z4_7 = 0;         // Register 124.

};

class BeaconSensors {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref BeaconSensors(EV3).
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("BeaconSensors.EV3");
        return instance;
    }

    Ev3i2c i2c_BeaconSensors_;

    bool connected_BeaconSensors_;
    uint i2c_aAddr_;

    int readRegnBytes(uint8_t command, uint8_t *aData, uint nb);
    int readAddr(uint8_t address, uint8_t * data);

    //unsigned char readRegByte(unsigned char command);
    /*
     int readReg2Bytes(unsigned char command, unsigned char *aData);
     int readReg3Bytes(unsigned char command, unsigned char *aData);

     int writeRegByte(unsigned char command, unsigned char aData1);
     int writeReg2Bytes(unsigned char command, unsigned char aData1, unsigned char aData2);
     */


public:

    /*!
     * \brief Constructor BeaconSensors.
     */
    BeaconSensors(int bus, unsigned char address);

    /*!
     * \brief Destructor.
     */
    ~BeaconSensors() {
    }

    /*!
     * \brief Init 12C.
     */
    bool connect(); //TODO RENAME BEGIN

    /*!
     *  \brief get distance in mm.
     *  //every 250 ms
     */
    Registers getData();

    void display(int number);

    int ScanBus();
};

#endif
