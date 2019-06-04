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

    temp_ignoreRearCollision_ = false;
    temp_ignoreFrontCollision_ = false;
    temp_forceRotation_ = false;

    matchColorPosition_ = false;

}
Asserv::~Asserv()
{
    delete asservdriver_;
    delete pMovingBase_;
    delete pAsservInsa_;
    delete pAsservEsialR_;
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
        //asservdriver_->path_InterruptTrajectory(); //TODO path_InterruptTrajectory() utile ? car caresh en coredump sur le lego en SIMU
        asservdriver_->motion_ActivateManager(false);
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        //pAsservEsialR_->path_InterruptTrajectory();//TODO path_InterruptTrajectory() utile ?
        pAsservEsialR_->stopAsserv();
    }
}

void Asserv::setLowSpeedForward(bool enable, int percent)
{
    logger().error() << "Surcharge à faire par config Robot!!!!!!!! percent " << percent << logs::end;
    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO setLowSpeed ASSERV_INT_INSA NOT EXIST !!!" << logs::end;

    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->motion_setLowSpeedForward(enable, percent);
    } else if (useAsservType_ == 0)
        asservdriver_->motion_setLowSpeedForward(enable, percent);
}

void Asserv::setLowSpeedBackward(bool enable, int percent)
{
    logger().error() << "Surcharge à faire par config Robot!!!!!!!! percent "<< percent << logs::end;
    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO setLowSpeed ASSERV_INT_INSA NOT EXIST!!!" << logs::end;
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->motion_setLowSpeedBackward(enable, percent);
    } else if (useAsservType_ == 0)
        asservdriver_->motion_setLowSpeedBackward(enable, percent);
}
void Asserv::disablePID() //deprecated
{
    freeMotion();
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

//doit etre surcharger par robot
bool Asserv::filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm)
{

    logger().error() << "Surcharge à faire par config Robot!!!!!!!!" << logs::end;
    return false;

}

void Asserv::resetFrontCollisionOnTraj()
{
    if (useAsservType_ == ASSERV_INT_INSA)
    {
        //pAsservInsa_->path_ResetEmergencyStop();
    }
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver_->path_ResetEmergencyStop();
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->path_ResetEmergencyStop();
}

void Asserv::warnFrontCollisionOnTraj()
{
    if (temp_forceRotation_) {
        //logger().error() << "forceRotation_ = " << forceRotation_ << logs::end;
        return;
    }
    if (temp_ignoreFrontCollision_)
        return;

    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_->path_CollisionOnTrajectory();
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver_->path_CollisionOnTrajectory();
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->path_CollisionOnTrajectory();

}

void Asserv::warnBackCollisionOnTraj()
{
    if (temp_forceRotation_)
        return;
    if (temp_ignoreRearCollision_)
        return;

    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_->path_CollisionRearOnTrajectory();
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver_->path_CollisionRearOnTrajectory();
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->path_CollisionRearOnTrajectory();
}

TRAJ_STATE Asserv::doLineAbs(float distance_mm) // if distance <0, move backward
{

    if (distance_mm > 0) {
        temp_ignoreRearCollision_ = true;
    } else {
        temp_ignoreFrontCollision_ = true;
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
    else
        ts = TRAJ_ERROR;
    //logger().info() << "Asserv::doLineAbs f=" << f << " r=" << r << logs::end;

    if (distance_mm > 0) {
        temp_ignoreRearCollision_ = false;
    } else {
        temp_ignoreFrontCollision_ = false;
    }

    return ts;
}

TRAJ_STATE Asserv::doRotateAbs(float degreesRelative)
{
    //logger().debug() << "doRotateAbs degrees=" << degrees << " ignorefront=" << temp_ignoreFrontCollision_ << logs::end;

    temp_forceRotation_ = true;

    TRAJ_STATE ts;
    float radians = (degreesRelative * M_PI) / 180.0f;

    if (useAsservType_ == ASSERV_INT_INSA)
        ts = pAsservInsa_->motion_DoRotate(radians);
    else if (useAsservType_ == ASSERV_EXT)
        ts = asservdriver_->motion_DoRotate(radians);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        ts = pAsservEsialR_->motion_DoRotate(radians);
    else
        ts = TRAJ_ERROR;

    temp_forceRotation_ = false;
//    logger().debug() << "END doRotateAbs degrees=" << degrees << " ignorefront=" << temp_ignoreFrontCollision_ << logs::end;
    return ts;
}
TRAJ_STATE Asserv::doRotateLeft(float degreesRelative) //TODO utiliser doRotateTo (qui contient le modulo ?)
{
    if (matchColorPosition_ != 0) {
        return doRotateAbs(-degreesRelative); //jaune
    } else
        return doRotateAbs(degreesRelative); //violet
}
TRAJ_STATE Asserv::doRotateRight(float degreesRelative)
{
    if (matchColorPosition_ != 0) {
        return doRotateAbs(degreesRelative); //jaune
    } else
        return doRotateAbs(-degreesRelative); //violet
}

TRAJ_STATE Asserv::doFaceTo(float xMM, float yMM)
{
    logger().debug() << "doFaceTo xMM=" << xMM << " yMM=" << yMM << logs::end;

    TRAJ_STATE ts;

    if (useAsservType_ == ASSERV_INT_INSA) {
        //TODO
        ts = TRAJ_ERROR;
        logger().error() << "TODO doFaceTo ASSERV_INT_INSA !!!" << logs::end;
    } else if (useAsservType_ == ASSERV_EXT)
        ts = asservdriver_->motion_DoFace(getRelativeX(xMM) / 1000.0, yMM / 1000.0);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        ts = pAsservEsialR_->motion_DoFace(getRelativeX(xMM) / 1000.0, yMM / 1000.0);
    else
        ts = TRAJ_ERROR;

    return ts;
}

//relative motion (depends on current position of the robot)
TRAJ_STATE Asserv::doRotateTo(float thetaInDegreeAbsolute)
{
//logger().debug() << "====2 doRotateTo thetaInDegree=" << thetaInDegree << "degrees " << logs::end;

    float currentThetaInDegree = pos_getThetaInDegree();
    float degrees = getRelativeAngle(thetaInDegreeAbsolute) - currentThetaInDegree;

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
TRAJ_STATE Asserv::doMoveForwardTo(float xMM, float yMM, bool rotate_ignored, float adjustment_mm)
{
    float dx = getRelativeX(xMM) - pos_getX_mm();
    float dy = yMM - pos_getY_mm();
    if (std::abs(dx) < 1.0 && std::abs(dy) < 1.0) { //Augmenter les valeurs??? par rapport à l'asserv fenetre d'arrivée
        logger().debug() << "___ TRAJ_FINISHED __doMoveForwardTo (std::abs(dx) < 1.0 && std::abs(dy) < 1.0)"
                << logs::end;
        return TRAJ_FINISHED;
    }
    float aRadian = atan2(dy, dx);
    logger().debug() << "doMoveForwardTo doRotateTo degrees=" << (aRadian * 180.0f) / M_PI << " dx=" << dx << " dy="
            << dy << "  (aRadian * 180.0f) / M_PI)= " << (aRadian * 180.0f) / M_PI << " get="
            << getRelativeAngle((aRadian * 180.0f) / M_PI) << " xMM=" << xMM << " yMM=" << yMM << " getX="
            << pos_getX_mm() << " getY=" << pos_getY_mm() << logs::end;

    TRAJ_STATE ts = doRotateTo(getRelativeAngle((aRadian * 180.0f) / M_PI));
    if (ts != TRAJ_FINISHED)
    {
        if(!rotate_ignored)
            return ts;
        else
        {
            logger().error() << " __on passe au doline !!!"  << logs::end;
        }
    }
    float dist = sqrt(dx * dx + dy * dy);
    logger().debug() << " __doMoveForwardTo dist sqrt(dx * dx + dy * dy)=" << dist << logs::end;

    return doLineAbs(dist + adjustment_mm);

}
TRAJ_STATE Asserv::doMoveBackwardTo(float xMM, float yMM, bool rotate_ignored)
{
    xMM = getRelativeX(xMM);

    float dx = xMM - pos_getX_mm();
    float dy = yMM - pos_getY_mm();
    if (std::abs(dx) < 1.0 && std::abs(dy) < 1.0) { //Augmenter les valeurs??? par rapport à l'asserv fenetre d'arrivée
        return TRAJ_FINISHED;
    }
    float aRadian = atan2(dy, dx);

    TRAJ_STATE ts = doRotateTo(getRelativeAngle(((M_PI + aRadian) * 180.0f) / M_PI));
    if (ts != TRAJ_FINISHED)
        {
            if(!rotate_ignored)
                return ts;
            else
            {
                //logger().error() << " __on passe au doline !!!"  << logs::end;
            }
        }

//    if (ts != TRAJ_FINISHED)
//        return ts;
    float dist = sqrt(dx * dx + dy * dy);
    return doLineAbs(-dist);
}
TRAJ_STATE Asserv::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
    TRAJ_STATE ts;
    ts = doMoveForwardTo(xMM, yMM);
    if (ts != TRAJ_FINISHED)
        return ts;

    ts = doRotateTo(thetaInDegree);
    return ts;
}
TRAJ_STATE Asserv::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
    TRAJ_STATE ts;
    ts = doMoveBackwardTo(xMM, yMM);
    if (ts != TRAJ_FINISHED)
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

bool Asserv::calculateDriftRightSideAndSetPos(float d2_theo_bordure_mm, float d2b_bordure_mm, float x_depart_mm,
        float y_depart_mm)
{
    logger().error() << "calculate : " << " d2_theo_bordure_mm= " << d2_theo_bordure_mm << " d2b_bordure_mm= "
            << d2b_bordure_mm << " x_depart_mm= " << x_depart_mm << " y_depart_mm= " << y_depart_mm << logs::end;

    if (abs(d2b_bordure_mm - d2_theo_bordure_mm) >= 5) {

//Partie théorique basé sur la position d'arrivée (que croit le robot)
        RobotPosition p = pos_getPosition();
        //tan teta = d1/l
        float dx = (p.x * 1000.0) - x_depart_mm;
        float dy = (p.y * 1000.0) - y_depart_mm;
        float l_theo_mm = std::sqrt(dx * dx + dy * dy);
        float d1_theo = d2_theo_bordure_mm; //TODO a modifier avec cos(beta)
        float teta_theo = atan2(d1_theo, l_theo_mm);

        logger().error() << "calculate : " << " dx= " << dx << " dy= " << dy << " l_theo_mm= " << l_theo_mm
                << " teta_theo_deg= " << teta_theo * 180.0 / M_PI << logs::end;

//partie reelle (avec la distance mesurée de la bordure
        //tan teta = d1b/l
        float d1_b = d2b_bordure_mm;    //TODO a modifier avec cos(beta)
        float teta_b = atan2(d1_b, l_theo_mm);

        float teta_error = teta_b - teta_theo;
        logger().error() << "calculate  : " << " teta_b_deg= " << teta_b * 180.0 / M_PI << " teta_error_deg= "
                << teta_error * 180.0 / M_PI << " p.x=" << p.x << " l_theo_mm / 1000.0=" << (l_theo_mm / 1000.0)
                << logs::end;

        float alpha = acos((float) ((dx) / (l_theo_mm)));
        float alpha_error = alpha - teta_error;

        logger().error() << "calculate  : " << " teta_b= " << teta_b << " teta_error= " << teta_error << " alpha_deg= "
                << alpha * 180.0 / M_PI << " alpha_error_deg= " << alpha_error * 180.0 / M_PI << logs::end;
        //changement des coordonnées
        float new_teta = p.theta + teta_error;
        float new_x = x_depart_mm + (l_theo_mm * cos(alpha_error));
        float new_y = y_depart_mm - (l_theo_mm * sin(alpha_error));

        logger().error() << "old position : " << " x= " << p.x << " y= " << p.y << " a_deg= " << p.theta * 180.0 / M_PI
                << logs::end;
        //pour 2 deg on a un decalage de 10mm
        float x_corr = (abs(teta_error) * 180.0 / M_PI) * 10.0 / 2.0;
        logger().error() << "x_corr : " << x_corr << logs::end;
        new_x = new_x + x_corr;

        logger().error() << "setPosition  : " << " x= " << new_x << " y= " << new_y << " a_rad= " << new_teta
                << " a_deg= " << new_teta * 180.0 / M_PI << logs::end;
        //regle de 3 pour modifier le x (decalage sur aire de depart qui influe sur le x)

        setPositionAndColor(new_x, new_y, new_teta * 180.0 / M_PI, matchColorPosition_);
        return true;
    } else
        return false;

}

bool Asserv::calculateDriftLeftSideAndSetPos(float d2_theo_bordure_mm, float d2b_bordure_mm, float x_depart_mm,
        float y_depart_mm)
{
    logger().error() << "calculate : " << " d2_theo_bordure_mm= " << d2_theo_bordure_mm << " d2b_bordure_mm= "
            << d2b_bordure_mm << " x_depart_mm= " << getRelativeX(x_depart_mm) << " y_depart_mm= " << y_depart_mm << logs::end;

    if (abs(d2b_bordure_mm - d2_theo_bordure_mm) >= 5) {

//Partie théorique basé sur la position d'arrivée (que croit le robot)
        RobotPosition p = pos_getPosition();
        //tan teta = d1/l
        float dx = (p.x * 1000.0) - getRelativeX(x_depart_mm);
        float dy = (p.y * 1000.0) - y_depart_mm;
        float l_theo_mm = std::sqrt(dx * dx + dy * dy);
        float d1_theo = d2_theo_bordure_mm; //TODO a modifier avec cos(beta)
        float teta_theo = atan2(d1_theo, l_theo_mm);

        logger().error() << "calculate : " << " dx= " << dx << " dy= " << dy << " l_theo_mm= " << l_theo_mm
                << " teta_theo_deg= " << teta_theo * 180.0 / M_PI << logs::end;

//partie reelle (avec la distance mesurée de la bordure
        //tan teta = d1b/l
        float d1_b = d2b_bordure_mm;    //TODO a modifier avec cos(beta)
        float teta_b = atan2(d1_b, l_theo_mm);

        float teta_error = teta_b - teta_theo;
        logger().error() << "calculate  : " << " teta_b_deg= " << teta_b * 180.0 / M_PI << " teta_error_deg= "
                << teta_error * 180.0 / M_PI << " p.x=" << p.x << " l_theo_mm / 1000.0=" << (l_theo_mm / 1000.0)
                << logs::end;

        float alpha = acos((float) ((abs(dx)) / (l_theo_mm)));//yellow
        float alpha_error = alpha - teta_error;

        logger().error() << "calculate  : " << " teta_b= " << teta_b << " teta_error= " << teta_error << " alpha_deg= "
                << alpha * 180.0 / M_PI << " alpha_error_deg= " << alpha_error * 180.0 / M_PI << logs::end;
        //changement des coordonnées
        float new_teta = (p.theta - teta_error);
        float new_x = getRelativeX(x_depart_mm) - (l_theo_mm * cos(alpha_error)); //yellow
        float new_y = y_depart_mm - (l_theo_mm * sin(alpha_error));

        logger().error() << "old position : " << " x= " << p.x << " y= " << p.y << " a_deg= " << p.theta * 180.0 / M_PI
                << logs::end;
        //pour 2 deg on a un decalage de 10mm
        float x_corr = (abs(teta_error) * 180.0 / M_PI) * 10.0 / 2.0;
        logger().error() << "x_corr : " << x_corr << logs::end;
        new_x = new_x + x_corr;

        logger().error() << "setPosition  : " << " x= " << new_x << " y= " << new_y << " a_rad= " << new_teta
                << " a_deg= " << new_teta * 180.0 / M_PI << logs::end;
        //regle de 3 pour modifier le x (decalage sur aire de depart qui influe sur le x)

        setPositionAndColor(getRelativeX(new_x), new_y, getRelativeAngle(new_teta * 180.0 / M_PI), matchColorPosition_);

        return true;
    } else
        return false;

}

void Asserv::doActivateReguAngle(bool enable)
{
    if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->motion_ActivateReguAngle(enable);
    } else if (useAsservType_ == ASSERV_EXT) {
        asservdriver_->motion_ActivateReguAngle(enable);
    }
}

TRAJ_STATE Asserv::doCalage(int dist, int percent)
{
    logger().info() << "doCalage" << logs::end;
    if (useAsservType_ == ASSERV_INT_ESIALR) {
        if (dist > 0)
            pAsservEsialR_->motion_setLowSpeedForward(true, percent);
        else if (dist < 0)
            pAsservEsialR_->motion_setLowSpeedBackward(true, percent);

        pAsservEsialR_->motion_ActivateReguAngle(false);
        pAsservEsialR_->motion_ActivateReguDist(true);
        pAsservEsialR_->motion_AssistedHandling();

        TRAJ_STATE ts = pAsservEsialR_->motion_DoDirectLine(dist / 1000.0); //sans asservissement L/R
        logger().error() << "calage ts=" << ts << logs::end;

        pAsservEsialR_->path_CancelTrajectory();
        pAsservEsialR_->motion_setLowSpeedForward(false);
        pAsservEsialR_->motion_setLowSpeedBackward(false);
        pAsservEsialR_->motion_ActivateReguAngle(true);
        pAsservEsialR_->motion_ActivateReguDist(true);

        return ts;
    } else if (useAsservType_ == ASSERV_EXT) {

        if (dist > 0)
            asservdriver_->motion_setLowSpeedForward(true, percent);
        else if (dist < 0)
            asservdriver_->motion_setLowSpeedBackward(true, percent);

        asservdriver_->motion_ActivateReguAngle(false);
        asservdriver_->motion_ActivateReguDist(true);
        asservdriver_->motion_AssistedHandling();

        TRAJ_STATE ts = asservdriver_->motion_DoDirectLine(dist / 1000.0); //sans asservissement L/R
        logger().error() << "ts=" << ts << logs::end;
        //sleep(3);
        asservdriver_->path_CancelTrajectory();
        asservdriver_->motion_setLowSpeedForward(false);
        asservdriver_->motion_setLowSpeedBackward(false);
        asservdriver_->motion_ActivateReguAngle(true);
        asservdriver_->motion_ActivateReguDist(true);

        return ts;
    } else
        return TRAJ_ERROR;

}

TRAJ_STATE Asserv::doCalage2(int dist, int tempo, int percent)
{
    logger().info() << "doCalage2" << logs::end;

    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO doCalage ASSERV_INT_INSA !!!" << logs::end;
    } else if (useAsservType_ == ASSERV_EXT) {
        //set low speed
        asservdriver_->motion_setLowSpeedForward(true, percent);
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

        asservdriver_->motion_setLowSpeedForward(false, 0);
        asservdriver_->motion_setLowSpeedBackward(false, 0);
        asservdriver_->motion_ActivateReguAngle(true);

        assistedHandling();
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        logger().error() << "TODO doCalage ASSERV_INT_ESIALR !!!" << logs::end;
        //set low speed
        pAsservEsialR_->motion_setLowSpeedForward(true, percent);
        pAsservEsialR_->motion_setLowSpeedBackward(true, percent);

        pAsservEsialR_->motion_ActivateReguAngle(false);

        pAsservEsialR_->motion_DoDirectLine(dist / 1000.0); //sans asservissement L/R
        sleep(tempo);
        pAsservEsialR_->path_CancelTrajectory();
        pAsservEsialR_->path_ResetEmergencyStop();

        //reset
        pAsservEsialR_->motion_ResetReguAngle();
        pAsservEsialR_->motion_ResetReguDist();

        //reactive

        pAsservEsialR_->motion_setLowSpeedForward(false, 0);
        pAsservEsialR_->motion_setLowSpeedBackward(false, 0);
        pAsservEsialR_->motion_ActivateReguAngle(true);

        assistedHandling();
    }

    return TRAJ_ERROR;
}

