#ifndef OPOS6UL_ACTIONSEXTENDED_HPP_
#define OPOS6UL_ACTIONSEXTENDED_HPP_

#include <string>
#include <iostream>

#include "../Common/Action/Actions.hpp"
#include "../Common/Action/ButtonBar.hpp"
#include "../Common/Action/LcdShield.hpp"
#include "../Common/Action/LedBar.hpp"
#include "../Common/Action/Sensors.hpp"
#include "../Common/Action/ServoObjectsSystem.hpp"
#include "../Common/Action/Tirette.hpp"
#include "../Common/Interface.Driver/AServoDriver.hpp"
#include "../Common/Action/ServoUsingMotor.hpp"

class OPOS6UL_ActionsExtended: public Actions {
private:

	static inline const logs::Logger& logger()
	{
		static const logs::Logger &instance = logs::LoggerFactory::logger("OPOS6UL_ActionsExtended");
		return instance;
	}

	/*!
	 * \brief LED Bar.
	 */
	LedBar ledbar_;

	/*!
	 * \brief Button Bar.
	 */
	ButtonBar buttonbar_;

	/*!
	 * \brief LcdShield 2x16 characters.
	 */
	LcdShield lcd2x16_;

	/*!
	 * \brief la tirette du robot.
	 */
	Tirette tirette_;

	/*!
	 * \brief capteurs IR/US.
	 */
	Sensors sensors_;

	/*!
	 * \brief objets avec servomotors ax12 et std.
	 */
	ServoObjectsSystem servos_;

	//ServoUsingMotor lanceurCerises_;

public:

	/*!
	 * \brief Enumération des libellés des servos associés au numéro de servo
	 * port-servo port1-4=1000-4000 servo=1-255 => 1000 à 4255
	 * [num port] * 1000 + [num servo]
	 */
	enum ServoAx12Label {
		AX12_SERVO_BRAS_D = 1 * 1000 + 5, //AX12
		AX12_SERVO_BRAS_G = 1000 + 7, //AX12

		AX12_SERVO_FINGER_R = 1000 + 51,
		AX12_SERVO_WRIST_R = 1000 + 63,
		AX12_SERVO_ELBOW_R = 1000 + 182,
		AX12_SERVO_SHOULDER_R = 1000 + 8,

		AX12_SERVO_FINGER_L = 1000 + 3,
		AX12_SERVO_WRIST_L = 1000 + 61,
		AX12_SERVO_ELBOW_L = 1000 + 62,
		AX12_SERVO_SHOULDER_L = 1000 + 52,

		AX12_enumTypeEnd
	};

	/*!
	 * \brief Enumération des libellés des servos STD associés au numéro de servo
	 *  port=10000
	 */
	enum ServoStdLabel {

		STD_SERVO_3 = 10000 * 5,

		STD_SERVO_4 = 10000 * 4,

		SERVO_enumTypeEnd
	};

	OPOS6UL_ActionsExtended(std::string botId, Robot *robot);

	~OPOS6UL_ActionsExtended()
	{
	}

	/*!
	 * \brief Cette methode retourne l'objet ledbar.
	 * \return ledbar_.
	 */
	LedBar& ledBar()
	{
		return ledbar_;
	}

	ButtonBar& buttonBar()
	{
		return buttonbar_;
	}

	/*!
	 * \brief Cette methode retourne l'objet LcdShield.
	 * \return lcd2x16_.
	 */
	LcdShield& lcd2x16()
	{
		return lcd2x16_;
	}

	/*!
	 * \brief Cette methode retourne l'objet tirette.
	 * \return tirette_.
	 */
	Tirette& tirette()
	{
		return tirette_;
	}

	/*!
	 * \brief Cette methode retourne l'objet lanceur.
	 * \return tirette_.
	 */
//    ServoUsingMotor& lanceur()
//    {
//        return lanceurCerises_;
//    }
	/*!
	 * \brief Cette methode retourne l'objet sensors.
	 * \return sensors_.
	 */
	Sensors& sensors()
	{
		return sensors_;
	}

	/*!
	 * \brief Cette methode retourne l'objet servos.
	 * \return servos_.
	 */
	ServoObjectsSystem& servos()
	{
		return servos_;
	}

	void stopExtra()
	{

		sensors_.stopTimerSensors(); //TODO rename stop
		ledbar_.stop();

		ledbar_.resetAll();
		lcd2x16_.reset();

		releaseAll();

	}

	//--------------------------------------------------------------
	//Actions 2023
	//--------------------------------------------------------------
	void releaseAll()
	{
		//logger().error() << "releaseAll()" << logs::end;
		servos().release(AX12_SERVO_BRAS_D);
		servos().release(AX12_SERVO_BRAS_G);

		servos().release(AX12_SERVO_ELBOW_L);
		servos().release(AX12_SERVO_ELBOW_R);
		servos().release(AX12_SERVO_FINGER_L);
		servos().release(AX12_SERVO_FINGER_R);
		servos().release(AX12_SERVO_SHOULDER_L);
		servos().release(AX12_SERVO_SHOULDER_R);
		servos().release(AX12_SERVO_WRIST_L);
		servos().release(AX12_SERVO_WRIST_R);

		//        for (int fooInt = 0; fooInt != AX12_enumTypeEnd; fooInt++) {
		//            ServoAx12Label foo = static_cast<ServoAx12Label>(fooInt);
		//            //servosAx12().setPosition(foo, 0);
		//            servosAx12().release(foo);
		//        }

		//         for (int fooInt = 0; fooInt != SERVO_enumTypeEnd; fooInt++) {
		//         ServoStdLabel foo = static_cast<ServoStdLabel>(fooInt);
		//         //servosStd().setPosition(foo, 0);
		//         servosStd().release(foo);
		//         }

		//         for (int fooInt = 0; fooInt <= SERVO_enumTypeEnd; fooInt++) {
		//          ServoStdLabel foo = static_cast<ServoStd Label>(fooInt);
		//          //servosStd().setPosition(foo, 0);
		//          servosStd().release(foo);
		//          }
	}
	void ax12_open_L(int time_eta_ms = 400, int keep_ms = 0)
	{
		servos().move_1_servo(time_eta_ms, AX12_SERVO_FINGER_L, 790, keep_ms, 1023, 1023);
	}
	void ax12_open_R(int time_eta_ms = 1000, int keep_ms = 0)
	{
		//3059756| O_ServoObjectsTest INFO Rfinger (1051)= 450
		servos().move_1_servo(time_eta_ms, AX12_SERVO_FINGER_R, 450, keep_ms, 1023);
	}
	void ax12_open_LR(int time_eta_ms = 400, int keep_ms = 0)
	{

	}
	void ax12_close_LR(int time_eta_ms = 400, int keep_ms = 0)
	{

	}
	void ax12_close_R(int time_eta_ms = 1000, int keep_ms = 0)
	{
		//max 60357053| O_ServoObjectsTest INFO Rfinger (1051)= 35
		servos().move_1_servo(time_eta_ms, AX12_SERVO_FINGER_R, 35, keep_ms, 300);
	}
	void ax12_close_L(int time_eta_ms = 400, int keep_ms = 0)
	{
		servos().move_1_servo(time_eta_ms, AX12_SERVO_FINGER_L, 210, keep_ms, 400, 400);
	}
	void ax12_repli(int time_eta_ms = 1000, int keep_ms = 0)
	{
		//TODO
		servos().move_1_servo(300, AX12_SERVO_SHOULDER_R, 499, 0, 1023);
		servos().move_1_servo(300, AX12_SERVO_SHOULDER_L, 504, 0, 1023);

		servos().move_2_servos(time_eta_ms, AX12_SERVO_ELBOW_R, 782, AX12_SERVO_WRIST_R, 484, 0, 1023, 1023);
		servos().move_2_servos(time_eta_ms, AX12_SERVO_ELBOW_L, 232, AX12_SERVO_WRIST_L, 533, keep_ms, 1023, 1023);
	}
	void ax12_init()
	{
//		ax12_init_R();
//		ax12_init_L();
		ax12_init_LR();
		/*
		 *
		 2374911| O_ServoObjectsTest INFO Lshouldr(1052)= 501
		 2385836| O_ServoObjectsTest INFO Lelbow  (1062)= 232
		 2396747| O_ServoObjectsTest INFO Lwrist  (1061)= 534
		 2407649| O_ServoObjectsTest INFO Lfinger (1003)= 541
		 2407677| O_ServoObjectsTest INFO
		 2418542| O_ServoObjectsTest INFO Rshouldr(1008)= 518
		 2429419| O_ServoObjectsTest INFO Relbow  (1182)= 786
		 2440348| O_ServoObjectsTest INFO Rwrist  (1063)= 476
		 2451285| O_ServoObjectsTest INFO Rfinger (1051)= 1010
		 */
		/*
		 ax12_bras_droit();
		 ax12_bras_gauche(-1);
		 ax12_bras_droit_init();
		 ax12_bras_gauche_init(-1);

		 aspiration_closed_init();

		 moustache_G_take_balls(-1);
		 moustache_D_init_high(-1);
		 moustache_G_init_medium_below_D(-1);
		 funny_init(-1);*/

	}

	void ax12_init_R(int time_eta_ms = 2000, int keep_ms = -1)
	{
		/*
		 3025329| O_ServoObjectsTest INFO Rshouldr(1008)= 499
		 3036215| O_ServoObjectsTest INFO Relbow  (1182)= 782
		 3047101| O_ServoObjectsTest INFO Rwrist  (1063)= 484*/

		servos().move_1_servo(300, AX12_SERVO_SHOULDER_R, 499, 0, 1023);
		servos().move_2_servos(time_eta_ms, AX12_SERVO_ELBOW_R, 782, AX12_SERVO_WRIST_R, 484, keep_ms, 1023, 1023);
	}

	void ax12_init_L(int time_eta_ms = 2000, int keep_ms = -1)
	{
		/*1154104| O_ServoObjectsTest INFO Lshouldr(1052)= 495
		 1165050| O_ServoObjectsTest INFO Lelbow  (1062)= 234
		 1175965| O_ServoObjectsTest INFO Lwrist  (1061)= 526*/

		servos().move_1_servo(300, AX12_SERVO_SHOULDER_L, 504, 0, 1023);
		servos().move_2_servos(time_eta_ms, AX12_SERVO_ELBOW_L, 232, AX12_SERVO_WRIST_L, 533, keep_ms, 1023, 1023);
	}

	void ax12_init_LR(int time_eta_ms = 2000, int keep_ms = -1)
	{
		servos().move_1_servo(300, AX12_SERVO_SHOULDER_R, 499, 0, 1023);
		servos().move_1_servo(300, AX12_SERVO_SHOULDER_L, 504, 0, 1023);

		servos().move_2_servos(time_eta_ms, AX12_SERVO_ELBOW_R, 782, AX12_SERVO_WRIST_R, 484, 0, 1023, 1023);
		servos().move_2_servos(time_eta_ms, AX12_SERVO_ELBOW_L, 232, AX12_SERVO_WRIST_L, 533, keep_ms, 1023, 1023);
	}
	//keep_millisec = -1 : wait moving until position
	//keep_millisec > 0 : time ms to wait then release
	//keep_millisec = 0 : continue and hold
	void ax12_up_R(int time_eta_ms = 2000, int keep_ms = -1)
	{
		/*
		 4245029| O_ServoObjectsTest INFO Rshouldr(1008)= 496
		 4255895| O_ServoObjectsTest INFO Relbow  (1182)= 498
		 4266787| O_ServoObjectsTest INFO Rwrist  (1063)= 191

		 18870613| O_ServoObjectsTest INFO Rshouldr(1008)= 478
 18881509| O_ServoObjectsTest INFO Relbow  (1182)= 503
 18892389| O_ServoObjectsTest INFO Rwrist  (1063)= 195

		 */
		//servos().deploy(AX12_SERVO_SHOULDER_R, 496, 0);
		servos().move_1_servo(300, AX12_SERVO_SHOULDER_R, 450, 0, 1023, 1023);
		servos().move_2_servos(time_eta_ms, AX12_SERVO_ELBOW_R, 503, AX12_SERVO_WRIST_R, 202, keep_ms, 1023, 1023);
	}

	void ax12_up_L(int time_eta_ms = 2000, int keep_ms = -1)
	{
		/*
		 18826131| O_ServoObjectsTest INFO Lshouldr(1052)= 518
		 18837044| O_ServoObjectsTest INFO Lelbow  (1062)= 520
		 18847945| O_ServoObjectsTest INFO Lwrist  (1061)= 832
		 */
		//servos().deploy(AX12_SERVO_SHOULDER_L, 518, 1000);
		servos().move_1_servo(300, AX12_SERVO_SHOULDER_L, 518, 0, 1023, 1023);
		servos().move_2_servos(time_eta_ms, AX12_SERVO_ELBOW_L, 520, AX12_SERVO_WRIST_L, 832, keep_ms, 1023, 1023);
	}

	void ax12_up(int time_eta_ms = 2000, int keep_ms = -1)
	{

//		servos().deploy(AX12_SERVO_SHOULDER_R, 496, 0);
//		servos().deploy(AX12_SERVO_SHOULDER_L, 518, 0);

		servos().move_1_servo(300, AX12_SERVO_SHOULDER_R, 450, 0, 1023, 1023);
		servos().move_1_servo(300, AX12_SERVO_SHOULDER_L, 518, 0, 1023, 1023);
		servos().move_4_servos(time_eta_ms, AX12_SERVO_ELBOW_R, 503, AX12_SERVO_WRIST_R, 202, AX12_SERVO_ELBOW_L, 520,
				AX12_SERVO_WRIST_L, 832, keep_ms, 1023, 1023);
	}
	void ax12_load()
	{
		int shoulderR = servos().getTorque(AX12_SERVO_SHOULDER_R);
		int shoulderL = servos().getTorque(AX12_SERVO_SHOULDER_L);
		int elbowR = servos().getTorque(AX12_SERVO_ELBOW_R);
		int elbowL = servos().getTorque(AX12_SERVO_ELBOW_L);
		int fR = servos().getTorque(AX12_SERVO_FINGER_R);
		int fL = servos().getTorque(AX12_SERVO_FINGER_L);

		logger().info() << "LOAD shoulderR " << shoulderR << " L " << shoulderL << "  elbowR " << elbowR << " L "
				<< elbowL<< "  fingerR " << fR << " L " << fL  << logs::end;
	}
	/*
	 void ax12_bras_droit_init(int keep = 0, int speed = 1023)
	 {
	 servos().setSpeed(AX12_SERVO_BRAS_D, speed);
	 servos().deploy(AX12_SERVO_BRAS_D, 815, keep);
	 }
	 void ax12_bras_droit(int keep = 0, int speed = 1023)
	 {
	 servos().setSpeed(AX12_SERVO_BRAS_D, speed);
	 servos().deploy(AX12_SERVO_BRAS_D, 480, keep);
	 }

	 void ax12_bras_gauche_init(int keep = 0, int speed = 1023)
	 {
	 servos().setSpeed(AX12_SERVO_BRAS_G, speed);
	 servos().deploy(AX12_SERVO_BRAS_G, 205, keep);
	 }
	 void ax12_bras_gauche(int keep = 0, int speed = 1023)
	 {
	 servos().setSpeed(AX12_SERVO_BRAS_G, speed);
	 servos().deploy(AX12_SERVO_BRAS_G, 512, keep);
	 }

	 void aspiration_closed_init(int keep = 0, int speed = 150)
	 {
	 servos().deployWithVelocity(AX12_SERVO_ASPIRATION, 512, keep, speed);
	 }
	 void aspiration_lacher_les_balles(int keep = -1, int speed = 200)
	 {
	 servos().setTorque(AX12_SERVO_ASPIRATION, 800);
	 servos().deployWithVelocity(AX12_SERVO_ASPIRATION, 615, keep, speed);
	 }

	 void funny_init(int keep = 0, int speed = 150)
	 {
	 servos().deployWithVelocity(AX12_SERVO_FUNNY, 512, keep, speed);
	 }

	 void funny_action_deploy(int keep = 1000, int speed = 1023)
	 {
	 servos().deployWithVelocity(AX12_SERVO_FUNNY, 680, keep, speed);
	 }

	 void moustache_D_init_high(int keep = 0, int speed = 150)
	 {
	 servos().deployWithVelocity(AX12_SERVO_MOUSTACHE_DROITE, 253, keep, speed);
	 }

	 void moustache_D_take_balls(int keep = 0, int speed = 150)
	 {
	 servos().setTorque(AX12_SERVO_MOUSTACHE_DROITE, 1023);
	 servos().deployWithVelocity(AX12_SERVO_MOUSTACHE_DROITE, 497, keep, speed);
	 }

	 void moustache_G_init_medium_below_D(int keep = 0, int speed = 150)
	 {
	 servos().deployWithVelocity(AX12_SERVO_MOUSTACHE_GAUCHE, 670, keep, speed);
	 }

	 void moustache_G_init_high(int keep = 0, int speed = 150)
	 {
	 servos().deployWithVelocity(AX12_SERVO_MOUSTACHE_GAUCHE, 781, keep, speed);
	 }

	 void moustache_G_take_balls(int keep = 0, int speed = 150)
	 {
	 servos().setTorque(AX12_SERVO_MOUSTACHE_GAUCHE, 1023);
	 servos().deployWithVelocity(AX12_SERVO_MOUSTACHE_GAUCHE, 528, keep, speed);
	 }
	 */
	/*
	 void aspi_tete_init(int keep = 0, int speed = 150) {

	 servos().deployWithVelocity(AX12_SERVO_TETE_ASPI, 512, keep, speed);

	 }

	 void aspi_tete_gauche(int keep = 0, int speed = 150) {

	 servos().deployWithVelocity(AX12_SERVO_TETE_ASPI, 430, keep, speed);
	 //TODO torque 150
	 }

	 void aspi_tete_droite(int keep = 0, int speed = 150) {

	 servos().deployWithVelocity(AX12_SERVO_TETE_ASPI, 430, keep, speed);
	 //TODO torque 150
	 }

	 void aspi_centre(int keep = 0, int speed = 150) {

	 servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 450, keep, speed);

	 }

	 void aspi_droite(int keep = 0, int speed = 150) {

	 servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 205, keep, speed);

	 }
	 void aspi_droite_full(int keep = 0, int speed = 150) {

	 servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 105, keep, speed);

	 }

	 void aspi_gauche(int keep = 0, int speed = 150) {

	 servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 688, keep, speed);

	 }
	 void aspi_gauche_full(int keep = 0, int speed = 150) {

	 servos().deployWithVelocity(AX18_SERVO_RUSSEL_LINKAGE, 788, keep, speed);

	 }
	 */

	/*//2022
	 void elevator2022_init(int keep = 0, int speed = 1024) {
	 //        servos().setSpeed(AX12_SERVO_ELEVATOR_L, speed);
	 //        servos().setSpeed(AX12_SERVO_ELEVATOR_R, speed);
	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 459, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 562, keep, speed);
	 }

	 //deploy full avec bras devant
	 void elevator2022_deploy(int keep = 0, int speed = 1024) {
	 //        servos().setSpeed(AX12_SERVO_ELEVATOR_L, speed);
	 //        servos().setSpeed(AX12_SERVO_ELEVATOR_R, speed);
	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 766, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 254, keep, speed);
	 }

	 void elevator2022_niv1(int keep = 0, int speed = 1024) {
	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 627, 0, speed);    //584
	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 388, keep, speed);    //432
	 }

	 void elevator2022_niv2(int keep = 0, int speed = 1024) {

	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 660, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 348, keep, speed);
	 }
	 void elevator2022_niv3(int keep = 0, int speed = 1024) {

	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_L, 734, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ELEVATOR_R, 274, keep, speed);
	 }

	 void arm_R_take(int keep = 0, int speed = 1024) {
	 servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 463, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 266, keep, speed);
	 }

	 void arm_R_side_init(int keep = 0, int speed = 1024) {
	 servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 575, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 141, keep, speed);
	 }

	 void arm_R_deploy1(int keep = 0, int speed = 1024) {

	 servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 652, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 124, keep, speed);
	 }
	 void arm_R_deploy2(int keep = 0, int speed = 1024) {

	 servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 780, -1, speed);
	 servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 124, keep, speed);
	 }

	 void arm_R_deploy3(int keep = 0, int speed = 1024) {

	 servos().deployWithVelocity(AX12_SERVO_ARM_R_TOP, 635, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ARM_R_BOTTOM, 380, keep, speed);

	 }

	 void arm_L_take(int keep = 0, int speed = 1024) {
	 servos().deployWithVelocity(AX12_SERVO_ARM_L_TOP, 539, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ARM_L_BOTTOM, 784, keep, speed);
	 }

	 void arm_L_side_init(int keep = 0, int speed = 1024) {
	 servos().deployWithVelocity(AX12_SERVO_ARM_L_TOP, 445, 0, speed);
	 servos().deployWithVelocity(AX12_SERVO_ARM_L_BOTTOM, 876, keep, speed);
	 }

	 void pump_R(int activate) {
	 tirette().setGPIO(4, activate);
	 }

	 void pump_R_electrov(int activate) {
	 tirette().setGPIO(5, activate);
	 }

	 void pump_L(int activate) {
	 tirette().setGPIO(6, activate);
	 }

	 void pump_L_electrov(int activate) {
	 tirette().setGPIO(7, activate);
	 }
	 */

	/*
	 void ax12_init() {

	 //on monte le barillet
	 ax12_elevator_up(true);
	 //TODO fermer les portes

	 //monter baisser le drapeau
	 //ax12_drapeaux();
	 ax12_drapeaux_init();

	 //remonter les bras
	 ax12_bras_droit_init();
	 ax12_bras_gauche_init();
	 }

	 void ax12_drapeaux_init() {
	 //        servosAx12().setSpeed(AX12_SERVO_DRAPEAUX, 400);
	 //        servosAx12().deploy(AX12_SERVO_DRAPEAUX, 806, -1);
	 }
	 void ax12_drapeaux() {
	 //        servosAx12().setSpeed(AX12_SERVO_DRAPEAUX, 400);
	 //        servosAx12().deploy(AX12_SERVO_DRAPEAUX, 204, -1);
	 }

	 void ax12_bras_droit_init(int keep = 0, int speed = 1024) {
	 //        servosAx12().setSpeed(AX12_SERVO_BRAS_D, speed);
	 //        servosAx12().deploy(AX12_SERVO_BRAS_D, 815, keep);
	 }
	 void ax12_bras_droit(int keep = 0, int speed = 1024) {
	 //        servosAx12().setSpeed(AX12_SERVO_BRAS_D, speed);
	 //        servosAx12().deploy(AX12_SERVO_BRAS_D, 480, keep);
	 }
	 void ax12_bras_droit_fanion(int keep = 0, int speed = 1024) {
	 //        servosAx12().setSpeed(AX12_SERVO_BRAS_D, speed);
	 //        servosAx12().deploy(AX12_SERVO_BRAS_D, 396, keep);
	 }

	 void ax12_bras_gauche_init(int keep = 0, int speed = 1024) {
	 //        servosAx12().setSpeed(AX12_SERVO_BRAS_G, speed);
	 //        servosAx12().deploy(AX12_SERVO_BRAS_G, 205, keep);
	 }
	 void ax12_bras_gauche(int keep = 0, int speed = 1024) {
	 //        servosAx12().setSpeed(AX12_SERVO_BRAS_G, speed);
	 //        servosAx12().deploy(AX12_SERVO_BRAS_G, 512, keep);
	 }
	 void ax12_bras_gauche_fanion(int keep = 0, int speed = 1024) {
	 //        servosAx12().setSpeed(AX12_SERVO_BRAS_G, speed);
	 //        servosAx12().deploy(AX12_SERVO_BRAS_G, 598, keep);
	 }

	 void ax12_elevator_up(bool keep = true) {
	 //        int high = 3865;
	 //        servosAx12().setSpeed(AX12_SERVO_ASCENSEUR, 1023);
	 //        servosAx12().deploy(AX12_SERVO_ASCENSEUR, high, -1);
	 //        while (servosAx12().getPos(AX12_SERVO_ASCENSEUR) > (high - 5)) {
	 //            servosAx12().deploy(AX12_SERVO_ASCENSEUR, high, -1);
	 //        }
	 //        if (!keep) servosAx12().release(AX12_SERVO_ASCENSEUR);
	 }
	 void ax12_elevator_down(bool keep = true) {
	 //        servosAx12().setSpeed(AX12_SERVO_ASCENSEUR, 1000);
	 //        servosAx12().deploy(AX12_SERVO_ASCENSEUR, 553, -1);
	 //        if (!keep) servosAx12().release(AX12_SERVO_ASCENSEUR);
	 }

	 void ax12_rotation_black_side(bool keep = true) {
	 //        servosAx12().setSpeed(AX12_SERVO_ROTATION, 1000);
	 //        servosAx12().deploy(AX12_SERVO_ROTATION, 3905, -1);
	 //        if (!keep) servosAx12().release(AX12_SERVO_ROTATION);
	 }

	 void ax12_rotation_blue_side(bool keep = true) {
	 //        servosAx12().setSpeed(AX12_SERVO_ROTATION, 1000);
	 //        servosAx12().deploy(AX12_SERVO_ROTATION, 123, -1);
	 //        if (!keep) servosAx12().release(AX12_SERVO_ROTATION);
	 }

	 void ax12_pince_a_droite(bool keep = true) {
	 //        servosAx12().setSpeed(AX12_SERVO_PINCE_SS_ROBOT, 512);
	 //        servosAx12().deploy(AX12_SERVO_PINCE_SS_ROBOT, 889, -1);
	 //        if (!keep) servosAx12().release(AX12_SERVO_PINCE_SS_ROBOT);
	 }

	 void ax12_pince_a_gauche(bool keep = true) {
	 //        servosAx12().setSpeed(AX12_SERVO_PINCE_SS_ROBOT, 512);
	 //        servosAx12().deploy(AX12_SERVO_PINCE_SS_ROBOT, 108, -1);
	 //        if (!keep) servosAx12().release(AX12_SERVO_PINCE_SS_ROBOT);
	 }

	 void ax12_pince_au_centre(bool keep = true) {
	 //        servosAx12().setSpeed(AX12_SERVO_PINCE_SS_ROBOT, 512);
	 //        servosAx12().deploy(AX12_SERVO_PINCE_SS_ROBOT, 486, -1);
	 //        if (!keep) servosAx12().release(AX12_SERVO_PINCE_SS_ROBOT);
	 }

	 */
	/*
	 void ax12_init()
	 {
	 servosAx12().detect();

	 ax12_leftHand();
	 ax12_rightHand();
	 ax12_left_cil_retract();
	 ax12_right_cil_retract(-1);
	 utils::sleep_for_micros(200000);
	 ax12_leftHand_retract();
	 ax12_rightHand_retract();
	 ax12_left_cil();
	 ax12_right_cil(-1);

	 ax12_left_cil_retract();
	 ax12_right_cil_retract();
	 }

	 void ax12_rightHand_retract(int keep = 0, int speed = 512)
	 {
	 servosAx12().setSpeed(7, speed);
	 servosAx12().deploy(7, 512, keep);
	 }

	 void ax12_rightHand(int keep = 0, int speed = 512)
	 {
	 servosAx12().setSpeed(7, speed);
	 servosAx12().deploy(7, 732, keep);
	 }

	 void ax12_leftHand_retract(int keep = 0, int speed = 512)
	 {
	 servosAx12().setSpeed(5, speed);
	 servosAx12().deploy(5, 512, keep);
	 }
	 void ax12_leftHand(int keep = 0, int speed = 512)
	 {
	 servosAx12().setSpeed(5, speed);
	 servosAx12().deploy(5, 300, keep);
	 }

	 void ax12_left_cil_release(int keep = 0, int speed = 512)
	 {
	 servosAx12().release(180);
	 servosAx12().release(181);
	 }

	 void ax12_left_cil_retract(int keep = 0, int speed = 512)
	 {
	 //activation du capteur central
	 sensors_.addConfigFront(true, true, true);

	 servosAx12().setSpeed(180, speed);
	 servosAx12().deploy(180, 512, keep);

	 }

	 void ax12_left_cil_retract_less(int keep = 0, int speed = 512)
	 {
	 //activation du capteur central
	 sensors_.addConfigFront(true, true, true);

	 servosAx12().setSpeed(180, speed);
	 servosAx12().deploy(180, 350, keep);

	 }

	 void ax12_left_cil_retract_more(int keep = 0, int speed = 512)
	 {
	 //activation du capteur central
	 sensors_.addConfigFront(true, true, true);

	 servosAx12().setSpeed(180, speed);
	 servosAx12().deploy(180, 572, keep);

	 }
	 void ax12_left_cil(int keep = 0, int speed = 512)
	 {
	 //desactivation du capteur central
	 sensors_.addConfigFront(true, false, true);

	 servosAx12().setSpeed(180, speed);
	 servosAx12().deploy(180, 152, keep);    //232 142
	 }

	 void ax12_right_cil_retract(int keep = 0, int speed = 512)
	 {
	 servosAx12().setSpeed(181, speed);
	 servosAx12().deploy(181, 512, keep);

	 }

	 void ax12_right_cil_retract_less(int keep = 0, int speed = 512)
	 {
	 servosAx12().setSpeed(181, speed);
	 servosAx12().deploy(181, 650, keep);

	 }
	 void ax12_right_cil_retract_more(int keep = 0, int speed = 512)
	 {
	 servosAx12().setSpeed(181, speed);
	 servosAx12().deploy(181, 462, keep);

	 }
	 void ax12_right_cil(int keep = 0, int speed = 512)
	 {
	 servosAx12().setSpeed(181, speed);
	 servosAx12().deploy(181, 842, keep);    //792 852
	 }

	 bool ax12_goldenium_in_cil()
	 {
	 int leftT = servosAx12().getTorque(180);
	 int RightT = servosAx12().getTorque(181);

	 if (RightT > 900 && leftT < 300)
	 return true;
	 else
	 return false;

	 }*/

};

#endif

