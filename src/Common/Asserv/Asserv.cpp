#include "Asserv.hpp"

#include <stdlib.h>
#include <thread>

#include "../../Asserv.Esial/AsservEsialR.hpp"
#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"
#include "../Robot.hpp"

Asserv::Asserv(std::string botId, Robot *robot) //TODO utiliser uniquement robot, puis robot->getID() dans le create en dessous
{

	asservdriver_ = AAsservDriver::create(botId, robot->sharedPosition());
	probot_ = robot;

	useAsservType_ = ASSERV_INT_ESIALR; //default internal asserv
	emergencyStop_ = false;

	//init des objets
	if (useAsservType_ == ASSERV_INT_ESIALR)
		pAsservEsialR_ = new AsservEsialR(robot); //TODO necessaire pour svg, botcodeur, et motors, essayer de donner uniquement l'asserv avec ce qu'il faut... a reflechir
	else
		pAsservEsialR_ = NULL;

	temp_ignoreBackDetection_ = false;
	temp_ignoreFrontDetection_ = false;
	temp_forceRotation_ = false;

	matchColorPosition_ = false;

	adv_pos_centre_ = { -100.0, -100.0, 0, 0 };

	//Configuration TABLE
	//table horizontale
	x_ground_table_ = 3000;
	//table verticale
	//x_ground_table_ = 2000;

	lowSpeedvalue_ = 30; //valeur par defaut qui est surchargée par chaque extension robot
	maxSpeedDistValue_ = 200;

}
Asserv::~Asserv()
{
	delete asservdriver_;
	delete pAsservEsialR_;
}
/*
 //TODO utiliser convertPositionBeaconToRepereTable dans ARobotPositionShared ?
 ROBOTPOSITION Asserv::convertPositionToRepereTable(float d_mm, float x_mm, float y_mm, float theta_deg, float *x_botpos,
 float *y_botpos)
 {
 ROBOTPOSITION p = pos_getPosition();
 //coordonnées de l'objet detecté sur la table// M_P/2
 //    *x_botpos = p.x + (d_mm * cos(p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f)));
 //    *y_botpos = p.y + (d_mm * sin(p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f)));
 float a = (p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f));
 std::fmod(a, 2 * M_PI);
 //    if (a < -M_PI)
 //        a += M_PI;
 //    if (a > M_PI)
 //        a -= M_PI;

 //ADV coord
 float fx_botpos= p.x + (d_mm * cos(a));
 float fy_botpos = p.y + (d_mm * sin(a));

 *x_botpos  = fx_botpos;
 *y_botpos = fy_botpos;
 logger().debug() << "DEBUG --xy_botpos= " << *x_botpos << " " << *y_botpos
 << " pos: " << p.x << " " << p.y << " p_deg:" << p.theta  * 180.0f / M_PI<< " --balise: " << d_mm << " " << x_mm << " "
 << y_mm << " t_deg:" << theta_deg << logs::end;


 return p;
 }
 */

void Asserv::endWhatTodo()
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{

		pAsservEsialR_->endWhatTodo();
	} else if (useAsservType_ == ASSERV_EXT)
	{

		asservdriver_->endWhatTodo();
	}
}

void Asserv::startMotionTimerAndOdo(bool assistedHandlingEnabled)
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{

		//TO BE surcharged because of the specific config file per robot
		logger().error() << "TODO startMotionTimerAndOdo  ASSERV_INT_ESIALR  TO BE surcharged !!!" << logs::end;

//        pAsservEsialR_->motion_ActivateManager(true); //on active le thread
//        if (assistedHandlingEnabled) pAsservEsialR_->motion_AssistedHandling();
//        else pAsservEsialR_->motion_FreeMotion();
	} else if (useAsservType_ == ASSERV_EXT)
	{

		asservdriver_->motion_ActivateManager(true); //on active la carte d'asserv externe et le thread de position
		if (assistedHandlingEnabled)
			asservdriver_->motion_AssistedHandling();
		else
			asservdriver_->motion_FreeMotion();

	}
}

void Asserv::stopMotionTimerAndOdo()
{

	if (useAsservType_ == ASSERV_EXT)
	{
		//asservdriver_->path_InterruptTrajectory(); //TODO path_InterruptTrajectory() utile ? car crash en coredump sur le lego en SIMU
		asservdriver_->motion_ActivateManager(false); //cancel the thread
	} else if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		//pAsservEsialR_->path_InterruptTrajectory();//TODO path_InterruptTrajectory() utile ?
		pAsservEsialR_->stopAsserv();
	}
}

void Asserv::setLowSpeedForward(bool enable, int percent)
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		pAsservEsialR_->motion_setLowSpeedForward(enable, percent);

	} else if (useAsservType_ == ASSERV_EXT) asservdriver_->motion_setLowSpeedForward(enable, percent);
}

void Asserv::setLowSpeedBackward(bool enable, int percent)
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		pAsservEsialR_->motion_setLowSpeedBackward(enable, percent);
	} else if (useAsservType_ == ASSERV_EXT) asservdriver_->motion_setLowSpeedBackward(enable, percent);
}

void Asserv::setMaxSpeed(bool enable, int speed_dist_percent, int speed_angle_percent)
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		//TODO setMaxSpeed ASSERV_INT_ESIALR

	} else if (useAsservType_ == ASSERV_EXT)
	{
		if (enable)
		{
			if (speed_dist_percent > 100) speed_dist_percent = 100;
			if (speed_angle_percent > 100) speed_angle_percent = 100;

			asservdriver_->motion_setMaxSpeed(true, speed_dist_percent, speed_angle_percent);

		} else
		{
			asservdriver_->motion_setMaxSpeed(false);
		}
	}
}

void Asserv::disablePID() //deprecated and ActivateQuanramp to be defined
{

	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		pAsservEsialR_->motion_ActivateQuadRamp(false);
	} else if (useAsservType_ == ASSERV_EXT)
	{
		freeMotion();
	}
}

void Asserv::freeMotion()
{
	if (useAsservType_ == ASSERV_EXT)
		asservdriver_->motion_FreeMotion();
	else if (useAsservType_ == ASSERV_INT_ESIALR) pAsservEsialR_->motion_FreeMotion();
}

void Asserv::assistedHandling()
{
	if (useAsservType_ == ASSERV_EXT)
		asservdriver_->motion_AssistedHandling();
	else if (useAsservType_ == ASSERV_INT_ESIALR) pAsservEsialR_->motion_AssistedHandling();
}

//WARNING matchColor = 0 => en bas à gauche
void Asserv::setPositionAndColor(float x_mm, float y_mm, float thetaInDegrees_, bool matchColor = 0)
{
	setMatchColorPosition(matchColor);

	x_mm = changeMatchX(x_mm);
	float thetaInRad = changeMatchAngleRad(degToRad(thetaInDegrees_));

	logger().debug() << "matchcolor [BLUE=0 YELLOW=1]=" << matchColor << " thetaInDegrees=" << thetaInDegrees_
			<< " getRelativeAngle=" << radToDeg(thetaInRad) << " x_mm=" << x_mm << " y_mm=" << y_mm << logs::end;

//    if (useAsservType_ == ASSERV_EXT)
//        asservdriver_->odo_SetPosition(x_mm, y_mm, thetaInRad);
//    else if (useAsservType_ == ASSERV_INT_ESIALR)
//        pAsservEsialR_->odo_SetPosition(x_mm, y_mm, thetaInRad);
	setPositionReal(x_mm, y_mm, thetaInRad);
}

void Asserv::setPositionReal(float x_mm, float y_mm, float thetaInRad)
{

	if (useAsservType_ == ASSERV_EXT)
		asservdriver_->odo_SetPosition(x_mm, y_mm, thetaInRad);
	else if (useAsservType_ == ASSERV_INT_ESIALR) pAsservEsialR_->odo_SetPosition(x_mm, y_mm, thetaInRad);
}

ROBOTPOSITION Asserv::pos_getAdvPosition()
{
	return adv_pos_centre_;
}

ROBOTPOSITION Asserv::pos_getPosition()
{
	ROBOTPOSITION p;

	if (useAsservType_ == ASSERV_EXT)
		p = asservdriver_->odo_GetPosition();
	else if (useAsservType_ == ASSERV_INT_ESIALR) p = pAsservEsialR_->odo_GetPosition();

//mise à jour de la position pour les sensors
	probot_->sharedPosition()->setRobotPosition(p);
	return p;
}
float Asserv::pos_getX_mm()
{
	ROBOTPOSITION p = pos_getPosition();
	return p.x;
}
float Asserv::pos_getY_mm()
{
	ROBOTPOSITION p = pos_getPosition();
	return p.y;
}
// angle in radian
float Asserv::pos_getTheta()
{
	ROBOTPOSITION p = pos_getPosition();
	return p.theta;
}

// angle in degrees
float Asserv::pos_getThetaInDegree()
{
	return (pos_getTheta() * 180.0f) / M_PI;
}

//TODO a corriger
//doit etre surcharger par robot , pour les detecteur de proxymité
bool Asserv::filtre_IsInsideTable(int dist_detect_mm, int lateral_pos_sensor_mm, std::string desc)
{

	logger().debug() << "Asserv::filtre_IsInsideTable Surcharge à faire par config Robot!!!!!!!!" << logs::end;
	return false;

}
/*
 bool Asserv::filtre_IsInsideTableXY(int d_mm, int x_mm, int y_mm, float theta_deg, int *x_botpos, int *y_botpos)
 {
 //return true; //PATCH

 //table verticale
 int table_x = 2000;
 int table_y = 3000;
 RobotPosition p = pos_getPosition();

 //coordonnées de l'objet detecté sur la table// M_P/2
 //    *x_botpos = p.x + (d_mm * cos(p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f)));
 //    *y_botpos = p.y + (d_mm * sin(p.theta - M_PI_2 + (theta_deg * M_PI / 180.0f)));
 float a = ((theta_deg * M_PI / 180.0f) - p.theta);
 std::fmod(a, 2 * M_PI);
 if (a < -M_PI)
 a += M_PI;
 if (a > M_PI)
 a -= M_PI;

 *x_botpos = p.x + (d_mm * cos(a));
 *y_botpos = p.y + (d_mm * sin(a));

 //on filtre si c'est en dehors de la table verticale! avec 10cm de marge
 if ((*x_botpos > 100 && *x_botpos < table_x - 100) && (*y_botpos > 100 && *y_botpos < table_y - 100)) {
 logger().debug() << "INSIDE filtre_IsInsideTableXY xy_botpos=" << *x_botpos << " " << *y_botpos
 << "pos: " << p.x << " " << p.y << " p_rad:" << p.theta << " balise: " << d_mm << " " << x_mm << " "
 << y_mm << " t_deg:" << theta_deg << logs::end;
 return true;
 } else
 return false;

 }*/
/*
 //TODO doit etre surcharger par robot
 bool Asserv::filtre_IsInFront(int threshold_mm, int dist_mm, int x_mm, int y_mm, float theta_deg)
 {

 logger().debug() << "filtre_IsInFront dist_mm=" << dist_mm << " x_mm=" << x_mm << " y_mm=" << y_mm << " theta_deg="
 << theta_deg << logs::end;
 //    float x = 0.0;
 //    float y = 0.0;
 //    bool result = false;
 //    RobotPosition p = pos_getPosition();
 //    x = p.x + ((x_mm) * cos(p.theta - M_PI_2)) + (y_mm * cos(p.theta));
 //    y = p.y + ((x_mm) * sin(p.theta - M_PI_2)) + (y_mm * sin(p.theta));
 if (y_mm > 0) {
 //on garde une marge supérieure à la config des sensors qui sera plus petite
 if (dist_mm < threshold_mm) {

 //TODO à mettre en parametre suivant le robot lego/oposul et la grosseur de l'adv
 if ((x_mm < 310) && (x_mm > -310)) {
 //on exclu y a cote du robot.... TODO faire un cercle ??
 if ((y_mm > 150) || (y_mm < -150)) {
 return true;
 } else
 return false;
 } else
 return false;
 } else
 return false;
 } else
 return false;
 }
 */
/*
 //todo filtre_IsInBack
 bool Asserv::filtre_IsInBack(int threshold_mm, int dist_mm, int x_mm, int y_mm, float theta_deg)
 {
 logger().debug() << "filtre_IsInBack dist_mm=" << dist_mm << " x_mm=" << x_mm << " y_mm=" << y_mm << " theta_deg="
 << theta_deg << logs::end;
 //logger().debug() << "Asserv::filtre_IsInFront Surcharge à faire par config Robot!!!!!!!!" << logs::end;
 //    float x = 0.0;
 //    float y = 0.0;
 //    bool result = false;
 //    RobotPosition p = pos_getPosition();
 //    x = p.x + ((x_mm) * cos(p.theta - M_PI_2)) + (y_mm * cos(p.theta));
 //    y = p.y + ((x_mm) * sin(p.theta - M_PI_2)) + (y_mm * sin(p.theta));
 if (y_mm < 0) {
 // dist tjs positive
 if (dist_mm < threshold_mm) {

 //TODO à mettre en parametre suivant le robot lego/oposul et la grosseur de l'adv
 if ((x_mm < 300) && (x_mm > -300)) {
 //on exclu yTODO faire un cercle ??
 if ((y_mm > 150) || (y_mm < -150)) {
 return true;
 } else
 return false;
 } else
 return false;
 } else
 return false;
 } else
 return false;
 }
 */


void Asserv::setEmergencyStop()
{
	logger().debug() << "Asserv::setEmergencyStop() !!!!!!!!!!!" << logs::end;
	if (emergencyStop_ == true)
	{
		logger().debug() << "Asserv::setEmergencyStop() emergencyStop_ ALREADY TRUE!" << logs::end;

	} else
	{
		emergencyStop_ = true;

		if (useAsservType_ == ASSERV_EXT)
			asservdriver_->path_InterruptTrajectory();
		else if (useAsservType_ == ASSERV_INT_ESIALR) pAsservEsialR_->path_InterruptTrajectory();
	}

}

void Asserv::resetEmergencyOnTraj(std::string message)
{
	if (emergencyStop_ == false)
	{
		logger().debug() << "Asserv::resetEmergencyOnTraj() emergencyStop_ IS NOT TRUE!" << logs::end;
		//return;
	}
	logger().debug() << "=====   resetEmergencyOnTraj message = " << message << logs::end;
	emergencyStop_ = false;
	if (useAsservType_ == ASSERV_EXT)
		asservdriver_->path_ResetEmergencyStop();
	else if (useAsservType_ == ASSERV_INT_ESIALR) pAsservEsialR_->path_ResetEmergencyStop();
}

void Asserv::update_adv()
{
	logger().info() << "update_adv tob surcharged = " << logs::end;
}

int Asserv::getLowSpeedvalue()
{
	return lowSpeedvalue_;
}

void Asserv::setLowSpeedvalue(int value)
{
	lowSpeedvalue_ = value;
}

int Asserv::getMaxSpeedDistValue()
{
	return maxSpeedDistValue_;
}

void Asserv::setMaxSpeedDistValue(int value)
{
	maxSpeedDistValue_ = value;
}

void Asserv::warnFrontDetectionOnTraj(int frontlevel, float x_adv_detect_mm, float y_adv_detect_mm)
{
//	logger().error() << "temp_forceRotation_ = " << temp_forceRotation_ << " temp_ignoreFrontDetection_="
//				<< temp_ignoreFrontDetection_ << logs::end;
	if (temp_forceRotation_) return;
	if (temp_ignoreFrontDetection_) return; //TODO back detection

//	logger().info() << "warnFrontDetectionOnTraj frontlevel = " << frontlevel << logs::end;
//	logger().error() << "temp_forceRotation_ = " << temp_forceRotation_ << " temp_ignoreFrontDetection_="
//			<< temp_ignoreFrontDetection_ << logs::end;

	if (frontlevel == 2)
	{
		setMaxSpeed(false);
		return;
	}
	//3  => on baisse la vitesse
	if (frontlevel == 3)
	{
		//setLowSpeedBackward(true, getLowSpeedvalue());
		setMaxSpeed(true, getMaxSpeedDistValue());
		return;
	}

	if (frontlevel == 4)
	{

		//On ne fait un HALT que si l'asserv n'est pas a IDLE
		ROBOTPOSITION p = pos_getPosition();

		logger().debug() << __FUNCTION__ << " HAAAAAAAAAAAALT p.asservStatus = " << p.asservStatus << logs::end;
		//if (true)
//		if (p.asservStatus == 1) // && p.queueSize > 0)
//		{
			setEmergencyStop();
//		}
	}

	/*
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
	 << logs::end;*/
}

void Asserv::warnBackDetectionOnTraj(int backlevel, float x_adv_detect_mm, float y_adv_detect_mm) //x positif devant le robot, y positif le coté gauche
{
	if (temp_forceRotation_) return;
	if (temp_ignoreBackDetection_) return;
//	logger().info() << "asserv warnBackDetectionOnTraj backlevel = " << backlevel << logs::end;
//		logger().error() << "temp_forceRotation_ = " << temp_forceRotation_ << " temp_ignoreBackDetection_="
//				<< temp_ignoreBackDetection_ << logs::end;



	if (backlevel == -2)
	{
		setMaxSpeed(false);
		return;
	}
	//3  => on baisse la vitesse
	if (backlevel == -3)
	{
		//setLowSpeedBackward(true, getLowSpeedvalue());
		setMaxSpeed(true, getMaxSpeedDistValue());
		return;
	}

	if (backlevel == -4)
	{
		//On ne fait un HALT que si l'asserv n'est pas a IDLE
		ROBOTPOSITION p = pos_getPosition();

		//logger().debug() << __FUNCTION__ << " HAAAAAAAAAAAALT p.asservStatus = " << p.asservStatus << logs::end;
		//if (true)
		if (p.asservStatus == 1) // && p.queueSize > 0)
		{
			setEmergencyStop();
		}
	}
	/*
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
	 logger().debug() << "warnBackCollisionOnTraj x_adv = " << x_adv << " y_adv = " << y_adv
	 << "         x_adv_detect_mm = " << x_adv_detect_mm << " y_adv_detect_mm = " << y_adv_detect_mm
	 << logs::end;*/
}

TRAJ_STATE Asserv::gotoChain(float xMM, float yMM)
{
	float x_match = changeMatchX(xMM);
	//temp_ignoreRearCollision_ = true;
	TRAJ_STATE ts;
	if (useAsservType_ == ASSERV_EXT)
		ts = asservdriver_->motion_GotoChain(x_match, yMM);
	else if (useAsservType_ == ASSERV_INT_ESIALR)
		ts = pAsservEsialR_->motion_GotoChain(x_match, yMM);
	else
		ts = TRAJ_ERROR;
	//temp_ignoreRearCollision_ = false;
	return ts;
}

TRAJ_STATE Asserv::gotoXY(float xMM, float yMM)
{
	float x_match = changeMatchX(xMM);
//temp_ignoreRearCollision_ = true;

	TRAJ_STATE ts;
	if (useAsservType_ == ASSERV_EXT)
		ts = asservdriver_->motion_Goto(x_match, yMM);
	else if (useAsservType_ == ASSERV_INT_ESIALR)
		ts = pAsservEsialR_->motion_Goto(x_match, yMM);

	else
		ts = TRAJ_ERROR;
//temp_ignoreRearCollision_ = false;

	return ts;
}

TRAJ_STATE Asserv::gotoReverse(float xMM, float yMM)
{
	float x_match = changeMatchX(xMM);
	//temp_ignoreFrontCollision_ = true;
	TRAJ_STATE ts;
	if (useAsservType_ == ASSERV_EXT)
		ts = asservdriver_->motion_GotoReverse(x_match, yMM);
	else if (useAsservType_ == ASSERV_INT_ESIALR)
		ts = pAsservEsialR_->motion_GotoReverse(x_match, yMM);

	else
		ts = TRAJ_ERROR;
	//temp_ignoreFrontCollision_ = false;
	return ts;
}

TRAJ_STATE Asserv::gotoReverseChain(float xMM, float yMM)
{
	float x_match = changeMatchX(xMM);
	//temp_ignoreFrontCollision_ = true;
	TRAJ_STATE ts;
	if (useAsservType_ == ASSERV_EXT)
		ts = asservdriver_->motion_GotoReverseChain(x_match, yMM);
	else if (useAsservType_ == ASSERV_INT_ESIALR)
		ts = pAsservEsialR_->motion_GotoReverseChain(x_match, yMM);

	else
		ts = TRAJ_ERROR;
	//temp_ignoreFrontCollision_ = false;
	return ts;
}

TRAJ_STATE Asserv::doLine(float dist_mm) // if distance <0, move backward
{

	if (dist_mm > 0)
	{
		temp_ignoreBackDetection_ = true;
	} else
	{
		temp_ignoreFrontDetection_ = true;
	}

	TRAJ_STATE ts;

	if (useAsservType_ == ASSERV_EXT)
		ts = asservdriver_->motion_DoLine(dist_mm);
	else if (useAsservType_ == ASSERV_INT_ESIALR)
		ts = pAsservEsialR_->motion_DoLine(dist_mm);
	else
		ts = TRAJ_ERROR;

	if (dist_mm > 0)
	{
		temp_ignoreBackDetection_ = false;
	} else
	{
		temp_ignoreFrontDetection_ = false;
	}

	return ts;
}

TRAJ_STATE Asserv::doRelativeRotateDeg(float degreesRelative, bool rotate_ignoring_opponent)
{

	TRAJ_STATE ts;
	//float radians = (degreesRelative * M_PI) / 180.0f;
	float rad = degToRad(degreesRelative);
	doRelativeRotateRad(rad, rotate_ignoring_opponent);

	return ts;
}

TRAJ_STATE Asserv::doRelativeRotateRad(float radiansRelative, bool rotate_ignoring_opponent)
{
	TRAJ_STATE ts;
	temp_forceRotation_ = rotate_ignoring_opponent;
	if (useAsservType_ == ASSERV_EXT)
		ts = asservdriver_->motion_DoRotate(radiansRelative);
	else if (useAsservType_ == ASSERV_INT_ESIALR)
		ts = pAsservEsialR_->motion_DoRotate(radiansRelative);
	else
		ts = TRAJ_ERROR;

	temp_forceRotation_ = false;

	return ts;
}

//prend automatiquement un angle dans un sens ou dans l'autre suivant la couleur de match
TRAJ_STATE Asserv::doRelativeRotateByMatchColor(float thetaInDegreeRelative, bool rotate_ignoring_opponent)
{
	if (matchColorPosition_ != 0)
	{
		return doRelativeRotateDeg(-thetaInDegreeRelative, rotate_ignoring_opponent); //couleur de match secondaire
	} else
		return doRelativeRotateDeg(thetaInDegreeRelative, rotate_ignoring_opponent); //couleur de match primaire
}

TRAJ_STATE Asserv::doFaceTo(float xMM, float yMM, bool back_face)
{
//    logger().error() << "1.============ doFaceTo temp_forceRotation_ = true;"  << logs::end;
	temp_forceRotation_ = true; //attention on ne prend pas en compte l'adversaire

	float x_match = changeMatchX(xMM);
//logger().error() << "doFaceTo xMM=" << xMM << " yMM=" << yMM << logs::end;

	TRAJ_STATE ts;

	if (useAsservType_ == ASSERV_EXT)
		ts = asservdriver_->motion_DoFace(x_match, yMM, back_face);
	else if (useAsservType_ == ASSERV_INT_ESIALR)
		ts = pAsservEsialR_->motion_DoFace(x_match, yMM, back_face);
	else
		ts = TRAJ_ERROR;

	temp_forceRotation_ = false;
//    logger().error() << "2.============ doFaceTo temp_forceRotation_ = true;"  << logs::end;
	return ts;
}

//absolute motion (depends on current position of the robot, thinking in the first color of match) [-179;0;+180] of the field
TRAJ_STATE Asserv::doAbsoluteRotateTo(float thetaInDegreeAbsolute, bool rotate_ignoring_opponent)
{
	float rad = changeMatchAngleRad(degToRad(thetaInDegreeAbsolute)) - pos_getTheta();

	rad = WrapAngle2PI(rad);

	logger().debug() << "==== doRotateTo degrees=" << radToDeg(rad) << " thetaInDegreeAbsolute="
			<< thetaInDegreeAbsolute << logs::end;
	TRAJ_STATE ts = doRelativeRotateRad(rad, rotate_ignoring_opponent);

	return ts;
}

//Move forward jusquà une position donnée en fonction de la position du robot (getPos)
TRAJ_STATE Asserv::doMoveForwardTo(float xMM, float yMM, bool rotate_ignoring_opponent, float adjustment_mm)
{
	float dx = changeMatchX(xMM) - pos_getX_mm();
	float dy = yMM - pos_getY_mm();
	if (std::abs(dx) < 5.0 && std::abs(dy) < 5.0)
	{
		logger().info() << "___ TRAJ_FINISHED __doMoveForwardTo (std::abs(dx) < 5.0 && std::abs(dy) < 5.0)"
				<< logs::end;
		return TRAJ_FINISHED;
	}
	float aRadian = atan2(dy, dx);

	aRadian = WrapAngle2PI(aRadian);

	logger().debug() << "doMoveForwardTo doRotateTo degrees=" << (aRadian * 180.0f) / M_PI << " dx=" << dx << " dy="
			<< dy << "  (aRadian * 180.0f) / M_PI)= " << (aRadian * 180.0f) / M_PI << " get="
			<< radToDeg(changeMatchAngleRad(aRadian)) << " xMM=" << xMM << " yMM=" << yMM << " getX=" << pos_getX_mm()
			<< " getY=" << pos_getY_mm() << logs::end;

	TRAJ_STATE ts = TRAJ_IDLE;
//	int count_rotation_ignored = 0;

	temp_forceRotation_ = rotate_ignoring_opponent;
	//temp_forceRotation_ = false;

	ts = doAbsoluteRotateTo(radToDeg(changeMatchAngleRad(aRadian)), rotate_ignoring_opponent);
	if (ts != TRAJ_FINISHED)
	{
		if (!rotate_ignoring_opponent)
			return ts;
		else
		{
			if (ts >= TRAJ_INTERRUPTED) //||ts == TRAJ_COLLISION || ts == TRAJ_COLLISION_REAR)
			{
				//on resette
				resetEmergencyOnTraj("doMoveForwardTo rotate_ignoring_opponent TRAJ_INTERRUPTED");
				//logger().error() << "doMoveForwardTo rotate_ignoring_opponent resetEmergencyOnTraj TRAJ_COLLISION" << logs::end;
				//on renvoi pour dire qu'on est en collision en  tournant
				//return ts;
				//count_rotation_ignored++;
				//if (count_rotation_ignored > 10) break;
			} else
			{
				resetEmergencyOnTraj("doMoveForwardTo rotate_ignoring_opponent TRAJ_OTHERS!!!! ");

				//logger().error() << "doMoveForwardTo ROTATION TRAJ ERROR CAS NON DESIRE  car on ignore l'adversaire!!!!!!!   => on passe au doline !!! ts=" << ts << logs::end;
				//return ts;
			}
		}
	}
	temp_forceRotation_ = false;

	float dist = sqrt(dx * dx + dy * dy);
	logger().debug() << " __doMoveForwardTo dist sqrt(dx * dx + dy * dy)=" << dist << logs::end;

	return doLine(dist + adjustment_mm);

}
TRAJ_STATE Asserv::doMoveBackwardTo(float xMM, float yMM, bool rotate_ignoring_opponent)
{
	xMM = changeMatchX(xMM);

	float dx = xMM - pos_getX_mm();
	float dy = yMM - pos_getY_mm();
	if (std::abs(dx) < 5.0 && std::abs(dy) < 5.0)
	{ //Augmenter les valeurs??? par rapport à l'asserv fenetre d'arrivée
		return TRAJ_FINISHED;
	}
	float aRadian = M_PI + atan2(dy, dx);
	aRadian = WrapAngle2PI(aRadian);

	temp_forceRotation_ = rotate_ignoring_opponent;

	TRAJ_STATE ts = doAbsoluteRotateTo(radToDeg(changeMatchAngleRad(aRadian)));
	if (ts != TRAJ_FINISHED)
	{
		if (!rotate_ignoring_opponent)
			return ts;
		else
		{
			resetEmergencyOnTraj("doMoveBackwardTo rotate_ignored");
			//logger().error() << " __on passe au doline !!!"  << logs::end;
		}
	}

	temp_forceRotation_ = false;

	float dist = sqrt(dx * dx + dy * dy);
	return doLine(-dist);
}

//deprecated ?
TRAJ_STATE Asserv::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree, bool rotate_ignore_opponent)
{
	logger().error() << "doMoveForwardAndRotateTo deprecated !!!" << logs::end;
	TRAJ_STATE ts;
	ts = doMoveForwardTo(xMM, yMM, rotate_ignore_opponent);
	if (ts != TRAJ_FINISHED) return ts;

	ts = doAbsoluteRotateTo(thetaInDegree);
	return ts;
}
//deprecated ?
TRAJ_STATE Asserv::doMoveBackwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
	logger().error() << "doMoveBackwardAndRotateTo deprecated !!!" << logs::end;
	TRAJ_STATE ts;
	ts = doMoveBackwardTo(xMM, yMM);
	if (ts != TRAJ_FINISHED) return ts;
	ts = doAbsoluteRotateTo(thetaInDegree);
	return ts;
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
	if (delta < 0)
	{
		return std::make_tuple(0, -1, -1);
	}
	if (delta == 0)
	{
		coord_x = (-1.0 * B / (2.0 * A));
		coord_y = b - (a * coord_x);

		return std::make_tuple(1, coord_x, coord_y);
	}
	if (delta > 0)
	{
		float x1 = (((-1.0 * B) + std::sqrt(delta)) / (2.0 * A));
		float y1 = b - (a * x1); //b- ax
		float x2 = ((-1.0 * B - std::sqrt(delta)) / (2.0 * A));
		float y2 = b - (a * x2); //b- ax

		logger().debug() << " solutions : x1=" << x1 << " y1=" << y1 << "   x2=" << x2 << " y2=" << y2 << logs::end;
		//filtrage de la table et de la largeur du robot !!
		if (y1 < robot_size_l_mm && y2 > robot_size_l_mm)
		{
			coord_x = x2;
			coord_y = y2;
			return std::make_tuple(2, coord_x, coord_y);
		} else if (y2 < robot_size_l_mm && y1 > robot_size_l_mm)
		{
			coord_x = x1;
			coord_y = y1;
			return std::make_tuple(3, coord_x, coord_y);
		} else
		{

			//double solution impossible à déterminer
			return std::make_tuple(-2, -1, -1);
		}
	} else
		return std::make_tuple(-1, -1, -1);

}

int Asserv::adjustRealPosition(float pos_x_start_mm, float pos_y_start_mm, ROBOTPOSITION p, float delta_jx_mm,
		float delta_ky_mm, float mesure_mm, float robot_size_l_mm)
{

	float pos_x_start_mm_conv = changeMatchX(pos_x_start_mm);
	logger().debug() << "adjustRealPosition : pos_x_start_mm=" << pos_x_start_mm_conv << " pos_y_start_mm="
			<< pos_y_start_mm << " p.x=" << p.x << " p.y=" << p.y << " p.theta=" << p.theta << " degrees="
			<< p.theta * 180 / M_PI << " delta_jx_mm=" << delta_jx_mm << " delta_ky_mm=" << delta_ky_mm << " mesure_mm="
			<< mesure_mm << logs::end;

	float dist_real_mm = std::sqrt(
			(((p.x) - pos_x_start_mm_conv) * ((p.x) - pos_x_start_mm_conv))
					+ (((p.y) - pos_y_start_mm) * ((p.y) - pos_y_start_mm)));

	float dist_x_when_mesuring_mm = std::abs(p.x - pos_x_start_mm_conv);
	float position_rel_theta_when_mesuring_rad = changeMatchAngleRad(p.theta); //on cherche juste l'angle relatif qu'on soit en couleur A ou B

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
			changeMatchX(pos_x_start_mm, dist_xDCp_mm), 0.0, BCprim_mm, robot_size_l_mm);

	float err = std::get<0>(r);

	logger().debug() << "alphap_rad=" << alphap_rad << " deg=" << alphap_rad * 180 / M_PI << " gamma_rad=" << gamma_rad
			<< " deg=" << gamma_rad * 180 / M_PI << " angle_rad=" << angle_rad << " deg=" << angle_rad * 180 / M_PI
			<< " position_rel_theta_when_mesuring_rad=" << position_rel_theta_when_mesuring_rad
			<< " dist_x_when_mesuring_mm=" << dist_x_when_mesuring_mm << " dist_real_mm=" << dist_real_mm << " BCprim="
			<< BCprim_mm << " DCprim=" << DCprim_mm << " dist_xDCprim_mm=" << dist_xDCp_mm << " err=" << err
			<< " new_x_mm=" << std::get<1>(r) << " new_y_mm=" << std::get<2>(r) << logs::end;

	if (err <= 0) return err;

	float new_x_mm = std::get<1>(r);
	float new_y_mm = std::get<2>(r);

//calcul de l'angle de correction
//ajouter la difference entre ancien alphap et le nouveau calculé à la position Theta
	float new_alphap = std::acos(std::abs((new_x_mm - pos_x_start_mm_conv)) / dist_real_mm);

//float new_teta = getRelativeAngle((position_rel_theta_when_mesuring_rad + (alphap_rad - new_alphap)) * 180.0 / M_PI) * M_PI / 180.0;

//TODO essaie de correction de l'angle
	float new_teta = changeMatchAngleRad((position_rel_theta_when_mesuring_rad - (alphap_rad - new_alphap)));

	logger().debug() << "new pos : x=" << new_x_mm << " y=" << new_y_mm << " a=" << new_teta << " degrees="
			<< new_teta * 180 / M_PI << logs::end;
//set de la nouvelle position et angle
	setPositionReal(new_x_mm, new_y_mm, new_teta);

	return 1; // il y a un resultat viable.
}

bool Asserv::calculateDriftRightSideAndSetPos(float d2_theo_bordure_mm, float d2b_bordure_mm, float x_depart_mm,
		float y_depart_mm)
{
	logger().error() << "calculate : " << " d2_theo_bordure_mm= " << d2_theo_bordure_mm << " d2b_bordure_mm= "
			<< d2b_bordure_mm << " x_depart_mm= " << x_depart_mm << " y_depart_mm= " << y_depart_mm << logs::end;

	if (abs(d2b_bordure_mm - d2_theo_bordure_mm) >= 5)
	{

//Partie théorique basé sur la position d'arrivée (que croit le robot)
		ROBOTPOSITION p = pos_getPosition();
		//tan teta = d1/l
		float dx = (p.x) - x_depart_mm;
		float dy = (p.y) - y_depart_mm;
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
				<< teta_error * 180.0 / M_PI << " p.x=" << p.x << " l_theo_mm =" << (l_theo_mm) << logs::end;

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
			<< d2b_bordure_mm << " x_depart_mm= " << changeMatchX(x_depart_mm) << " y_depart_mm= " << y_depart_mm
			<< logs::end;

	x_depart_mm = changeMatchX(x_depart_mm);

	if (abs(d2b_bordure_mm - d2_theo_bordure_mm) >= 5)
	{

//Partie théorique basé sur la position d'arrivée (que croit le robot)
		ROBOTPOSITION p = pos_getPosition();
		//tan teta = d1/l
		//float dx = (p.x * 1000.0) - getRelativeX(x_depart_mm);
		float dx = (p.x) - x_depart_mm;
		float dy = (p.y) - y_depart_mm;
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
				<< teta_error * 180.0 / M_PI << " p.x=" << p.x << " l_theo_mm=" << (l_theo_mm) << logs::end;

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

		setPositionAndColor(changeMatchX(new_x), new_y, radToDeg(changeMatchAngleRad(new_teta)), matchColorPosition_);

		return true;
	} else
		return false;

}

void Asserv::doRunPivotLeft(int powerL, int powerR, int timems) //tourner en pivot sur la roue gauche
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		pAsservEsialR_->motion_FreeMotion();
		pAsservEsialR_->motion_ActivateReguAngle(false);
		pAsservEsialR_->motion_ActivateReguDist(false);
		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();

		runMotorRight(powerR, timems);
		runMotorLeft(powerL, timems);
		utils::sleep_for_micros(timems * 1000);

		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();
		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();
		pAsservEsialR_->motion_ActivateReguAngle(true);
		pAsservEsialR_->motion_ActivateReguDist(true);
		pAsservEsialR_->motion_AssistedHandling();
		resetEmergencyOnTraj("doRunPivotLeft");
	} else if (useAsservType_ == ASSERV_EXT)
	{

		//TODO
	}
}

void Asserv::doRunPivotRight(int powerL, int powerR, int timems) //tourner en pivot sur la roue gauche
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		pAsservEsialR_->motion_FreeMotion();
		pAsservEsialR_->motion_ActivateReguAngle(false);
		pAsservEsialR_->motion_ActivateReguDist(false);
		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();

		runMotorRight(powerR, timems);
		runMotorLeft(powerL, timems);
		utils::sleep_for_micros(timems * 1000);

		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();
		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();
		pAsservEsialR_->motion_ActivateReguAngle(true);
		pAsservEsialR_->motion_ActivateReguDist(true);
		pAsservEsialR_->motion_AssistedHandling();
		resetEmergencyOnTraj("doRunPivotRight");
	} else if (useAsservType_ == ASSERV_EXT)
	{

		//TODO
	}
}

TRAJ_STATE Asserv::doCalage2(int distmm, int percent)
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		if (distmm > 0)
			pAsservEsialR_->motion_setLowSpeedForward(true, percent);
		else if (distmm < 0) pAsservEsialR_->motion_setLowSpeedBackward(true, percent);

		pAsservEsialR_->motion_ActivateReguAngle(true);
		pAsservEsialR_->motion_ActivateReguDist(true);
		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();

		pAsservEsialR_->motion_AssistedHandling();
		TRAJ_STATE ts = TRAJ_IDLE;
		while (ts == TRAJ_IDLE)
		{
			ts = pAsservEsialR_->motion_DoDirectLine(distmm); //sans asservissement L/R
			std::this_thread::yield();
		}

		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();
		pAsservEsialR_->motion_ActivateReguAngle(false);
		pAsservEsialR_->motion_ActivateReguDist(true);
		resetEmergencyOnTraj("doCalage phase 1");

		ts = pAsservEsialR_->motion_DoDirectLine(distmm); //sans asservissement L/R

		//pAsservEsialR_->path_CancelTrajectory();
		pAsservEsialR_->motion_setLowSpeedForward(false);
		pAsservEsialR_->motion_setLowSpeedBackward(false);

		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();
		pAsservEsialR_->motion_ActivateReguAngle(true);
		pAsservEsialR_->motion_ActivateReguDist(true);
		resetEmergencyOnTraj("doCalage phase 2");

		return ts;
	} else if (useAsservType_ == ASSERV_EXT)
	{

		//TODO A finir
	}
	return TRAJ_ERROR;
}

TRAJ_STATE Asserv::doCalage(int distmm, int percent)
{

	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		if (distmm > 0)
			pAsservEsialR_->motion_setLowSpeedForward(true, percent);
		else if (distmm < 0) pAsservEsialR_->motion_setLowSpeedBackward(true, percent);

		pAsservEsialR_->motion_ActivateReguAngle(false);
		pAsservEsialR_->motion_ActivateReguDist(true);
		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();

		pAsservEsialR_->motion_AssistedHandling();
		//TRAJ_STATE ts = TRAJ_OK;
		TRAJ_STATE ts = pAsservEsialR_->motion_DoDirectLine(distmm); //sans asservissement L/R

		//pAsservEsialR_->path_CancelTrajectory();
		pAsservEsialR_->motion_setLowSpeedForward(false);
		pAsservEsialR_->motion_setLowSpeedBackward(false);

		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();
		pAsservEsialR_->motion_ActivateReguAngle(true);
		pAsservEsialR_->motion_ActivateReguDist(true);
		resetEmergencyOnTraj("doCalage");

		return ts;
	} else if (useAsservType_ == ASSERV_EXT)
	{

		if (distmm > 0)
			asservdriver_->motion_setLowSpeedForward(true, percent);
		else if (distmm < 0) asservdriver_->motion_setLowSpeedBackward(true, percent);

//        asservdriver_->motion_ActivateReguAngle(false);
//        asservdriver_->motion_ActivateReguDist(true);
		asservdriver_->motion_AssistedHandling();

		TRAJ_STATE ts = asservdriver_->motion_DoLine(distmm); //sans asservissement L/R

		asservdriver_->motion_setLowSpeedForward(false);
		asservdriver_->motion_setLowSpeedBackward(false);

		asservdriver_->motion_ActivateReguAngle(true);
		asservdriver_->motion_ActivateReguDist(true);
		resetEmergencyOnTraj("doCalage");

		return ts;
	} else
		return TRAJ_ERROR;
}

TRAJ_STATE Asserv::doCalageNew(float dist_mm, int percent, float timeout_ms) // if distance <0, move backward
{
	if (useAsservType_ == ASSERV_INT_ESIALR)
	{
		if (dist_mm > 0)
			pAsservEsialR_->motion_setLowSpeedForward(true, percent);
		else if (dist_mm < 0) pAsservEsialR_->motion_setLowSpeedBackward(true, percent);

		pAsservEsialR_->motion_ActivateReguAngle(false);
		pAsservEsialR_->motion_ActivateReguDist(true);
		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();

		pAsservEsialR_->motion_AssistedHandling();

		//TODO implementer le timeout ??

		TRAJ_STATE ts = pAsservEsialR_->motion_DoDirectLine(dist_mm); //sans asservissement L/R

		//pAsservEsialR_->path_CancelTrajectory();
		pAsservEsialR_->motion_setLowSpeedForward(false);
		pAsservEsialR_->motion_setLowSpeedBackward(false);

		pAsservEsialR_->motion_ResetReguAngle();
		pAsservEsialR_->motion_ResetReguDist();
		pAsservEsialR_->motion_ActivateReguAngle(true);
		pAsservEsialR_->motion_ActivateReguDist(true);
		resetEmergencyOnTraj("doCalage");

		return ts;
	} else if (useAsservType_ == ASSERV_EXT)
	{

		if (dist_mm > 0)
			asservdriver_->motion_setLowSpeedForward(true, percent);
		else if (dist_mm < 0) asservdriver_->motion_setLowSpeedBackward(true, percent);

        asservdriver_->motion_ActivateReguAngle(false);
        asservdriver_->motion_ActivateReguDist(true);

        //TODO implementer le timeout ??
		TRAJ_STATE ts = asservdriver_->motion_DoLine(dist_mm);

		asservdriver_->motion_setLowSpeedForward(false);
		asservdriver_->motion_setLowSpeedBackward(false);

		asservdriver_->motion_ActivateReguAngle(true);
		asservdriver_->motion_ActivateReguDist(true);
		//resetEmergencyOnTraj("doCalage");

		return ts;
	} else
		return TRAJ_ERROR;
}



//FONCTION ASSERV DE BASE

void Asserv::resetEncoders()
{
	asservdriver_->resetEncoders();
}
void Asserv::getDeltaEncodersCounts(int *deltaCountR, int *deltaCountL)
{
	asservdriver_->getDeltaCountsExternal(deltaCountR, deltaCountL);
}
void Asserv::getEncodersCounts(int *countR, int *countL)
{
	asservdriver_->getCountsExternal(countR, countL);
}
void Asserv::runMotorLeft(int power, int timems)
{
	asservdriver_->setMotorLeftPower(power, timems);
}
void Asserv::runMotorRight(int power, int timems)
{
	asservdriver_->setMotorRightPower(power, timems);
}
void Asserv::stopMotors()
{
	asservdriver_->stopMotors();
}
