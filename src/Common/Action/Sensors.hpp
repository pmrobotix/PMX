#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <string>

#include "../../Log/Logger.hpp"
#include "../../Log/LoggerFactory.hpp"
#include "../Utils/Chronometer.hpp"
#include "AActionsElement.hpp"
#include "ITimerListener.hpp"

class Robot;

class ASensorsDriver;

class Sensors: public AActionsElement
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref Sensors.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("Sensors");
        return instance;
    }

    Robot * robot_;

    ASensorsDriver* sensorsdriver_;

public:


    /*!
     * \brief Constructor.
     *
     */
    Sensors(Actions & actions, Robot * robot);

    /*!
     * \brief Destructor.
     */
    ~Sensors();

    Robot * robot()
    {
        return robot_;
    }

    int right();
    int left();

    bool front();
    bool rear();

    bool frontVeryClosed();
    bool rearVeryClosed();

    //Ajoute le timer des sensors de detection
    void addTimerSensors();

    //TODO supprimer la tache des sensors void removeTimerSensors();

};

/*!
 * \brief Le timer associé aux sensors
 *
 */
class SensorsTimer: public ITimerListener
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorsTimer.
     */
    static const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("SensorsTimer");
        return instance;
    }

    /*!
     * \brief Référence vers sensors.
     */
    Sensors & sensors_;

    utils::Chronometer chrono_;

    int lastdetect_front_nb_;
    int lastdetect_back_nb_;

public:

    /*!
     * \brief Constructeur de la classe.
     * \param sensors
     *        Reference vers l'objet associée.
     * \param timeSpan_ms
     *        Temps interval en millisecondes.
     * \param name
     *        Libellé du timer.
     */
    SensorsTimer(Sensors & sensors, int timeSpan_ms, std::string name);

    /*!
     * \brief Destructeur de la classe.
     */
    virtual inline ~SensorsTimer()
    {
        logger().debug() << "~SensorsTimer()" << logs::end;
    }

    virtual void onTimer(utils::Chronometer chrono);

    virtual void onTimerEnd(utils::Chronometer chrono);

    virtual std::string info();

};

#endif
