#ifndef SIMU_SENSORSDRIVER_HPP_
#define SIMU_SENSORSDRIVER_HPP_

#include "../Common/Action.Driver/ASensorsDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class SensorsDriver: public ASensorsDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(SIMU).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsDriver.SIMU");
        return instance;
    }

    Robot * robot_;

public:

    /*!
     * \brief Constructor.
     */
    SensorsDriver(Robot * robot);

    /*!
     * \brief Destructor.
     */
    ~SensorsDriver();

    bool is_connected();

    int getAnalogPinData();
    void displayNumber(int number);

    /*!
         * \brief Synch.ronise les données des capteurs.
         * \result renvoi -1 si erreur, zero sinon
         */
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
