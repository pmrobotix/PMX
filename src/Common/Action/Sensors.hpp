#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <sstream>
#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../Interface.Driver/ASensorsDriver.hpp"
#include "../Robot.hpp"
#include "../Utils/Chronometer.hpp"
#include "AActionsElement.hpp"
#include "ITimerPosixListener.hpp"

class ASensorsDriver;
class Robot;

class Sensors: public AActionsElement
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref Sensors.
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("Sensors");
        return instance;
    }

//    inline const logs::Logger& loggerSvg()
//    {
//        std::ostringstream s;
//        s << "Sensors4" << robot_->getID();
//        const logs::Logger &svg_ = logs::LoggerFactory::logger(s.str());
//        return svg_;
//    }

//    static inline const logs::Logger& loggerSvg()
//    {
//        static const logs::Logger &instance = logs::LoggerFactory::logger("SvgSensors");
//        return instance;
//    }

    Robot *robot_;
    ASensorsDriver *sensorsdriver_;

//    int diameterOpponent_mm_;
    bool remove_outside_table_;

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

    bool adv_is_detected_front_right_;
    bool adv_is_detected_back_right_;
    bool adv_is_detected_front_left_;
    bool adv_is_detected_back_left_;

    //2023
    bool is_cake_there_in_D2_;
    bool is_cake_there_in_D5_;
    bool is_cake_there_in_A5_;

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

    //distance de ce qu'il y a devant le robot
    float x_adv_mm;
    float y_adv_mm;

    /*!
     * \brief Constructor.
     *
     */
    Sensors(Actions &actions, Robot *robot);

    /*!
     * \brief Destructor.
     */
    ~Sensors();

    void toSVG();

    Robot* robot()
    {
        return robot_;
    }

    bool is_connected(); //is connected and alive

//    bool is_alive();

    void display(int n);

    inline bool getAvailableFrontCenter()
    {
//        logger().debug() << " ignoreFrontCenter_=" << ignoreFrontCenter_ << " enableFrontCenter_=" << enableFrontCenter_  << logs::end;
        return (enableFrontCenter_ & !ignoreFrontCenter_);
    }

    inline bool getAvailableBackCenter()
    {
        return (enableBackCenter_ & !ignoreBackCenter_);
    }

    float MultipleRightSide(int nb);
    float MultipleLeftSide(int nb);

    //capteur de distance
    int rightSide();
    int leftSide();
    float multipleRightSide(int nb);
    float multipleLeftSide(int nb);

    //acces directement aux capteurs
    int sync(std::string sensorname);

    //get la liste des positions adverses
    ASensorsDriver::bot_positions getPositionsAdv();
    //accès detection
    int front(bool display = false); //retourne le niveau de detection
    int back(bool display = false);

    //detection adversaire
    int right(bool display = false);
    int left(bool display = false);

    //activation des capteurs
    void addConfigFront(bool left, bool center, bool right);
    void addConfigBack(bool left, bool center, bool right);

    void remove_outside_table(bool enable);
    int filtre_levelInFront(int threshold_LR_mm, int threshold_Front_mm, int threshold_veryclosed_front_mm,
            float dist_adv_mm, float x_adv_mm, float y_adv_mm, float theta_adv_deg);

    int filtre_levelInBack(int threshold_mm, int threshold_veryclosed_mm, int dist_mm, int x_mm, int y_mm,
            float theta_deg);

//    void addThresholdDiameterOpponent_mm(int diam);

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
class SensorsTimer: public ITimerPosixListener //ITimerListener//
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorsTimer.
     */
    static const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("SensorsTimer");
        return instance;
    }

    /*!
     * \brief Référence vers sensors.
     */
    Sensors &sensors_;

    //utils::Chronometer chrono_;

    int lastdetect_front_level_;

    //int lastdetect_front_nb_;
    //int lastdetect_back_nb_;

    bool lastfrontl2_temp_;
    bool lastbackl2_temp_;

    int nb_sensor_front_a_zero;
    int nb_sensor_back_a_zero;

    int nb_ensurefront4;

    int nb_sensor_level2;
    int nb_sensor_b_level2;

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
    SensorsTimer(Sensors &sensors, int timeSpan_ms, std::string name);

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
