#ifndef OPOS6UL_SERVOUSINGMOTORDRIVER_HPP_
#define OPOS6UL_SERVOUSINGMOTORDRIVER_HPP_

#include "../Common/Action.Driver/AServoUsingMotorDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class ServoUsingMotorDriver: public AServoUsingMotorDriver {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ServoUsingMotorDriver(OPOS6UL).
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ServoUsingMotorDriver.OPO");
        return instance;
    }

    int connected_;

protected:

public:
    /*!
     * \brief Constructor.
     */
    ServoUsingMotorDriver();

    /*!
     * \brief Destructor.
     */
    inline ~ServoUsingMotorDriver() {
    }

    bool is_connected();

    void setMotorPosition(int power, int pos, int timems);

    long getInternalEncoder();

    void resetEncoder(int pos);

    void stopMotor();

    int getMotorCurrent();

    void enableHardRegulation(bool enable);

};

#endif
