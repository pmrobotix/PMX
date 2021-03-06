#ifndef COMMON_ASSERV_HPP_
#define COMMON_ASSERV_HPP_

#include <string>

#include "../../Log/LoggerFactory.hpp"
#include "../Asserv.Driver/AAsservDriver.hpp"

class MovingBase;

class AsservEsialR;

class Robot;

class AsservInsa;

class AAsservDriver;

/*!
 * Asservissement of the robot.It contains default elements.
 */
class Asserv
{
private:

    /*!
     * \brief Return \ref Logger linked to \ref Asserv.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("Asserv");
        return instance;
    }



protected:

    /*!
     * \brief motorisation = motors + encoders
     */
    MovingBase * pMovingBase_;

    /*!
     * \brief type d'asservissement utilisé
     * \0=Asserdriver 1=ESIAL internal 2=INSA internal
     */
    //int useAsservType_;
    enum AsservType
    {
        ASSERV_EXT, ASSERV_INT_ESIALR, ASSERV_INT_INSA
    };

    AsservType useAsservType_;

    std::string botId_;


    AAsservDriver* asservdriver_;

    /*!
     * \brief asservissement interne INSA.
     * NULL si non defini
     */
    AsservInsa * pAsservInsa_;

    /*!
     * \brief asservissement interne ESIAL.
     * NULL si non defini
     */
    AsservEsialR * pAsservEsialR_;

    bool temp_ignoreRearCollision_;
    bool temp_ignoreFrontCollision_;
    bool temp_forceRotation_;

    //0=>LEFT with coordinate x, y, angle
    //1=>RIGHT with coordinate 3000-x, y , -angle
    bool matchColorPosition_;

    Robot * probot_; //reference du parent

    RobotPosition adv_pos_centre_;

public:

    /*!
     * \brief Constructor.
     *
     */
    Asserv(std::string botId, Robot * robot); //TODO robot is deprecated ?

    /*!
     * \brief Destructor.
     */
    virtual ~Asserv();

    /*!
     * \brief return objet movingBase.
     * \return movingBase_.
     */
    MovingBase * base();

    //Gestion de l'asservissement
    virtual void startMotionTimerAndOdo(bool assistedHandlingEnabled);

    virtual void setLowSpeedForward(bool enable, int percent = 0);
    virtual void setLowSpeedBackward(bool enable, int percent = 0);
    virtual void setPositionAndColor(float x_mm, float y_mm, float theta_degrees, bool matchColor); //matchColor = 0 =>en bas à gauche du log svg
    virtual bool filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm);
    virtual void warnFrontCollisionOnTraj(int frontlevel, float x_adv__mm, float y_adv_mm); // X, Y dans le repère du robot
    virtual void warnBackCollisionOnTraj(float x_adv_mm, float y_adv_mm); // X, Y dans le repère du robot
    virtual void update_adv();


    void resetEmergencyOnTraj(std::string message = "default");

    //modes d'arret de l'asservissement
    virtual void stopMotionTimerAndOdo();
    void disablePID(); //TODO deprecated
    void freeMotion();
    void assistedHandling();

    RobotPosition pos_getAdvPosition();
    RobotPosition pos_getPosition();
    float pos_getX_mm();
    float pos_getY_mm();
    // angle in radian
    float pos_getTheta();
    // angle in degrees
    float pos_getThetaInDegree();

    //relative motion (depends on current position of the robot)
    TRAJ_STATE doLineAbs(float distance_mm); // if distance <0, move backward
    TRAJ_STATE doRotateAbs(float degreesRelative);
    TRAJ_STATE doRelativeRotateBy(float thetaInDegreeRelative); //prend automatiquement un angle dans un sens ou dans l'autre suivant la couleur de match
    TRAJ_STATE doCalage(int d, int percent);

    //absolute motion (coordinates thinking in the first color of match)
    TRAJ_STATE doFaceTo(float xMM, float yMM);
    TRAJ_STATE doAbsoluteRotateTo(float thetaInDegreeAbsolute, bool rotate_ignored = false);
    TRAJ_STATE doMoveForwardTo(float xMM, float yMM, bool rotate_ignored = false, float adjustment = 0);
    TRAJ_STATE doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree, bool rotate_ignored = false);
    TRAJ_STATE doMoveBackwardTo(float xMM, float yMM, bool rotate_ignored = false);
    TRAJ_STATE doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree);
    TRAJ_STATE doMoveArcRotate(int degrees, float radiusMM);

    //void doActivateReguAngle(bool enable);

    //attentionLa couleur de match doit deja etre effectué !
    bool calculateDriftRightSideAndSetPos(float d2_theo_bordure_mm, float d2b_bordure_mm, float x_depart_mm,
            float y_depart_mm);
    bool calculateDriftLeftSideAndSetPos(float d2_theo_bordure_mm, float d2b_bordure_mm, float x_depart_mm,
            float y_depart_mm);

    void setMatchColorPosition(bool c)
    {
        matchColorPosition_ = c;
    }

    //transformation suivant la couleur de match
    inline float getRelativeX(float x_mm, float width = 0.0)
    {
        //printf("matchcolor:%d", matchColorPosition_);
        //logger().error() << "color==" << matchColorPosition_ << " width=" << width<< logs::end;
        if (matchColorPosition_ != 0) {
            return 3000 - x_mm - width;
        }
        return x_mm;
    }
    //transformation suivant la couleur de match
    inline float getRelativeAngle(float degrees)
    {
        if (matchColorPosition_ != 0) {
            float limit = (180 - degrees);
            if (limit >= 360)
                limit -= 360;
            return limit;
        }
        return degrees;
    }

    //TODO a tester
    inline float limitAngle(float degrees)
    {
        // On ajuste l'angle à parcourir pour ne pas faire plus d'un demi-tour
        // Exemple, tourner de 340 degrés est plus chiant que de tourner de -20 degrés
        if (degrees >= 180) {
            degrees -= 2.0 * 180;
        } else if (degrees < -180) {
            degrees += 2.0 * 180;
        }
        return degrees;
    }

};

#endif
