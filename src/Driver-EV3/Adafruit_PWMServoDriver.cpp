/*!
 *  @file Adafruit_PWMServoDriver.cpp
 *
 *  @mainpage Adafruit 16-channel PWM & Servo driver
 *
 *  @section intro_sec Introduction
 *
 *  This is a library for the 16-channel PWM & Servo driver.
 *
 *  Designed specifically to work with the Adafruit PWM & Servo driver.
 *
 *  Pick one up today in the adafruit shop!
 *  ------> https://www.adafruit.com/product/815
 *
 *  These displays use I2C to communicate, 2 pins are required to interface.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  @section author Author
 *
 *  Limor Fried/Ladyada (Adafruit Industries).
 *
 *  @section license License
 *
 *  BSD license, all text above must be included in any redistribution
 *
 *  PMXlib basé sur la VERSION 2.4.0 avec inclusion des pull requests suivantes:
 *  https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library/pull/81/commits/0e169b0f73382ce4cda1fa32d1213b627bc6bcb6
 *  https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library/pull/84/commits/f22e66bbbe172742074882356f61b9fe307e1bbf
 *  https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library/pull/87/commits/4b71091c32735c4cc930900b8a4e12d3cec59b71
 *
 */

#include "Adafruit_PWMServoDriver.hpp"

#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

//#define ENABLE_DEBUG_OUTPUT

/*!
 *  @brief  Instantiates a new PCA9685 PWM driver chip with the I2C address on a
 * TwoWire interface
 */
//Adafruit_PWMServoDriver::Adafruit_PWMServoDriver() :
//        _i2caddr(PCA9685_I2C_ADDRESS), i2c_(1, false), _connected_(0)
//{
//    _oscillator_freq = FREQUENCY_OSCILLATOR;
//
//}
/*!
 *  @brief  Instantiates a new PCA9685 PWM driver chip with the I2C address on a
 * TwoWire interface
 *  @param  addr The 7-bit I2C address to locate this chip, default is 0x40
 */
Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(uint i2c_bus_num, const uint8_t addr) :
        _i2caddr(addr), i2c_(i2c_bus_num, false), connected_(false)
{
    _oscillator_freq = FREQUENCY_OSCILLATOR;

}

/*!
 *  @brief  Instantiates a new PCA9685 PWM driver chip with the I2C address on a
 * TwoWire interface
 *  @param  addr The 7-bit I2C address to locate this chip, default is 0x40
 *  @param  i2c  A reference to a 'TwoWire' object that we'll use to communicate
 *  with
 */
//Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(const uint8_t addr,
//                                                 TwoWire &i2c)
//    : _i2caddr(addr), _i2c(&i2c) {}
/*!
 *  @brief  Setups the I2C interface and hardware
 *  @param  frequence ( limit is 3052=50MHz/(4*4096))
 *  @param  prescale
 *          Sets External Clock (Optional)
 */
bool Adafruit_PWMServoDriver::connect(float freq, uint8_t prescale) {
    if (connected_) return connected_;

    int err = i2c_.begin(_i2caddr);
    if (err >= 0) connected_ = true;
    else {
        connected_ = false;
        //logger().error() << "Adafruit_PWMServoDriver::connect() : NOT CONNECTED!" << logs::end;
    }

    reset();

    // set the default internal frequency
    setOscillatorFrequency(FREQUENCY_OSCILLATOR);

    if (prescale) {
        setExtClk(prescale);
    }
    else {
        setPWMFreq(freq); //Datasheet limit is 3052=50MHz/(4*4096)
    }

    return connected_;
}

/*!
 *  @brief  Sends a reset command to the PCA9685 chip over I2C
 */
void Adafruit_PWMServoDriver::reset() {
    write8(PCA9685_MODE1, MODE1_RESTART);
    usleep(10000);
}

/*!
 *  @brief  Puts board into sleep mode
 */
void Adafruit_PWMServoDriver::sleep() {
    uint8_t awake = read8(PCA9685_MODE1);
    uint8_t sleep = awake | MODE1_SLEEP; // set sleep bit high
    write8(PCA9685_MODE1, sleep);
    usleep(5000); // wait until cycle ends for sleep to be active
}

/*!
 *  @brief  Wakes board from sleep
 */
void Adafruit_PWMServoDriver::wakeup() {
    uint8_t sleep = read8(PCA9685_MODE1);
    uint8_t wakeup = sleep & ~MODE1_SLEEP; // set sleep bit low
    write8(PCA9685_MODE1, wakeup);
}

/*!
 *  @brief  Sets EXTCLK pin to use the external clock
 *  @param  prescale
 *          Configures the prescale value to be used by the external clock
 */
void Adafruit_PWMServoDriver::setExtClk(uint8_t prescale) {
    uint8_t oldmode = read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
    write8(PCA9685_MODE1, newmode); // go to sleep, turn off internal oscillator

    // This sets both the SLEEP and EXTCLK bits of the MODE1 register to switch to
    // use the external clock.
    write8(PCA9685_MODE1, (newmode |= MODE1_EXTCLK));

    write8(PCA9685_PRESCALE, prescale); // set the prescaler

    usleep(5000);
    // clear the SLEEP bit to start
    write8(PCA9685_MODE1, (newmode & ~MODE1_SLEEP) | MODE1_RESTART | MODE1_AI);

#ifdef ENABLE_DEBUG_OUTPUT
//    Serial.print("Mode now 0x");
//    Serial.println(read8(PCA9685_MODE1), HEX);
#endif
}

/*!
 *  @brief  Sets the PWM frequency for the entire chip, up to ~1.6 KHz
 *  @param  freq Floating point frequency that we will attempt to match
 */
void Adafruit_PWMServoDriver::setPWMFreq(float freq) {

#ifdef ENABLE_DEBUG_OUTPUT
//    Serial.print("Attempting to set freq ");
//    Serial.println(freq);
    printf("\nAttempting to set freq %f", freq);
#endif
    // Range output modulation frequency is dependant on oscillator
    if (freq < 1) freq = 1;
    if (freq > 3500) freq = 3500; // Datasheet limit is 3052=50MHz/(4*4096)

    float prescaleval = ((_oscillator_freq / (freq * 4096.0)) + 0.5) - 1;
    if (prescaleval < PCA9685_PRESCALE_MIN) prescaleval = PCA9685_PRESCALE_MIN;
    if (prescaleval > PCA9685_PRESCALE_MAX) prescaleval = PCA9685_PRESCALE_MAX;
    uint8_t prescale = (uint8_t) prescaleval;

#ifdef ENABLE_DEBUG_OUTPUT
//    Serial.print("Final pre-scale: ");
//    Serial.println(prescale);
    printf("\nFinal pre-scale: %d", prescale);
#endif

    uint8_t oldmode = read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
    write8(PCA9685_MODE1, newmode);                             // go to sleep
    write8(PCA9685_PRESCALE, prescale); // set the prescaler
    write8(PCA9685_MODE1, oldmode);
    usleep(5000);
    // This sets the MODE1 register to turn on auto increment.
    write8(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);

#ifdef ENABLE_DEBUG_OUTPUT
    //usleep(5000);
    //printf("\nverifprescale");
    //uint8_t verifprescale = readPrescale();
    //printf("\nverifprescale: %d", verifprescale);
//    Serial.print("Mode now 0x");
//    Serial.println(, HEX);
    printf("\nMode now 0x%02x", read8(PCA9685_MODE1));
#endif
}

/*!
 *  @brief  Sets the output mode of the PCA9685 to either
 *  open drain or push pull / totempole.
 *  Warning: LEDs with integrated zener diodes should
 *  only be driven in open drain mode.
 *  @param  totempole Totempole if true, open drain if false.
 */
void Adafruit_PWMServoDriver::setOutputMode(bool totempole) {
    uint8_t oldmode = read8(PCA9685_MODE2);
    uint8_t newmode;
    if (totempole) {
        newmode = oldmode | MODE2_OUTDRV;
    }
    else {
        newmode = oldmode & ~MODE2_OUTDRV;
    }
    write8(PCA9685_MODE2, newmode);
#ifdef ENABLE_DEBUG_OUTPUT
//    Serial.print("Setting output mode: ");
//    Serial.print(totempole ? "totempole" : "open drain");
//    Serial.print(" by setting MODE2 to ");
//    Serial.println(newmode);
#endif
}

/*!
 *  @brief  Reads set Prescale from PCA9685
 *  @return prescale value
 */
uint8_t Adafruit_PWMServoDriver::readPrescale(void) {
    return read8(PCA9685_PRESCALE);
}

/*!
 *  @brief  Gets the PWM output of one of the PCA9685 pins
 *  @param  num One of the PWM output pins, from 0 to 15
 *  @return requested PWM output value
 */
//uint8_t Adafruit_PWMServoDriver::getPWM(uint8_t num) {
//
//
//  _i2c->requestFrom((int)_i2caddr, PCA9685_LED0_ON_L + 4 * num, (int)4);
//  return _i2c->read();
//}
/*!
 *  @brief read pulsewidth (bits 0->4096) from spec'd channel
 *  @param num is the channel number
 *  @param on is a switch to say whether we want the "on" PWM bits or the "off" bits
 *
 * reference:
 * https://thecavepearlproject.org/2017/11/03/configuring-i2c-sensors-with-arduino/
 */
//uint8_t Adafruit_PWMServoDriver::getPWM(uint8_t num) {
//  _i2c->requestFrom((int)_i2caddr, PCA9685_LED0_ON_L + 4 * num, (int)4);
//  return _i2c->read();
//}

uint16_t Adafruit_PWMServoDriver::getPWM(uint8_t num, bool on) {
//    printf("Adafruit_PWMServoDriver::getPWM NOT IMPLEMENTED\n");
//    return -1;

//  uint16_t _value;
//  // on == true means we want to read ON PWM value; true = 1,
//  // so invert to point to PCA9685_LED0_OFF_L
//  uint8_t _register_addr = PCA9685_LED0_ON_L + 2 * (int)!on + 4 * num;
//  _i2c->beginTransmission(_i2caddr);      // set sensor target
//  _i2c->write(_register_addr);            // set memory pointer
//  _i2c->endTransmission();
//  _i2c->requestFrom((int)_i2caddr, (int) 2); // request two bytes
//  uint8_t _registerDataLo = _i2c->read(); // get low byte
//  uint8_t _registerDataHi = _i2c->read(); // get high byte
//  _value = (((int)_registerDataHi) << 8) | _registerDataLo; // combine two bytes
//  return (_value);

    uint16_t _value;
    double _value_d;
    //uint8_t _value;
    // on == true means we want to read ON PWM value; true = 1,
    // so invert to point to PCA9685_LED0_OFF_L
    uint8_t _register_addr = PCA9685_LED0_ON_L + (2 * (int) !on) + (4 * num);
    //uint8_t _register_addr = PCA9685_LED0_OFF_L + 4 * num;

//      _i2c->beginTransmission(_i2caddr);      // set sensor target
//      _i2c->write(_register_addr);            // set memory pointer
//      _i2c->endTransmission();
//      _i2c->requestFrom((int)_i2caddr, (int) 2); // request two bytes
//      uint8_t _registerDataLo = _i2c->read(); // get low byte
//      uint8_t _registerDataHi = _i2c->read(); // get high byte

    uint8_t readArray[2] = { 0, 0 };


   int err = i2c_.readReg(_register_addr, readArray, 2);
    if (err < 0) {
        printf("Adafruit_PWMServoDriver::getPWM readReg ERROR\n");
        return -1;
    }

    _value = (((int) readArray[1]) << 8) | readArray[0]; // combine two bytes


    _value_d = round (_value * 4.88);
    //printf("Adafruit_PWMServoDriver::getPWM _value=%d _value_d=%f\n", _value, _value_d);
    return (_value_d);



}

/*!
 *  @brief  Sets the PWM output of one of the PCA9685 pins
 *  @param  num One of the PWM output pins, from 0 to 15
 *  @param  on At what point in the 4096-part cycle to turn the PWM output ON
 *  @param  off At what point in the 4096-part cycle to turn the PWM output OFF
 */
void Adafruit_PWMServoDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
#ifdef ENABLE_DEBUG_OUTPUT
//  Serial.print("Setting PWM ");
//  Serial.print(num);
//  Serial.print(": ");
//  Serial.print(on);
//  Serial.print("->");
//  Serial.println(off);
#endif
    if (!connected_) {
        //printf("\nAdafruit_PWMServoDriver::setPWM in servo%d NOT CONNECTED !", num);
        return;
    }
    uint8_t reg_address = PCA9685_LED0_ON_L + 4 * num;
    uint8_t values[] = { (uint8_t) on, (uint8_t) (on >> 8), (uint8_t) off, (uint8_t) (off >> 8) };

    int err = i2c_.writeReg(reg_address, values, 4);
    if (err < 0) {
        printf("\nError Adafruit_PWMServoDriver::setPWM writeReg");
        //exit(0);
    }


//  _i2c->beginTransmission(_i2caddr);
//  _i2c->write(PCA9685_LED0_ON_L + 4 * num);
//  _i2c->write(on);
//  _i2c->write(on >> 8);
//  _i2c->write(off);
//  _i2c->write(off >> 8);
//  _i2c->endTransmission();
}

/*!
 *   @brief  Helper to set pin PWM output. Sets pin without having to deal with
 * on/off tick placement and properly handles a zero value as completely off and
 * 4095 as completely on.  Optional invert parameter supports inverting the
 * pulse for sinking to ground.
 *   @param  num One of the PWM output pins, from 0 to 15
 *   @param  val The number of ticks out of 4096 to be active, should be a value
 * from 0 to 4095 inclusive.
 *   @param  invert If true, inverts the output, defaults to 'false'
 */
void Adafruit_PWMServoDriver::setPin(uint8_t num, uint16_t val, bool invert) {
    // Clamp value between 0 and 4095 inclusive.
    val = std::min(val, (uint16_t) 4095);
    if (invert) {
        if (val == 0) {
            // Special value for signal fully on.
            setPWM(num, 4096, 0);
        }
        else if (val == 4095) {
            // Special value for signal fully off.
            setPWM(num, 0, 4096);
        }
        else {
            setPWM(num, 0, 4095 - val);
        }
    }
    else {
        if (val == 4095) {
            // Special value for signal fully on.
            setPWM(num, 4096, 0);
        }
        else if (val == 0) {
            // Special value for signal fully off.
            setPWM(num, 0, 4096);
        }
        else {
            setPWM(num, 0, val);
        }
    }
}

/*!
 *  @brief  Sets the PWM output of one of the PCA9685 pins based on the input
 * microseconds, output is not precise
 *  @param  num One of the PWM output pins, from 0 to 15
 *  @param  Microseconds The number of Microseconds to turn the PWM output ON
 */
void Adafruit_PWMServoDriver::writeMicroseconds(uint8_t num, uint16_t microseconds) {
#ifdef ENABLE_DEBUG_OUTPUT
//    Serial.print("Setting PWM Via Microseconds on output");
//    Serial.print(num);
//    Serial.print(": ");
//    Serial.print(Microseconds);
//    Serial.println("->");
    printf("\nSetting PWM Via Microseconds on output %d :microsec %d", num , microseconds);
#endif

    double pulse = (double) microseconds;
    double pulselength;

    pulselength = 1000000.0; // 1,000,000 us per second

    // Read prescale
    uint8_t prescale = readPrescale();

#ifdef ENABLE_DEBUG_OUTPUT
    printf("\nPCA9685 chip prescale %d", prescale);
//    Serial.print(prescale);
//    Serial.println(" PCA9685 chip prescale");
#endif

    // Calculate the pulse for PWM based on Equation 1 from the datasheet section
    // 7.3.5
    prescale += 1;
    pulselength *= prescale;
    pulselength /= _oscillator_freq;

#ifdef ENABLE_DEBUG_OUTPUT
    printf("\npulselength: %f us per bit : _oscillator_freq = %d", pulselength, _oscillator_freq);
//    Serial.print(pulselength);
//    Serial.println(" us per bit");
#endif

    pulse /= pulselength;

#ifdef ENABLE_DEBUG_OUTPUT
    printf("\npulse: %f  %d pulse for PWM", pulse, (uint16_t)pulse);
//    Serial.print(pulse);
//    Serial.println(" pulse for PWM");
#endif

    setPWM(num, 0, (uint16_t) pulse);
}

void Adafruit_PWMServoDriver::fastWriteMicroseconds(uint8_t num, uint16_t microseconds) {
    //Prérequis:
    //prescale = 0x79
    //_oscillator_freq = 25000000
    //freq = 50Hz

    double pulse = (double) microseconds;
    pulse = round(pulse / 4.88);
    setPWM(num, 0, (uint16_t) pulse);
}

/*!
 *  @brief  Getter for the internally tracked oscillator used for freq
 * calculations
 *  @returns The frequency the PCA9685 thinks it is running at (it cannot
 * introspect)
 */
uint32_t Adafruit_PWMServoDriver::getOscillatorFrequency(void) {
    return _oscillator_freq;
}

/*!
 *  @brief Setter for the internally tracked oscillator used for freq
 * calculations
 *  @param freq The frequency the PCA9685 should use for frequency calculations
 */
void Adafruit_PWMServoDriver::setOscillatorFrequency(uint32_t freq) {
    _oscillator_freq = freq;
}

/******************* Low level I2C interface */
uint8_t Adafruit_PWMServoDriver::read8(uint8_t addr_reg) {
    if (!connected_) {
        //printf("\nAdafruit_PWMServoDriver::read8 NOT CONNECTED ! \n");
        return -1;
    }
    uint8_t readValue = 0;
    i2c_.readReg(addr_reg, &readValue, 1);
    return readValue;
//  _i2c->beginTransmission(_i2caddr);
//  _i2c->write(addr);
//  _i2c->endTransmission();
//
//  _i2c->requestFrom((uint8_t)_i2caddr, (uint8_t)1);
//  return _i2c->read();
}

int Adafruit_PWMServoDriver::write8(uint8_t addr_reg, uint8_t d) {
    if (!connected_) {
        //printf("\nAdafruit_PWMServoDriver::write8 NOT CONNECTED = %d! \n", connected_);
        return -99;
    }

    int err = i2c_.writeReg(addr_reg, &d, 1);
    if (err < 0) {
        //printf("\nError Adafruit_PWMServoDriver i2c write8 writeReg\n");
        return -1;
    }
    return 0;
//  _i2c->beginTransmission(_i2caddr);
//  _i2c->write(addr);
//  _i2c->write(d);
//  _i2c->endTransmission();
}
