/*
 Reference:
 http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y0e02_03_appl_e.pdf

 https://github.com/martinpalsson/GP2Y0E02B_E-Fuse_Burner

 other examples found on the web:
 https://github.com/NorbotNorway/TeensyAndGP2Y0E02B/blob/master/Sample.ino

 https://github.com/GeeAlex/PiSharpIR/blob/master/GP2Y.py

 https://github.com/aahlborg/SharpRangerReader/blob/master/SharpRangerReader.ino

 https://www.robot-electronics.co.uk/htm/arduino_examples.htm#GP2Y0E02B
 */

#ifndef DRIVER_OPOS6UL_ARM_GP2Y0E02B_HPP_
#define DRIVER_OPOS6UL_ARM_GP2Y0E02B_HPP_

#include <as_devices/cpp/as_i2c.hpp>

#include "../Log/LoggerFactory.hpp"

//#define ADDRESS_gp2y0e02b       0x40 //0x80 >> 1  // Arduino uses 7 bit addressing so we shift address right one bit
#define SHIFT_ADDR      0x35
#define DISTANCE_ADDR   0x5E
#define RIGHT_EDGE_ADDR 0xF8 // C
#define LEFT_EDGE_ADDR  0xF9 // A
#define PEAK_EDGE_ADDR  0xFA // B

class Gp2y0e02b
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref Gp2y0e02b(OPOS6UL).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("Gp2y0e02b.OPO");
        return instance;
    }

    AsI2c i2c_gp2y0e02b_;
    unsigned char address_;
    bool connected_gp2y0e02b_;
    long shift_;

    unsigned char readAddr(unsigned char address);
    unsigned char readRegByte(unsigned char command);
    int readReg2Bytes(unsigned char command, unsigned char *aData);
    int readReg3Bytes(unsigned char command, unsigned char *aData);

    int writeRegByte(unsigned char command, unsigned char aData1);
    int writeReg2Bytes(unsigned char command, unsigned char aData1, unsigned char aData2);

public:

    /*!
     * \brief Constructor.
     */
    Gp2y0e02b(int bus, unsigned char address);

    /*!
     * \brief Destructor.
     */
    ~Gp2y0e02b()
    {
    }

    /*!
     *  \brief get distance in mm.
     *  //every 250 ms
     */
    int getDistanceMm();
    void getInfoData(unsigned int *spotSize, unsigned int *spotSym);
    void EFuseSlaveID(unsigned char desiredAddress, bool pin_activated);
    int ScanBus();
};

#endif /* DRIVER_OPOS6UL_ARM_GP2Y0E02B_HPP_ */
