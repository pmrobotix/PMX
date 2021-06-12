#ifndef SIMULAUNCHER_SENSORSDRIVER_HPP_
#define SIMULAUNCHER_SENSORSDRIVER_HPP_

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class SensorsDriver: public ASensorsDriver {
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(SIMULAUNCHER).
     */
    static inline const logs::Logger & logger() {
        static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsDriver.SIMULAUNCHER");
        return instance;
    }

public:

    /*!
     * \brief Constructor.
     */
    SensorsDriver();

    /*!
     * \brief Destructor.
     */
    ~SensorsDriver();

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

};

#endif
