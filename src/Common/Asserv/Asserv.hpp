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

    bool forceRotation_;

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

    bool ignoreRearCollision_;
    bool ignoreFrontCollision_;

    //0=>LEFT with coordinate x, y, angle
    //1=>RIGHT with coordinate 3000-x, y , -angle
    bool matchColorPosition_;

    Robot * probot_; //reference du parent

public:

    /*!
     * \brief Constructor.
     *
     */
    Asserv(std::string botId, Robot * robot); //TODO robot is deprecated

    /*!
     * \brief Destructor.
     */
    virtual ~Asserv()
    {
    }

    /*!
     * \brief return objet movingBase.
     * \return movingBase_.
     */
    MovingBase * base();

    AAsservDriver* asservdriver;

    //Gestion de l'asservissement
    virtual void startMotionTimerAndOdo(bool assistedHandlingEnabled);
    virtual void stopMotionTimerAndOdo();
    virtual void disablePID(); //TODO deprecated
    virtual void setLowSpeed(bool enable);

    //modes d'arret de l'asservissement
    virtual void freeMotion();
    virtual void assistedHandling();
    //absolute motion
    virtual TRAJ_STATE doLineAbs(float distance_mm); // if distance <0, move backward
    virtual TRAJ_STATE doRotateAbs(float degrees);
    virtual TRAJ_STATE doRotateLeft(float degrees);
    virtual TRAJ_STATE doRotateRight(float degrees);
    virtual TRAJ_STATE doFaceTo(float xMM, float yMM);
    //relative motion (depends on current position of the robot)
    virtual TRAJ_STATE doRotateTo(float thetaInDegree);
    virtual TRAJ_STATE doMoveForwardTo(float xMM, float yMM, float adjustment = 0);
    virtual TRAJ_STATE doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree);
    virtual TRAJ_STATE doMoveBackwardTo(float xMM, float yMM);
    virtual TRAJ_STATE doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree);
    virtual TRAJ_STATE doMoveArcRotate(int degrees, float radiusMM);

    virtual TRAJ_STATE doCalage(int d, int d2);

    //void findPidAD(float degrees, int mm, int sec);
    //void findPidLR(float posl, int posr, int sec);
    //void configureAlphaPID(float Ap, float Ai, float Ad);
    //void configureDeltaPID(float Dp, float Di, float Dd);

    /*!
     * Attention startMotionTimerAndOdo() est necessaire auparavant pour configurer vTops et donc la position du robot
     */
    virtual void setPositionAndColor(float x_mm, float y_mm, float theta_degrees, bool matchColor); //matchColor = 0 =>en bas à gauche du log svg

    void setMatchColorPosition(bool c)
    {
        matchColorPosition_ = c;
    }

    //transformation suivant la couleur de match
    inline float getRelativeX(float x, float width = 0.0)
    {
        //printf("matchcolor:%d", matchColorPosition_);
        //logger().error() << "color==" << matchColorPosition_ << " width=" << width<< logs::end;
        if (matchColorPosition_ != 0) {
            return 3000 - x - width;
        }
        return x;
    }
    //transformation suivant la couleur de match
    inline float getRelativeAngle(float degrees)
    {
        if (matchColorPosition_ != 0) {
            //TODO limitAngle()
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

    bool filtreInsideTable(float mm);

    virtual void setFrontCollision(); //TODO Virtual ???? a verifier car peut etre surcharger?
    virtual void setRearCollision();

    virtual RobotPosition pos_getPosition();
    virtual float pos_getX_mm();
    virtual float pos_getY_mm();
    // angle in radian
    virtual float pos_getTheta();
    // angle in degrees
    virtual float pos_getThetaInDegree();

    void ignoreFrontCollision(bool ignore);
    void ignoreRearCollision(bool ignore);

    bool getIgnoreFrontCollision();
    bool getIgnoreRearCollision();
};

#endif
