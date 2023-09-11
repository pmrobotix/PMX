#ifndef SIMU_SENSORSDRIVER_HPP_
#define SIMU_SENSORSDRIVER_HPP_

#include <string>

#include "../Common/Interface.Driver/ARobotPositionShared.hpp"
#include "../Common/Interface.Driver/ASensorsDriver.hpp"
#include "../Log/Logger.hpp"
#include "../Log/LoggerFactory.hpp"
#include "../Log/SvgWriter.hpp"

class ARobotPosShared;

#define square(a)  (a)*(a)

using namespace std;

class SensorsDriver: public ASensorsDriver
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref SensorsDriver(SIMU).
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("SensorsDriver.SIMU");
        return instance;
    }

    static inline const logs::Logger& loggerSvg()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("SensorsDriver.SIMU.SVG");
        return instance;
    }

    bot_positions vadv_;  //tableau des pos des adv

    ROBOTPOSITION pos_pour_calcul_;
    ROBOTPOSITION pos_pour_calcul_prec_;
    ARobotPositionShared *robotPositionShared_;

    int x_adv_;
    int y_adv_;

public:

    /*!
     * \brief Constructor.
     */
    SensorsDriver(ARobotPositionShared *aRobotPositionShared);

    /*!
     * \brief Destructor.
     */
    ~SensorsDriver();

    bool is_connected();

    void displayNumber(int number);

    /*!
     * \brief Synch.ronise les données des capteurs.
     * \result renvoi -1 si erreur, zero sinon
     */
    int sync(); //synchronise les données avec la balise
    ASensorsDriver::bot_positions getvPositionsAdv(); //retourne les dernieres positions connues

    RobotPos transformPosTableToPosRobot(int nb, float x_table, float y_table);

    int rightSide();
    int leftSide();

    int frontLeft();
    int frontCenter();
    int frontRight();

    int backLeft();
    int backCenter();
    int backRight();

};

class SvgWriterExtended: public SvgWriter
{
private:
    logs::Logger::LoggerBuffer *fLogBuffer;
public:

    SvgWriterExtended(std::string botId);

    ~SvgWriterExtended()
    {
    }

    void writePosition_AdvPos(float x_adv_mm, float y_adv_mm, float x_pos_mm, float y_pos_mm, int color = 0);

    void writePosition_Bot(float x_mm, float y_mm, float angle_rad, int color = 0);

    void writePosition_BotPos(float x_mm, float y_mm, float angle_rad);

    void writeZone(const char* name, float minX, float minY, float width, float height, float startX, float startY,
            float startAngle_rad){}

    void writeIaPath(const char* zone1Name, const char* zone2Name, float x_mm, float y_mm){}

    void pathPolyline(std::string points){}
};


#endif
