#include "Asserv.hpp"

#include <unistd.h>
#include <cmath>

#include "../../Asserv.Esial/AsservEsialR.hpp"
#include "../../Asserv.Insa/AsservInsa.hpp"
#include "../../Log/Logger.hpp"
#include "MovingBase.hpp"

Asserv::Asserv(std::string botId, Robot * robot)
{
    pMovingBase_ = new MovingBase(botId);
    asservdriver = AAsservDriver::create(botId);
    probot_ = robot;

    useAsservType_ = ASSERV_INT_ESIALR; //default

    //init des objets
    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_ = new AsservInsa(robot);
    else
        pAsservInsa_ = NULL;

    if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_ = new AsservEsialR(robot);
    else
        pAsservEsialR_ = NULL;

    ignoreRearCollision_ = true;
    ignoreFrontCollision_ = true;
    matchColorPosition_ = false;
}

MovingBase * Asserv::base()
{
    return pMovingBase_;
}

void Asserv::startMotionTimerAndOdo(bool assistedHandlingEnabled)
{
    if (useAsservType_ == ASSERV_INT_INSA) {

        //TO BE surcharged because of the specific config file per robot
        logger().error() << "TODO startMotionTimerAndOdo  ASSERV_INT_INSA  TO BE surcharged !!!" << logs::end;

    } else if (useAsservType_ == ASSERV_INT_ESIALR) {

        //TO BE surcharged because of the specific config file per robot
        logger().error() << "TODO startMotionTimerAndOdo  ASSERV_INT_ESIALR  TO BE surcharged !!!" << logs::end;

    } else if (useAsservType_ == ASSERV_EXT) {

        asservdriver->motion_ActivateManager(true); //on active la carte d'asserv externe
        if (assistedHandlingEnabled)
            asservdriver->motion_AssistedHandling();
        else
            asservdriver->motion_FreeMotion();

    }
}

void Asserv::stopMotionTimerAndOdo()
{
    if (useAsservType_ == ASSERV_INT_INSA) {
        pAsservInsa_->motion_StopTimer();
    } else if (useAsservType_ == ASSERV_EXT) {
        asservdriver->path_InterruptTrajectory();
        asservdriver->motion_ActivateManager(false);
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->stopAsserv();
    }

}

void Asserv::setLowSpeed(bool enable)
{
    if (useAsservType_ == ASSERV_INT_INSA) {
        //TODO
        logger().error() << "TODO setLowSpeed ASSERV_INT_INSA !!!" << logs::end;

    } else if (useAsservType_ == ASSERV_INT_ESIALR) {

        pAsservEsialR_->motion_setLowSpeed(enable);

    } else if (useAsservType_ == 0)
        asservdriver->motion_setLowSpeed(enable);
}
void Asserv::disablePID() //deprecated TODO a remplacer par freemotion
{
    logger().error() << "TODO DEPRECATED disablePID !!!" << logs::end;
}

void Asserv::freeMotion()
{
    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_->motion_FreeMotion();
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver->motion_FreeMotion();
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->motion_FreeMotion();
}

void Asserv::assistedHandling()
{
    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_->motion_AssistedHandling();
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver->motion_AssistedHandling();
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->motion_AssistedHandling();
}

//WARNING matchColor = 0 => en bas à gauche
void Asserv::setPositionAndColor(float x_mm, float y_mm, float thetaInDegrees_, bool matchColor = 0)
{
//printf("matchcolor ORANGE=0 GREEN=1 : %d\n", matchColor);
    //logger().error() << "matchcolor VIOLET=0 YELLOWs=1 : " << matchColor << logs::end;

    setMatchColorPosition(matchColor);

    x_mm = getRelativeX(x_mm);
    float thetaInDegrees = getRelativeAngle(thetaInDegrees_);

    logger().error() << "matchcolor VIOLET=0 YELLOWs=1 : " << matchColor << "thetaInDegrees=" << thetaInDegrees_
            << "get=" << thetaInDegrees << logs::end;

    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
}

void Asserv::ignoreFrontCollision(bool ignore) //TODO rename setIgnoreFrontCollision
{
    ignoreFrontCollision_ = ignore;
}

void Asserv::ignoreRearCollision(bool ignore) //TODO rename setIgnoreRearCollision
{
    ignoreRearCollision_ = ignore;
}

RobotPosition Asserv::pos_getPosition()
{
    RobotPosition p;

    if (useAsservType_ == ASSERV_INT_INSA)
        p = pAsservInsa_->odo_GetPosition();
    else if (useAsservType_ == ASSERV_EXT)
        p = asservdriver->odo_GetPosition();
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        p = pAsservEsialR_->odo_GetPosition();
    return p;
}
float Asserv::pos_getX_mm()
{
    RobotPosition p = pos_getPosition();
    return p.x * 1000.0f;
}
float Asserv::pos_getY_mm()
{
    RobotPosition p = pos_getPosition();
    return p.y * 1000.0f;
}
// angle in radian
float Asserv::pos_getTheta()
{
    RobotPosition p = pos_getPosition();
    return p.theta;
}

// angle in degrees
float Asserv::pos_getThetaInDegree()
{
    return (pos_getTheta() * 180.0f) / M_PI;
}

void Asserv::setFrontCollision()
{
    logger().error() << "setFrontCollision ignoreFrontCollision_=" << ignoreFrontCollision_ << logs::end;

    if (!ignoreFrontCollision_) {

        if (useAsservType_ == ASSERV_INT_INSA)
            pAsservInsa_->path_CollisionOnTrajectory();
        else if (useAsservType_ == ASSERV_EXT)
            asservdriver->path_CollisionOnTrajectory();
        else if (useAsservType_ == ASSERV_INT_ESIALR)
            pAsservEsialR_->path_CollisionOnTrajectory();

    }
}

void Asserv::setRearCollision()
{
    logger().error() << "setRearCollision ignoreRearCollision_=" << ignoreRearCollision_ << logs::end;
    if (!ignoreRearCollision_) {
        if (useAsservType_ == ASSERV_INT_INSA)
            pAsservInsa_->path_CollisionRearOnTrajectory();
        else if (useAsservType_ == ASSERV_EXT)
            asservdriver->path_CollisionRearOnTrajectory();
        else if (useAsservType_ == ASSERV_INT_ESIALR)
            pAsservEsialR_->path_CollisionRearOnTrajectory();
    }
}

TRAJ_STATE Asserv::doLineAbs(float distance_mm) // if distance <0, move backward
{
//TODO test si l'adversaire est toujours present, si oui on ne cherche pas à avancer
//if(asservdriver->path_CollisionOnTrajectory())
//    return TRAJ_COLLISION;

    int f = ignoreFrontCollision_;
    int r = ignoreRearCollision_;
    if (distance_mm > 0) {
        ignoreRearCollision_ = true;
    } else {
        ignoreFrontCollision_ = true;
    }

    float meters = distance_mm / 1000.0f;
    //logger().info() << "Asserv::doLineAbs meters=" << meters << " mm=" << distance_mm << logs::end;
    TRAJ_STATE ts;

    if (useAsservType_ == ASSERV_INT_INSA)
        ts = pAsservInsa_->motion_DoLine(meters);
    else if (useAsservType_ == ASSERV_EXT)
        ts = asservdriver->motion_DoLine(meters);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        ts = pAsservEsialR_->motion_DoLine(meters);

    //logger().info() << "Asserv::doLineAbs f=" << f << " r=" << r << logs::end;

    ignoreFrontCollision_ = f;
    ignoreRearCollision_ = r;
    return ts;
}

TRAJ_STATE Asserv::doRotateAbs(float degrees)
{
    logger().debug() << "doRotateAbs degrees=" << degrees << logs::end;

    int f = ignoreFrontCollision_;
    int r = ignoreRearCollision_;
    ignoreRearCollision_ = true;
    ignoreFrontCollision_ = true;

    TRAJ_STATE ts;
    float radians = (degrees * M_PI) / 180.0f;

    if (useAsservType_ == ASSERV_INT_INSA)
        ts = pAsservInsa_->motion_DoRotate(radians);
    else if (useAsservType_ == ASSERV_EXT)
        ts = asservdriver->motion_DoRotate(radians);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        ts = pAsservEsialR_->motion_DoRotate(radians);

    logger().debug() << "Asserv::doRotateAbs f=" << f << " r=" << r << logs::end;
    ignoreFrontCollision_ = f;
    ignoreRearCollision_ = r;

    return ts;
}
TRAJ_STATE Asserv::doRotateLeft(float degrees)
{
    return doRotateAbs(degrees);
}
TRAJ_STATE Asserv::doRotateRight(float degrees)
{
    return doRotateAbs(-degrees);
}

TRAJ_STATE Asserv::doFaceTo(float xMM, float yMM)
{
    logger().debug() << "doFaceTo xMM=" << xMM << " yMM=" << yMM << logs::end;

    TRAJ_STATE ts;

    if (useAsservType_ == ASSERV_INT_INSA) {
        //TODO
        logger().error() << "TODO doFaceTo ASSERV_INT_INSA !!!" << logs::end;
    } else if (useAsservType_ == ASSERV_EXT)
        ts = asservdriver->motion_DoFace(xMM / 1000.0, yMM / 1000.0);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        ts = pAsservEsialR_->motion_DoFace(xMM / 1000.0, yMM / 1000.0);

    return ts;

}

//relative motion (depends on current position of the robot)
TRAJ_STATE Asserv::doRotateTo(float thetaInDegree)
{
//logger().debug() << "====2 doRotateTo thetaInDegree=" << thetaInDegree << "degrees " << logs::end;

    float currentThetaInDegree = pos_getThetaInDegree();
    float degrees = getRelativeAngle(thetaInDegree) - currentThetaInDegree;

// force it to be the positive remainder, so that 0 <= angle < 360
//degrees = (degrees + 360) % 360;
    degrees = (((int) (degrees * 1000.0f) + 360000) % 360000) / 1000.0f;

// reduce the angle
//reduction sur une plage de [0 à 360]
    if (degrees >= 360.0) {
        degrees = ((int) (degrees * 1000.0f) % 360000) / 1000.0f;

    }
    if (degrees < -360.0) {
        int d = (int) -(degrees * 1000.0f);
        d = d % 360000;
        degrees = -d / 1000.0f;
    }

// force into the minimum absolute value residue class, so that -180 < angle <= 180
    if (degrees >= 180)
        degrees -= 360;

//logger().debug() << "==== doRotateTo degrees=" << degrees << "degrees " << logs::end;
    TRAJ_STATE ts = doRotateAbs(degrees);

    return ts;
}
TRAJ_STATE Asserv::doMoveForwardTo(float xMM, float yMM, float adjustment)
{
    float dx = getRelativeX(xMM) - pos_getX_mm();
    float dy = yMM - pos_getY_mm();
    float aRadian = atan2(dy, dx);
    logger().error() << "doMoveForwardTo doRotateTo degrees=" << (aRadian * 180.0f) / M_PI << " dx=" << dx << " dy="
            << dy << "  (aRadian * 180.0f) / M_PI)= " << (aRadian * 180.0f) / M_PI << " get="
            << getRelativeAngle((aRadian * 180.0f) / M_PI) << " xMM=" << xMM << " yMM=" << yMM << " getX="
            << pos_getX_mm() << " getY=" << pos_getY_mm() << logs::end;

    doRotateTo(getRelativeAngle((aRadian * 180.0f) / M_PI)); //J'ai supprimé le getRelativeAngle ici :on doit utiliser Abs
    float dist = sqrt(dx * dx + dy * dy);
    logger().debug() << " __doMoveForwardTo dist sqrt(dx * dx + dy * dy)=" << dist << logs::end;
    return doLineAbs(dist + adjustment);
}
TRAJ_STATE Asserv::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
    TRAJ_STATE ts;
    ts = doMoveForwardTo(xMM, yMM);
    if (ts != TRAJ_OK)
        return ts;

    ts = doRotateTo(thetaInDegree);//je l'ai rajouté
    return ts;
}
TRAJ_STATE Asserv::doMoveBackwardTo(float xMM, float yMM)
{
    xMM = getRelativeX(xMM);

    float dx = xMM - pos_getX_mm();
    float dy = yMM - pos_getY_mm();
    float aRadian = atan2(dy, dx);

    doRotateTo(((M_PI + aRadian) * 180.0f) / M_PI); //todo a supprimer

    float dist = sqrt(dx * dx + dy * dy);
    return doLineAbs(-dist);
}
TRAJ_STATE Asserv::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
    TRAJ_STATE ts;
    ts = doMoveBackwardTo(xMM, yMM);
    if (ts != TRAJ_OK)
        return ts;
    ts = doRotateTo(thetaInDegree); //rajouté
    return ts;
}
TRAJ_STATE Asserv::doMoveArcRotate(int degrees, float radiusMM)
{
//TODO
    logger().error() << "TODO doMoveArcRotate !!!" << logs::end;
    return TRAJ_ERROR;
}

TRAJ_STATE Asserv::doCalage(int dist, int tempo)
{
    logger().info() << "doCalage" << logs::end;

    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO doCalage ASSERV_INT_INSA !!!" << logs::end;
    } else if (useAsservType_ == ASSERV_EXT) {
        //set low speed
        asservdriver->motion_setLowSpeed(true);

        asservdriver->motion_ActivateReguAngle(false);
        //asservdriver->motion_ActivateReguDist(true);

        //TRAJ_STATE ts = asservdriver->motion_DoLine(dist);
        //logger().info() << "ts=" << ts<< logs::end;
        asservdriver->motion_DoDirectLine(dist / 1000.0); //sans asservissement L/R
        sleep(tempo);
        asservdriver->path_CancelTrajectory();
        asservdriver->path_ResetEmergencyStop();

        //reset
        asservdriver->motion_ResetReguAngle();
        asservdriver->motion_ResetReguDist();

        //reactive
        asservdriver->motion_ActivateReguAngle(true);
        asservdriver->motion_setLowSpeed(false);

        assistedHandling();
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        logger().error() << "TODO doCalage ASSERV_INT_ESIALR !!!" << logs::end;
    }

    return TRAJ_ERROR;
}

