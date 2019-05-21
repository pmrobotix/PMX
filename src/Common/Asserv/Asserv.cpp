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
    asservdriver_ = AAsservDriver::create(botId);
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
    forceRotation_ = false;
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

        asservdriver_->motion_ActivateManager(true); //on active la carte d'asserv externe
        if (assistedHandlingEnabled)
            asservdriver_->motion_AssistedHandling();
        else
            asservdriver_->motion_FreeMotion();

    }
}

void Asserv::stopMotionTimerAndOdo()
{
    if (useAsservType_ == ASSERV_INT_INSA) {
        pAsservInsa_->motion_StopTimer();
    } else if (useAsservType_ == ASSERV_EXT) {
        asservdriver_->path_InterruptTrajectory();
        asservdriver_->motion_ActivateManager(false);
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->stopAsserv();
    }

}

void Asserv::setLowSpeedForward(bool enable, int percent)
{
    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO setLowSpeed ASSERV_INT_INSA NOT EXIST !!!" << logs::end;

    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->motion_setLowSpeedForward(enable, percent);
    } else if (useAsservType_ == 0)
        asservdriver_->motion_setLowSpeedForward(enable, percent);
}

void Asserv::setLowSpeedBackward(bool enable, int percent)
{
    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO setLowSpeed ASSERV_INT_INSA NOT EXIST!!!" << logs::end;
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->motion_setLowSpeedBackward(enable, percent);
    } else if (useAsservType_ == 0)
        asservdriver_->motion_setLowSpeedBackward(enable, percent);
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
        asservdriver_->motion_FreeMotion();
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->motion_FreeMotion();
}

void Asserv::assistedHandling()
{
    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_->motion_AssistedHandling();
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver_->motion_AssistedHandling();
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

    logger().debug() << "matchcolor VIOLET=0 YELLOW=1 : " << matchColor << "thetaInDegrees=" << thetaInDegrees_
            << "get=" << thetaInDegrees << logs::end;

    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
}
/*
 void Asserv::setIgnoreFrontNearObstacle(bool ignoreLeft, bool ignoreCenter, bool ignoreRight) //TODO comment preciser le capteur à ignorer??
 {
 ignoreFrontCollision_ = ignore;
 }
 bool Asserv::getIgnoreFrontNearObstacle()
 {
 return ignoreFrontCollision_;
 }

 void Asserv::setIgnoreBackNearObstacle(bool ignore)
 {
 ignoreRearCollision_ = ignore;
 }
 bool Asserv::getIgnoreBackNearObstacle()
 {
 return ignoreRearCollision_;
 }*/

RobotPosition Asserv::pos_getPosition()
{
    RobotPosition p;

    if (useAsservType_ == ASSERV_INT_INSA)
        p = pAsservInsa_->odo_GetPosition();
    else if (useAsservType_ == ASSERV_EXT)
        p = asservdriver_->odo_GetPosition();
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

//TODO Configuration à faire par robot ! donc a surcharger !
bool Asserv::filtreInsideTable(float metre)
{
    //On filtre si c'est pas à l'exterieur du terrain
    float x = 0.0;
    float y = 0.0;
    bool result = false;
    RobotPosition p = pos_getPosition();
    x = p.x + metre * cos(p.theta);
    y = p.y + metre * sin(p.theta);
    if ((x > 0.150 && x < 2.850) && (y > 0.150 && y < 1.500)) //en mètre
        result = true;
    else
        result = false;
    logger().debug() << "filtreInsideTable" << " p.x=" << p.x << " p.y=" << p.y << " p.T=" << p.theta << " x=" << x
            << " y=" << y << " result = " << result << logs::end;

    if (result) {
        return true; //si ok
    } else
        return false; //si en dehors de la table
}

void Asserv::warnFrontCollisionOnTraj()
{
    if (forceRotation_) {
        logger().error() << "forceRotation_ = " << forceRotation_ << logs::end;
        return;
    }

    float metre = 0.300; //distance devant le robot //a inclure dans le filtre et par configuration des robots

    if (filtreInsideTable(metre)) {

        if (useAsservType_ == ASSERV_INT_INSA)
            pAsservInsa_->path_CollisionOnTrajectory();
        else if (useAsservType_ == ASSERV_EXT)
            asservdriver_->path_CollisionOnTrajectory();
        else if (useAsservType_ == ASSERV_INT_ESIALR)
            pAsservEsialR_->path_CollisionOnTrajectory();
    } else {
        logger().error() << "filtreInsideTable" << logs::end;
    }
}

void Asserv::warnBackCollisionOnTraj()
{
    if (forceRotation_)
        return;

    float metre = -0.300; //distance derriere le robot

    if (filtreInsideTable(metre)) {
        if (useAsservType_ == ASSERV_INT_INSA)
            pAsservInsa_->path_CollisionRearOnTrajectory();
        else if (useAsservType_ == ASSERV_EXT)
            asservdriver_->path_CollisionRearOnTrajectory();
        else if (useAsservType_ == ASSERV_INT_ESIALR)
            pAsservEsialR_->path_CollisionRearOnTrajectory();
    }
}

TRAJ_STATE Asserv::doLineAbs(float distance_mm) // if distance <0, move backward
{
//TODO test si l'adversaire est toujours present, si oui on ne cherche pas à avancer  //a voir si toujorus d'actualité
//if(asservdriver->path_CollisionOnTrajectory())
//    return TRAJ_COLLISION;

    logger().debug() << "doLineAbs mm=" << distance_mm << " ignorefront=" << ignoreFrontCollision_ << logs::end;
    bool f = ignoreFrontCollision_;
    bool r = ignoreRearCollision_;
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
        ts = asservdriver_->motion_DoLine(meters);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        ts = pAsservEsialR_->motion_DoLine(meters);

    //logger().info() << "Asserv::doLineAbs f=" << f << " r=" << r << logs::end;

    ignoreFrontCollision_ = f;
    ignoreRearCollision_ = r;
    logger().debug() << "END doLineAbs mm=" << distance_mm << " ignorefront=" << ignoreFrontCollision_ << logs::end;
    return ts;
}

TRAJ_STATE Asserv::doRotateAbs(float degrees)
{
    logger().debug() << "doRotateAbs degrees=" << degrees << " ignorefront=" << ignoreFrontCollision_ << logs::end;

    bool f = ignoreFrontCollision_; //TODO a supprimer
    bool r = ignoreRearCollision_;
    ignoreRearCollision_ = true;
    ignoreFrontCollision_ = true;
    forceRotation_ = true;

    TRAJ_STATE ts;
    float radians = (degrees * M_PI) / 180.0f;

    if (useAsservType_ == ASSERV_INT_INSA)
        ts = pAsservInsa_->motion_DoRotate(radians);
    else if (useAsservType_ == ASSERV_EXT)
        ts = asservdriver_->motion_DoRotate(radians);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        ts = pAsservEsialR_->motion_DoRotate(radians);

    logger().debug() << "Asserv::doRotateAbs f=" << f << " r=" << r << logs::end;
    ignoreFrontCollision_ = f;
    ignoreRearCollision_ = r;
    forceRotation_ = false;
    logger().debug() << "END doRotateAbs degrees=" << degrees << " ignorefront=" << ignoreFrontCollision_ << logs::end;
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
        ts = asservdriver_->motion_DoFace(xMM / 1000.0, yMM / 1000.0);
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

    logger().debug() << "==== doRotateTo degrees=" << degrees << "degrees " << logs::end;
    TRAJ_STATE ts = doRotateAbs(degrees);

    return ts;
}
TRAJ_STATE Asserv::doMoveForwardTo(float xMM, float yMM, float adjustment)
{
    float dx = getRelativeX(xMM) - pos_getX_mm();
    float dy = yMM - pos_getY_mm();
    if (std::abs(dx) < 1.0 && std::abs(dy) < 1.0) { //Augmenter les valeurs??? par rapport à l'asserv fenetre d'arrivée
        return TRAJ_OK;
    }
    float aRadian = atan2(dy, dx);
    logger().debug() << "doMoveForwardTo doRotateTo degrees=" << (aRadian * 180.0f) / M_PI << " dx=" << dx << " dy="
            << dy << "  (aRadian * 180.0f) / M_PI)= " << (aRadian * 180.0f) / M_PI << " get="
            << getRelativeAngle((aRadian * 180.0f) / M_PI) << " xMM=" << xMM << " yMM=" << yMM << " getX="
            << pos_getX_mm() << " getY=" << pos_getY_mm() << logs::end;

    doRotateTo(getRelativeAngle((aRadian * 180.0f) / M_PI));
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

    ts = doRotateTo(thetaInDegree);
    return ts;
}
TRAJ_STATE Asserv::doMoveBackwardTo(float xMM, float yMM)
{
    xMM = getRelativeX(xMM);

    float dx = xMM - pos_getX_mm();
    float dy = yMM - pos_getY_mm();
    if (std::abs(dx) < 1.0 && std::abs(dy) < 1.0) { //Augmenter les valeurs??? par rapport à l'asserv fenetre d'arrivée
            return TRAJ_OK;
        }
    float aRadian = atan2(dy, dx);

    doRotateTo(((M_PI + aRadian) * 180.0f) / M_PI);

    float dist = sqrt(dx * dx + dy * dy);
    return doLineAbs(-dist);
}
TRAJ_STATE Asserv::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
    TRAJ_STATE ts;
    ts = doMoveBackwardTo(xMM, yMM);
    if (ts != TRAJ_OK)
        return ts;
    ts = doRotateTo(thetaInDegree);
    return ts;
}
TRAJ_STATE Asserv::doMoveArcRotate(int degrees, float radiusMM)
{
//TODO
    logger().error() << "TODO doMoveArcRotate !!!" << logs::end;
    return TRAJ_ERROR;
}

TRAJ_STATE Asserv::calculateDriftRightSideAndSetPos(float d2_theo_bordure_mm, float d2b_bordure_mm, float x_depart_mm, float y_depart_mm)
{

//Partie théorique basé sur la position d'arrivée (que croit le robot)
    RobotPosition p = pos_getPosition();
    //tan teta = d1/l
    float dx = (p.x * 1000.0) - x_depart_mm;
    float dy = (p.y * 1000.0) - y_depart_mm;
    float l_theo_mm = std::sqrt(dx * dx + dy * dy);
    float d1_theo = d2_theo_bordure_mm; //TODO a modifier avec cos(beta)
    float teta_theo = atan2(d1_theo, l_theo_mm);

//partie reelle (avec la distance mesurée de la bordure
    //tan teta = d1b/l
    float d1_b = d2b_bordure_mm;    //TODO a modifier avec cos(beta)
    float teta_b = atan2(d1_b, l_theo_mm);

    float teta_error = teta_theo - teta_b;
    float alpha = acos((p.x * 1000.0) / l_theo_mm);
    float alpha_error = alpha - teta_error;

    //changement des coordonnées
    float new_teta = p.theta + teta_error;
    float new_x = x_depart_mm + (l_theo_mm * cos(alpha_error));
    float new_y = y_depart_mm + (l_theo_mm * sin(alpha_error));

    setPositionAndColor(new_x, new_y, new_teta, matchColorPosition_);

    return TRAJ_OK;
}

TRAJ_STATE Asserv::doCalage(int dist, int tempo, int percent) //TODO ajouter le percent de vitesse
{
    logger().info() << "doCalage" << logs::end;

    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO doCalage ASSERV_INT_INSA !!!" << logs::end;
    } else if (useAsservType_ == ASSERV_EXT) {
        //set low speed
        asservdriver_->motion_setLowSpeedForward(true, percent); //TODO percent par config robot
        asservdriver_->motion_setLowSpeedBackward(true, percent);

        asservdriver_->motion_ActivateReguAngle(false);
        //asservdriver->motion_ActivateReguDist(true);

        //TRAJ_STATE ts = asservdriver->motion_DoLine(dist);
        //logger().info() << "ts=" << ts<< logs::end;
        asservdriver_->motion_DoDirectLine(dist / 1000.0); //sans asservissement L/R
        sleep(tempo);
        asservdriver_->path_CancelTrajectory();
        asservdriver_->path_ResetEmergencyStop();

        //reset
        asservdriver_->motion_ResetReguAngle();
        asservdriver_->motion_ResetReguDist();

        //reactive
        asservdriver_->motion_ActivateReguAngle(true);
        asservdriver_->motion_setLowSpeedForward(false, 0);
        asservdriver_->motion_setLowSpeedBackward(false, 0);

        assistedHandling();
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        logger().error() << "TODO doCalage ASSERV_INT_ESIALR !!!" << logs::end;
        //set low speed
        pAsservEsialR_->motion_setLowSpeedForward(true, 50);
        pAsservEsialR_->motion_setLowSpeedBackward(true, 50);

        pAsservEsialR_->motion_ActivateReguAngle(false);

        pAsservEsialR_->motion_DoDirectLine(dist / 1000.0); //sans asservissement L/R
        sleep(tempo);
        pAsservEsialR_->path_CancelTrajectory();
        pAsservEsialR_->path_ResetEmergencyStop();

        //reset
        pAsservEsialR_->motion_ResetReguAngle();
        pAsservEsialR_->motion_ResetReguDist();

        //reactive
        pAsservEsialR_->motion_ActivateReguAngle(true);

        pAsservEsialR_->motion_setLowSpeedForward(false, 0);
        pAsservEsialR_->motion_setLowSpeedBackward(false, 0);

        assistedHandling();
    }

    return TRAJ_ERROR;
}

