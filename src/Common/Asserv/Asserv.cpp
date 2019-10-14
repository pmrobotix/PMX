#include "Asserv.hpp"

#include <cmath>
#include <cstdlib>

#include "../../Asserv.Esial/AsservEsialR.hpp"
#include "../../Asserv.Insa/AsservInsa.hpp"
#include "../../Log/Logger.hpp"
#include "MovingBase.hpp"

Asserv::Asserv(std::string botId, Robot *robot)
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
        pAsservEsialR_ = new AsservEsialR(robot); //TODO necessaire pour svg, botcodeur, et motors, essayer de donner uniquement l'asserv avec ce qu'il faut... a reflechir
    else
        pAsservEsialR_ = NULL;

    temp_ignoreRearCollision_ = false;
    temp_ignoreFrontCollision_ = false;
    temp_forceRotation_ = false;

    matchColorPosition_ = false;

    adv_pos_centre_ = { -100.0, -100.0, 0, 0 };

}
Asserv::~Asserv()
{
    delete asservdriver_;
    delete pMovingBase_;
    delete pAsservInsa_;
    delete pAsservEsialR_;
}

MovingBase* Asserv::base()
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
    logger().error() << "Asserv::setLowSpeedForward Surcharge à faire par config Robot!!!!!!!! percent " << percent
            << logs::end;
    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO setLowSpeed ASSERV_INT_INSA NOT EXIST !!!" << logs::end;

    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->motion_setLowSpeedForward(enable, percent);
    } else if (useAsservType_ == 0)
        asservdriver_->motion_setLowSpeedForward(enable, percent);
}

void Asserv::setLowSpeedBackward(bool enable, int percent)
{
    logger().error() << "Asserv::setLowSpeedBackward Surcharge à faire par config Robot!!!!!!!! percent " << percent
            << logs::end;
    if (useAsservType_ == ASSERV_INT_INSA) {
        logger().error() << "TODO setLowSpeed ASSERV_INT_INSA NOT EXIST!!!" << logs::end;
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->motion_setLowSpeedBackward(enable, percent);
    } else if (useAsservType_ == 0)
        asservdriver_->motion_setLowSpeedBackward(enable, percent);
}
void Asserv::disablePID() //deprecated and ActivateQuanramp to be defined
{

    if (useAsservType_ == ASSERV_INT_INSA) {
        freeMotion();
    } else if (useAsservType_ == ASSERV_INT_ESIALR) {
        pAsservEsialR_->motion_ActivateQuadRamp(false);
    } else if (useAsservType_ == 0) {
        freeMotion();
    }
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

    logger().debug() << "matchcolor [VIOLET=0 YELLOW=1]=" << matchColor << " thetaInDegrees=" << thetaInDegrees_
            << " getRelativeAngle=" << thetaInDegrees << " x_mm=" << x_mm << " y_mm=" << y_mm<< logs::end;

    if (useAsservType_ == ASSERV_INT_INSA)
        pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
    else if (useAsservType_ == ASSERV_EXT)
        asservdriver_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInDegrees * M_PI / 180.0);
}

void Asserv::setPositionReal(float x_mm, float y_mm, float thetaInRad)
{
    if (useAsservType_ == ASSERV_INT_INSA)
            pAsservInsa_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInRad);
        else if (useAsservType_ == ASSERV_EXT)
            asservdriver_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInRad);
        else if (useAsservType_ == ASSERV_INT_ESIALR)
            pAsservEsialR_->odo_SetPosition(x_mm / 1000.0, y_mm / 1000.0, thetaInRad);
}

RobotPosition Asserv::pos_getAdvPosition()
{
    return adv_pos_centre_;
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

    logger().error() << "Asserv::filtre_IsInsideTable Surcharge à faire par config Robot!!!!!!!!" << logs::end;
    return false;

}

void Asserv::resetEmergencyOnTraj(std::string message)
{
    logger().debug() << "resetEmergencyOnTraj message = " << message << logs::end;
    if (useAsservType_ == ASSERV_INT_INSA) {
        //pAsservInsa_->path_ResetEmergencyStop();
    } else if (useAsservType_ == ASSERV_EXT)
        asservdriver_->path_ResetEmergencyStop();
    else if (useAsservType_ == ASSERV_INT_ESIALR)
        pAsservEsialR_->path_ResetEmergencyStop();
}

void Asserv::update_adv()
{
    logger().info() << "update_adv tob surcharged = " << logs::end;
}

void Asserv::warnFrontCollisionOnTraj(int frontlevel, float x_adv_detect_mm, float y_adv_detect_mm)
{
    //logger().error() << "warnFrontCollisionOnTraj forceRotation_ = " << temp_forceRotation_ << logs::end;
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

    if (frontlevel >= 2) {
        if (useAsservType_ == ASSERV_INT_INSA) {

        }

        else if (useAsservType_ == ASSERV_EXT) {
            //resetEmergencyOnTraj();
//doLineAbs(-30);
//resetEmergencyOnTraj();
        } else if (useAsservType_ == ASSERV_INT_ESIALR) {

        }
    }

    //conversion de la position du le terrain et determination du centre du robot adverse
    float dist_centre_robot_mm = 200.0;
    float x_adv = 0.0;
    float y_adv = 0.0;
    if (x_adv_detect_mm >= 1.0 || y_adv_detect_mm > 1.0) {

        RobotPosition p_current = pos_getPosition();
        x_adv = p_current.x + ((x_adv_detect_mm + dist_centre_robot_mm) * cos(p_current.theta))
                - (y_adv_detect_mm * sin(p_current.theta));
        y_adv = p_current.y + ((x_adv_detect_mm + dist_centre_robot_mm) * sin(p_current.theta))
                + (y_adv_detect_mm * cos(p_current.theta));
        adv_pos_centre_ = { x_adv, y_adv, 0.0, 0 };

    } else {
        adv_pos_centre_ = { -100.0, -100.0, 0.0, 0 };
    }
    logger().debug() << "warnFrontCollisionOnTraj x_adv = " << x_adv << " y_adv = " << y_adv
            << "         x_adv_detect_mm = " << x_adv_detect_mm << " y_adv_detect_mm = " << y_adv_detect_mm
            << logs::end;
}

void Asserv::warnBackCollisionOnTraj(float x_adv_detect_mm, float y_adv_detect_mm) //x positif devant le robot, y positif le coté gauche
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

//conversion de la position du le terrain et determination du centre du robot adverse
    float dist_centre_robot_mm = 350.0;
    float x_adv = 0.0;
    float y_adv = 0.0;

    if (x_adv_detect_mm >= 1.0 || y_adv_detect_mm > 1.0) {
        RobotPosition p_current = pos_getPosition();
        x_adv = p_current.x + ((x_adv_detect_mm + dist_centre_robot_mm) * cos(p_current.theta))
                - (y_adv_detect_mm * sin(p_current.theta));
        y_adv = p_current.y + ((x_adv_detect_mm + dist_centre_robot_mm) * sin(p_current.theta))
                + (y_adv_detect_mm * cos(p_current.theta));
        adv_pos_centre_ = { x_adv, y_adv, 0.0, 0 };

    } else {
        adv_pos_centre_ = { -100.0, -100.0, 0.0, 0 };
    }
    logger().info() << "warnBackCollisionOnTraj x_adv = " << x_adv << " y_adv = " << y_adv
            << "         x_adv_detect_mm = " << x_adv_detect_mm << " y_adv_detect_mm = " << y_adv_detect_mm
            << logs::end;
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

    return ts;
}

TRAJ_STATE Asserv::doRelativeRotateBy(float thetaInDegreeRelative) //prend automatiquement un angle dans un sens ou dans l'autre suivant la couleur de match
{
    if (matchColorPosition_ != 0) {
        return doRotateAbs(-thetaInDegreeRelative); //jaune
    } else
        return doRotateAbs(thetaInDegreeRelative); //violet
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

//relative motion (depends on current position of the robot, thinking in the first color of match)
TRAJ_STATE Asserv::doAbsoluteRotateTo(float thetaInDegreeAbsolute, bool rotate_ignored)
{
//logger().debug() << "====2 doRotateTo thetaInDegree=" << thetaInDegree << "degrees " << logs::end;

    float currentThetaInDegree = pos_getThetaInDegree();
    float degrees = getRelativeAngle(thetaInDegreeAbsolute) - currentThetaInDegree;

// force it to be the positive remainder, so that 0 <= angle < 360

    degrees = (((int) (degrees * 1000.0f) + 360000) % 360000) / 1000.0f;

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

    TRAJ_STATE ts = doAbsoluteRotateTo(getRelativeAngle((aRadian * 180.0f) / M_PI), rotate_ignored);
    if (ts != TRAJ_FINISHED) {
        if (!rotate_ignored)
            return ts;
        else {
            //on resete
            resetEmergencyOnTraj("doMoveForwardTo rotate_ignored");
            logger().error() << " __on passe au doline !!!" << logs::end;
        }
    }
    float dist = sqrt(dx * dx + dy * dy);
//logger().error() << " __doMoveForwardTo dist sqrt(dx * dx + dy * dy)=" << dist << logs::end;
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

    TRAJ_STATE ts = doAbsoluteRotateTo(getRelativeAngle(((M_PI + aRadian) * 180.0f) / M_PI));
    if (ts != TRAJ_FINISHED) {
        if (!rotate_ignored)
            return ts;
        else {
            resetEmergencyOnTraj("doMoveBackwardTo rotate_ignored");
            //logger().error() << " __on passe au doline !!!"  << logs::end;
        }
    }

    float dist = sqrt(dx * dx + dy * dy);
    return doLineAbs(-dist);
}
//deprecated
TRAJ_STATE Asserv::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree, bool rotate_ignored)
{
    logger().error() << "doMoveForwardAndRotateTo deprecated !!!" << logs::end;
    TRAJ_STATE ts;
    ts = doMoveForwardTo(xMM, yMM, rotate_ignored);
    if (ts != TRAJ_FINISHED)
        return ts;

    ts = doAbsoluteRotateTo(thetaInDegree);
    return ts;
}
//deprecated
TRAJ_STATE Asserv::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
    logger().error() << "doMoveBackwardAndRotateTo deprecated !!!" << logs::end;
    TRAJ_STATE ts;
    ts = doMoveBackwardTo(xMM, yMM);
    if (ts != TRAJ_FINISHED)
        return ts;
    ts = doAbsoluteRotateTo(thetaInDegree);
    return ts;
}
TRAJ_STATE Asserv::doMoveArcRotate(int degrees, float radiusMM)
{
//TODO
    logger().error() << "TODO doMoveArcRotate !!!" << logs::end;
    return TRAJ_ERROR;
}

std::tuple<int, float, float> Asserv::eq_2CirclesCrossed_getXY(float x1, float y1, float d1, float x2, float y2,
        float d2, float robot_size_l_mm)
{
    logger().debug() << "eq_2CirclesCrossed_getXY x1=" << x1 << " y1=" << y1 << " d1=" << d1 << "  x2=" << x2 << " y2="
            << y2 << " d2=" << d2 << logs::end;
    //On définit y en fonction de x : y = ax +b
    float b = (((x2 * x2) + (y2 * y2) + (d1 * d1) - (d2 * d2) - (x1 * x1) - (y1 * y1)) / (2.0 * (y2 - y1)));

    float a = (x2 - x1) / (y2 - y1);

    //resolution de l'equation du second degré Ax² + Bx + C = 0
    float A = (a * a) + 1.0;

    float B = ((2.0 * y1 * a) - (2 * x1) - (2.0 * a * b));

    float C = ((x1 * x1) + (y1 * y1) - (2.0 * y1 * b) + (b * b) - (d1 * d1));

    return eq_2nd_deg_getXY(a, b, A, B, C, robot_size_l_mm);

}

float Asserv::eq_2nd_deg_getDelta(float A, float B, float C)
{
    return ((B * B) - (4.0 * A * C));
}

std::tuple<int, float, float> Asserv::eq_2nd_deg_getXY(float a, float b, float A, float B, float C,
        float robot_size_l_mm)
{
    logger().debug() << "eq_2nd_deg_getXY a=" << a << " b=" << b << " A=" << A << " B=" << B << " C=" << C << logs::end;
    float coord_x = 0.0, coord_y = 0.0;
    float delta = eq_2nd_deg_getDelta(A, B, C);
    if (delta < 0) {
        return std::make_tuple(0, -1, -1);
    }
    if (delta == 0) {
        coord_x = (-1.0 * B / (2.0 * A));
        coord_y = b - (a * coord_x);

        return std::make_tuple(1, coord_x, coord_y);
    }
    if (delta > 0) {
        float x1 = (((-1.0 * B) + std::sqrt(delta)) / (2.0 * A));
        float y1 = b - (a * x1); //b- ax
        float x2 = ((-1.0 * B - std::sqrt(delta)) / (2.0 * A));
        float y2 = b - (a * x2); //b- ax

        logger().debug() << " solutions : x1=" << x1 << " y1=" << y1 << "   x2=" << x2 << " y2=" << y2 << logs::end;
        //filtrage de la table et de la largeur du robot !!
        if (y1 < robot_size_l_mm && y2 > robot_size_l_mm) {
            coord_x = x2;
            coord_y = y2;
            return std::make_tuple(2, coord_x, coord_y);
        } else if (y2 < robot_size_l_mm && y1 > robot_size_l_mm) {
            coord_x = x1;
            coord_y = y1;
            return std::make_tuple(3, coord_x, coord_y);
        } else {

            //double solution impossible à déterminer
            return std::make_tuple(-2, -1, -1);
        }
    } else
        return std::make_tuple(-1, -1, -1);

}

bool Asserv::adjustRealPosition(float pos_x_start_mm, float pos_y_start_mm, RobotPosition p, float delta_jx_mm,
        float delta_ky_mm, float mesure_mm, float robot_size_l_mm)
{

    float pos_x_start_mm_conv = getRelativeX(pos_x_start_mm);
    logger().debug() << "adjustRealPosition : pos_x_start_mm=" << pos_x_start_mm_conv << " pos_y_start_mm="
            << pos_y_start_mm << " p.x=" << p.x << " p.y=" << p.y << " p.theta=" << p.theta << " degrees="
            << p.theta * 180 / M_PI << " delta_jx_mm=" << delta_jx_mm << " delta_ky_mm=" << delta_ky_mm << " mesure_mm="
            << mesure_mm

            << logs::end;

    float dist_real_mm = std::sqrt(
            (((p.x * 1000.0) - pos_x_start_mm_conv) * ((p.x * 1000.0) - pos_x_start_mm_conv))
                    + (((p.y * 1000.0) - pos_y_start_mm) * ((p.y * 1000.0) - pos_y_start_mm)));

    float dist_x_when_mesuring_mm = std::abs(p.x * 1000.0 - pos_x_start_mm_conv);
    float position_rel_theta_when_mesuring_rad = getRelativeAngle(p.theta * 180.0 / M_PI) * M_PI / 180.0; //on cherche juste l'angle relatif qu'on soit en couleur A ou B


    //calcul de l'angle entre les 2 rayons de cercle = angle_rad
    float alphap_rad = acos((dist_x_when_mesuring_mm / dist_real_mm));
    float gamma_rad = atan2(delta_jx_mm, (delta_ky_mm + mesure_mm));
    float angle_rad = M_PI_2 + alphap_rad + gamma_rad + position_rel_theta_when_mesuring_rad;

    //calcul de BCprim (distance centre robot au point de mesure)
    float BCprim_mm = std::sqrt((delta_jx_mm * delta_jx_mm) + ((delta_ky_mm + mesure_mm) * (delta_ky_mm + mesure_mm)));

    //calcul de la distance entre les 2 centres de cercle DCprim
    float DCprim_mm = std::sqrt(
            (dist_real_mm * dist_real_mm) + (BCprim_mm * BCprim_mm)
                    - (2.0 * dist_real_mm * BCprim_mm * cos(angle_rad)));

    //calcul de l'abcisse du 2eme centre de cercle par rapport au premier
    float dist_xDCp_mm = std::sqrt((DCprim_mm * DCprim_mm) - (pos_y_start_mm * pos_y_start_mm));

    //on determine le croisement des 2 cercles en fct des coord et des rayons
    auto r = eq_2CirclesCrossed_getXY(pos_x_start_mm_conv, pos_y_start_mm, dist_real_mm,
            getRelativeX(pos_x_start_mm, dist_xDCp_mm), 0.0, BCprim_mm, robot_size_l_mm);

    float err = std::get<0>(r);

    logger().debug() << "alphap_rad=" << alphap_rad << " deg=" << alphap_rad * 180 / M_PI << " gamma_rad=" << gamma_rad
            << " deg=" << gamma_rad * 180 / M_PI << " angle_rad=" << angle_rad << " deg=" << angle_rad * 180 / M_PI
            << " position_rel_theta_when_mesuring_rad=" << position_rel_theta_when_mesuring_rad << " dist_x_when_mesuring_mm="
            << dist_x_when_mesuring_mm << " dist_real_mm=" << dist_real_mm << " BCprim=" << BCprim_mm << " DCprim="
            << DCprim_mm << " dist_xDCprim_mm=" << dist_xDCp_mm << " err=" << err << " new_x_mm=" << std::get<1>(r)
            << " new_y_mm=" << std::get<2>(r) << logs::end;

    if (err <= 0)
        return false;

    float new_x_mm = std::get<1>(r);
    float new_y_mm = std::get<2>(r);

//calcul de l'angle de correction
    //ajouter la difference entre ancien alphap et le nouveau calculé à la position Theta
    float new_alphap = std::acos(std::abs((new_x_mm - pos_x_start_mm_conv)) / dist_real_mm);
    float new_teta = getRelativeAngle((position_rel_theta_when_mesuring_rad + (alphap_rad - new_alphap)) * 180.0 / M_PI) * M_PI / 180.0;

    logger().debug() << "new pos : x=" << new_x_mm << " y=" << new_y_mm << " a=" << new_teta << " degrees="
            << new_teta * 180 / M_PI << logs::end;
//set de la nouvelle position et angle
    setPositionReal(new_x_mm, new_y_mm, new_teta);

    return true; // il y a un resultat viable.
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
            << d2b_bordure_mm << " x_depart_mm= " << getRelativeX(x_depart_mm) << " y_depart_mm= " << y_depart_mm
            << logs::end;

    x_depart_mm = getRelativeX(x_depart_mm);

    if (abs(d2b_bordure_mm - d2_theo_bordure_mm) >= 5) {

//Partie théorique basé sur la position d'arrivée (que croit le robot)
        RobotPosition p = pos_getPosition();
        //tan teta = d1/l
        //float dx = (p.x * 1000.0) - getRelativeX(x_depart_mm);
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

        float alpha = acos((float) ((abs(dx)) / (l_theo_mm)));    //yellow
        float alpha_error = alpha - teta_error;

        logger().error() << "calculate  : " << " teta_b= " << teta_b << " teta_error= " << teta_error << " alpha_deg= "
                << alpha * 180.0 / M_PI << " alpha_error_deg= " << alpha_error * 180.0 / M_PI << logs::end;
        //changement des coordonnées
        float new_teta = (p.theta - teta_error);
        //float new_x = getRelativeX(x_depart_mm) - (l_theo_mm * cos(alpha_error)); //yellow
        float new_x = x_depart_mm - (l_theo_mm * cos(alpha_error)); //yellow
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
/*
 void Asserv::doActivateReguAngle(bool enable)
 {
 if (useAsservType_ == ASSERV_INT_ESIALR) {
 pAsservEsialR_->motion_ActivateReguAngle(enable);
 } else if (useAsservType_ == ASSERV_EXT) {
 asservdriver_->motion_ActivateReguAngle(enable);
 }
 }*/

TRAJ_STATE Asserv::doCalage(int dist, int percent)
{

    if (useAsservType_ == ASSERV_INT_ESIALR) {
        if (dist > 0)
            pAsservEsialR_->motion_setLowSpeedForward(true, percent);
        else if (dist < 0)
            pAsservEsialR_->motion_setLowSpeedBackward(true, percent);

        pAsservEsialR_->motion_ActivateReguAngle(false);
        pAsservEsialR_->motion_ActivateReguDist(true);
        pAsservEsialR_->motion_AssistedHandling();

        TRAJ_STATE ts = pAsservEsialR_->motion_DoDirectLine(dist / 1000.0); //sans asservissement L/R

        //pAsservEsialR_->path_CancelTrajectory();
        pAsservEsialR_->motion_setLowSpeedForward(false);
        pAsservEsialR_->motion_setLowSpeedBackward(false);
        pAsservEsialR_->motion_ActivateReguAngle(true);
        pAsservEsialR_->motion_ActivateReguDist(true);
        resetEmergencyOnTraj("doCalage");

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

        //asservdriver_->path_CancelTrajectory();
        asservdriver_->motion_setLowSpeedForward(false);
        asservdriver_->motion_setLowSpeedBackward(false);
        asservdriver_->motion_ActivateReguAngle(true);
        asservdriver_->motion_ActivateReguDist(true);
        resetEmergencyOnTraj("doCalage");

        return ts;
    } else
        return TRAJ_ERROR;
}
