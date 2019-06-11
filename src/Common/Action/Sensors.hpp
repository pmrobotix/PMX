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

    int frontLeftThreshold_;
    int frontCenterThreshold_;
    int frontRightThreshold_;

    int frontLeftVeryClosedThreshold_;
    int frontCenterVeryClosedThreshold_;
    int frontRightVeryClosedThreshold_;

    int backLeftThreshold_;
    int backCenterThreshold_;
    int backRightThreshold_;

    int backLeftVeryClosedThreshold_;
    int backCenterVeryClosedThreshold_;
    int backRightVeryClosedThreshold_;

    bool enableFrontLeft_;
    bool enableFrontCenter_;
    bool enableFrontRight_;

    bool enableBackLeft_;
    bool enableBackCenter_;
    bool enableBackRight_;

    bool ignoreFrontLeft_;
    bool ignoreFrontCenter_;
    bool ignoreFrontRight_;

    bool ignoreBackLeft_;
    bool ignoreBackCenter_;
    bool ignoreBackRight_;



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

    float MultipleRightSide(int nb);
    float MultipleLeftSide(int nb);

    int rightSide();
    int leftSide();
    float multipleRightSide(int nb);
    float multipleLeftSide(int nb);

    //acces directement aux capteurs
    int sensorDist(std::string sensorname);

    //accès detection
    int front(bool display = false);
    int back(bool display = false);

    //activation des capteurs
    void addConfigFront(bool left, bool center, bool right);
    void addConfigBack(bool left, bool center, bool right);

    //configuration à partir du centre du robot
    void addThresholdFront(int left, int center, int right);
    void addThresholdBack(int left, int center, int right);
    void addThresholdFrontVeryClosed(int left, int center, int right);
    void addThresholdBackVeryClosed(int left, int center, int right);

    void setIgnoreFrontNearObstacle(bool ignoreLeft, bool ignoreCenter, bool ignoreRight);
    void setIgnoreBackNearObstacle(bool ignoreLeft, bool ignoreCenter, bool ignoreRight);
    void setIgnoreAllFrontNearObstacle(bool ignore);
    void setIgnoreAllBackNearObstacle(bool ignore);

    //Ajoute le timer des sensors de detection
    void addTimerSensors(int timespan_ms);

    //supprime le timer des sensors
    void stopTimerSensors();
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

    bool lastfrontl2_temp_;
    bool lastbackl2_temp_;

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
    }

    virtual void onTimer(utils::Chronometer chrono);

    virtual void onTimerEnd(utils::Chronometer chrono);

    virtual std::string info();

};

#endif
