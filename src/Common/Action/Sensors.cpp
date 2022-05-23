#include "Sensors.hpp"

#include <algorithm>
#include <sstream>

#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"
#include "../Asserv/Asserv.hpp"
#include "../Robot.hpp"
#include "Actions.hpp"

using namespace std;

Sensors::Sensors(Actions &actions, Robot *robot) :
        AActionsElement(actions), robot_(robot)

{
    sensorsdriver_ = ASensorsDriver::create(robot->getID());

    addThresholdFront(0, 0, 0);
    addThresholdFrontVeryClosed(0, 0, 0);
    addThresholdBack(0, 0, 0);
    addThresholdBackVeryClosed(0, 0, 0);
    addConfigFront(false, false, false);
    addConfigBack(false, false, false);

    setIgnoreAllFrontNearObstacle(false);
    setIgnoreAllBackNearObstacle(false);

    x_adv_mm = -1.0;
    y_adv_mm = -1.0;

    recordADC = false;
    for (int i = 0; i < 100; i++) {
        tabADC[i] = 0;
    }
    index_adc = 0;

}
Sensors::~Sensors() {
    delete sensorsdriver_;
}

SensorsTimer::SensorsTimer(Sensors &sensors, int timeSpan_ms, std::string name) :
        sensors_(sensors)
{
    name_ = name;
    timeSpan_us_ = timeSpan_ms * 1000;

    lastdetect_front_nb_ = 0;
    lastdetect_back_nb_ = 0;

    lastfrontl2_temp_ = false;
    lastbackl2_temp_ = false;

    nb_sensor_front_a_zero = 0;
    nb_sensor_back_a_zero = 0;
    nb_sensor_level2 = 0;

    //initialise le timer avec le nom et la periode.
    this->init(name_, timeSpan_us_);
}

void Sensors::addThresholdFront(int left, int center, int right) {
    frontLeftThreshold_ = left;
    frontCenterThreshold_ = center;
    frontRightThreshold_ = right;
}
void Sensors::addThresholdFrontVeryClosed(int left, int center, int right) {
    frontLeftVeryClosedThreshold_ = left;
    frontCenterVeryClosedThreshold_ = center;
    frontRightVeryClosedThreshold_ = right;
}
void Sensors::addThresholdBack(int left, int center, int right) {
    backLeftThreshold_ = left;
    backCenterThreshold_ = center;
    backRightThreshold_ = right;
}
void Sensors::addThresholdBackVeryClosed(int left, int center, int right) {
    backLeftVeryClosedThreshold_ = left;
    backCenterVeryClosedThreshold_ = center;
    backRightVeryClosedThreshold_ = right;
}

void Sensors::addConfigFront(bool left, bool center, bool right) {
    enableFrontLeft_ = left;
    enableFrontCenter_ = center;
    enableFrontRight_ = right;
}
void Sensors::addConfigBack(bool left, bool center, bool right) {
    enableBackLeft_ = left;
    enableBackCenter_ = center;
    enableBackRight_ = right;
}

void Sensors::setIgnoreFrontNearObstacle(bool ignoreLeft, bool ignoreCenter, bool ignoreRight) {
    ignoreFrontLeft_ = ignoreLeft;
    ignoreFrontCenter_ = ignoreCenter;
    ignoreFrontRight_ = ignoreRight;
}
void Sensors::setIgnoreBackNearObstacle(bool ignoreLeft, bool ignoreCenter, bool ignoreRight) {
    ignoreBackLeft_ = ignoreLeft;
    ignoreBackCenter_ = ignoreCenter;
    ignoreBackRight_ = ignoreRight;
}
void Sensors::setIgnoreAllFrontNearObstacle(bool ignore) {
    ignoreFrontLeft_ = ignore;
    ignoreFrontCenter_ = ignore;
    ignoreFrontRight_ = ignore;
}
void Sensors::setIgnoreAllBackNearObstacle(bool ignore) {
    ignoreBackLeft_ = ignore;
    ignoreBackCenter_ = ignore;
    ignoreBackRight_ = ignore;
}

ASensorsDriver::bot_positions Sensors::getPositionsAdv() {
    //recupere les données qui ont ete enregistrées par le sync
    return sensorsdriver_->getvPositionsAdv();
}

void Sensors::display(int n) {
    sensorsdriver_->displayNumber(n);
}

int Sensors::getADC() {
    return sensorsdriver_->getAnalogPinData();
}

int Sensors::RecordADC(bool activate) {

    if (activate) { //uniquement en cas d'activation
        for (int i = 0; i < 100; i++) {
            tabADC[i] = -1;
        }
        index_adc = 0;
    }

    recordADC = activate;
}

int Sensors::sync(std::string sensorname) {
    //synchronise les données sur les sensors drivers
    if (sensorname == "beacon_sync") {
        return sensorsdriver_->sync(); //renvoi -1 si erreur
    }

    if (sensorname == "right") {
        multipleRightSide(10);
    }

    if (sensorname == "left") {
        multipleLeftSide(10);
    }

    if (sensorname == "fL") {
        return sensorsdriver_->frontLeft(); //renvoi la distance mini
    }
//    if (sensorname == "fC") {
//        return sensorsdriver_->frontCenter();
//    }
    if (sensorname == "fR") {
        return sensorsdriver_->frontRight(); //renvoi la distance mini
    }
    if (sensorname == "bL") {
        return sensorsdriver_->backLeft(); //renvoi la distance mini
    }
//    if (sensorname == "bC") {
//        return sensorsdriver_->backCenter();
//    }
    if (sensorname == "bR") {
        return sensorsdriver_->backRight(); //renvoi la distance mini
    }

//    if (sensorname == "beaconADV") {
////        ASensorsDriver::bot_positions
////            return sensorsdriver_->getvPositionsAdv();
//        }
    return -1;
}

float Sensors::multipleRightSide(int nb) {
    int data[nb];
    float moy = 0.0;
    for (int ii = 0; ii < nb; ii++) {
        data[ii] = rightSide();
        //logger().debug() << "Right= " << data[ii] << logs::end;
        utils::sleep_for_micros(40000);
    }

    //Now we call the sort function
    std::sort(data, data + nb);

//    for (int ii = 0; ii < nb; ii++) {
//        logger().info() << "rtrie= " << data[ii] << logs::end;
//        //usleep(50000);
//    }

    moy = (data[3] + data[4] + data[5] + data[6]) / 4;

    return moy;
}

float Sensors::multipleLeftSide(int nb) {
    int data[nb];

    float moy = 0.0;
    for (int ii = 0; ii < nb; ii++) {
        data[ii] = leftSide();
        logger().debug() << "Left= " << data[ii] << logs::end;
        utils::sleep_for_micros(40000);
    }

    //Now we call the sort function
    std::sort(data, data + nb);

    for (int ii = 0; ii < nb; ii++) {
        logger().info() << "ltrie= " << data[ii] << logs::end;
        //usleep(50000);
    }

    moy = (data[3] + data[4] + data[5] + data[6]) / 4;

    return moy;
}

int Sensors::rightSide() {
    return sensorsdriver_->rightSide();
}
int Sensors::leftSide() {
    return sensorsdriver_->leftSide();
}

//retourne 0, sinon le niveau detecté 2 veryClosed, 1 first level
int Sensors::front(bool display) {

    //on recupere les distances de detection
    int fL = sync("fL");
    //int fC = sync("fC");
    int fR = sync("fR");

    ASensorsDriver::bot_positions vpos;

//    int tfL = 0;
//    int tfC = 0;
//    int tfR = 0;
    int tfMin = 9999;

    //logger().info() << " L " << enableFrontLeft_ << " C " << enableFrontCenter_ << " R " << enableFrontRight_ << logs::end;
    int level = 0;

    if (enableFrontLeft_) //existance
    {
        bool fL_filter = this->robot()->asserv()->filtre_IsInsideTable(fL, -1, "fL"); //negatif = capteur placé à gauche
        //logger().info() << " fL_filter= " << fL_filter << logs::end;
        if (fL_filter) {
            {
                if ((!ignoreFrontLeft_ && (fL < frontLeftThreshold_))) {
                    if (display) logger().info() << "1 frontLeft= " << fL << logs::end;
//                tfL = fL;
                    if (fL > 60) if (tfMin > fL) tfMin = fL;
                    //tfMin = fL;
                    level = 1;
                }
                if ((!ignoreFrontLeft_ && (fL < frontLeftVeryClosedThreshold_))) {
                    if (display) logger().info() << "2 frontLeft= " << fL << logs::end;
                    level = 2;
                }
            }
        }
    }
    if (enableFrontCenter_) {
        //trouver la distance de detection
        vpos = Sensors::getPositionsAdv();
        bool is_in_front = false;

        for (auto botpos : vpos) {
            //std::cout << botpos.d << " " << botpos.x << " " << botpos.y << " " << botpos.theta << std::endl;
            if (botpos.d < frontCenterThreshold_) //seuil de distance
                    {
                //test si devant le robot/filtre devant le robot ou pas.
                is_in_front |= this->robot()->asserv()->filtre_IsInFront(botpos.d, botpos.x, botpos.y, botpos.theta);

                if (is_in_front) {
                    //TODO filtre dans la table ou non
                    //bool fC_filter = this->robot()->asserv()->filtre_IsInsideTable(botpos.d, 0, "fC");

                    level = 1;
                    logger().info() << "1 frontCenter= " << botpos.d << " is_in_front=" << is_in_front << logs::end;
                    if (botpos.d < frontCenterVeryClosedThreshold_) { //seuil de level 2
                        level = 2;
                        logger().info() << "2 frontCenter= " << botpos.d << " is_in_front=" << is_in_front << logs::end;

                    }
                }

            }
            if (botpos.nbDetectedBots == 1) break;
        }

//        bool fC_filter = this->robot()->asserv()->filtre_IsInsideTable(fC, 0, "fC");
//        if (fC_filter) {
//            if ((!ignoreFrontCenter_ && (fC < frontCenterThreshold_))) {
//                if (display) logger().info() << "1 frontCenter= " << fC << logs::end;
////                tfC = fC;
//                if (fC > 60) if (tfMin > fC) tfMin = fC;
//                level = 1;
//            }
//            if ((!ignoreFrontCenter_ && (fC < frontCenterVeryClosedThreshold_))) {
//                if (display) logger().info() << "2 frontCenter= " << fC << logs::end;
//                level = 2;
//            }
//        }
    }
    if (enableFrontRight_) {
        bool fR_filter = this->robot()->asserv()->filtre_IsInsideTable(fR, 1, "fR");
        if (fR_filter) {
            if ((!ignoreFrontRight_ && (fR < frontRightThreshold_))) {
                if (display) logger().info() << "1 frontRight= " << fR << logs::end;
//                tfR = fR;
                if (fR > 60) if (tfMin > fR) tfMin = fR;
                level = 1;
            }
            if ((!ignoreFrontRight_ && (fR < frontRightVeryClosedThreshold_))) {
                if (display) logger().info() << "2 frontRight= " << fR << logs::end;
                level = 2;
            }
        }
    }
    /*
     if (enableFrontLeft_)
     if (this->robot()->asserv()->filtre_IsInsideTable(fL, -1)) { //negatif = capteur placé à gauche
     if ((!ignoreFrontLeft_ && (fL < frontLeftVeryClosedThreshold_))) {
     if (display)
     logger().info() << "2 frontLeft= " << fL << logs::end;
     level = 2;
     }
     }
     if (enableFrontCenter_)
     if (this->robot()->asserv()->filtre_IsInsideTable(fC, 0)) {
     if ((!ignoreFrontCenter_ && (fC < frontCenterVeryClosedThreshold_))) {
     if (display)
     logger().info() << "2 frontCenter= " << fC << logs::end;
     level = 2;
     }
     }
     if (enableFrontRight_)
     if (this->robot()->asserv()->filtre_IsInsideTable(fR, +1)) {
     if ((!ignoreFrontRight_ && (fR < frontRightVeryClosedThreshold_))) {
     if (display)
     logger().info() << "2 frontRight= " << fR << logs::end;
     level = 2;
     }
     }
     */
    //Mise à jour de la position de l'adversaire via les capteurs de proximité
    if (level >= 1) {
        //logger().info() << "front  level >=1 tfMin= " << tfMin << logs::end;
        if (tfMin != 0) {
            //au centre
            //logger().info() << "front         tfMin= " << tfMin << logs::end;
            x_adv_mm = (float) tfMin;
            y_adv_mm = 0.0;
        }
        else {
            x_adv_mm = -1.0;
            y_adv_mm = -1.0;
        }
    }

    return level;
}

int Sensors::back(bool display) {
    //on recupere les distances de detection
    int bL = sync("bL");
    int bC = sync("bC");
    int bR = sync("bR");

    int tfMin = 9999;
    int level = 0;

    ASensorsDriver::bot_positions vpos;

    if (enableBackLeft_) {
        bool bL_filter = this->robot()->asserv()->filtre_IsInsideTable(-bL, -1, "bL");
        if (bL_filter) { //negatif = capteur placé à gauche
            if ((!ignoreBackLeft_ && (bL < backLeftThreshold_))) {
                if (display) logger().info() << "1 backLeft= " << bL << logs::end;
                //tfMin = bL;
                if (bL > 60) if (tfMin > bL) tfMin = bL;
                level = 1;
            }
            if ((!ignoreBackLeft_ && (bL < backLeftVeryClosedThreshold_))) {
                if (display) logger().info() << "2 backLeft= " << bL << logs::end;
                level = 2;
            }
        }
    }
    if (enableBackCenter_) {
//        bool bC_filter = this->robot()->asserv()->filtre_IsInsideTable(-bC, 0, "bC");
//        if (bC_filter) {
//            if ((!ignoreBackCenter_ && (bC < backCenterThreshold_))) {
//                if (display) logger().info() << "1 backCenter= " << bC << logs::end;
//                if (bC > 60) if (tfMin > bC) tfMin = bC;
//                level = 1;
//            }
//            if ((!ignoreBackCenter_ && (bC < backCenterVeryClosedThreshold_))) {
//                if (display) logger().info() << "2 backCenter= " << bC << logs::end;
//                level = 2;
//            }
//        }
        //trouver la distance de detection
        vpos = Sensors::getPositionsAdv();
        bool is_in_back = false;

        for (auto botpos : vpos) {
            //std::cout << botpos.d << " " << botpos.x << " " << botpos.y << " " << botpos.theta << std::endl;
            if (botpos.d < backCenterThreshold_) //seuil de distance
                    {
                //test si devant le robot/filtre devant le robot ou pas.
                is_in_back |= this->robot()->asserv()->filtre_IsInBack(botpos.d, botpos.x, botpos.y, botpos.theta);

                if (is_in_back) {
                    //TODO filtre dans la table ou non

                    level = 1;
                    logger().info() << "1 backCenter= " << botpos.d << " is_in_back=" << is_in_back << logs::end;
                    if (botpos.d < backCenterVeryClosedThreshold_) { //seuil de level 2
                        level = 2;
                        logger().info() << "2 backCenter= " << botpos.d << " is_in_back=" << is_in_back << logs::end;

                    }
                }

            }
            if (botpos.nbDetectedBots == 1) break;
        }

    }
    if (enableBackRight_) {
        bool bR_filter = this->robot()->asserv()->filtre_IsInsideTable(-bR, 1, "bR");
        if (bR_filter) {
            if ((!ignoreBackRight_ && (bR < backRightThreshold_))) {
                if (display) logger().info() << "1 backRight= " << bR << logs::end;
                if (bR > 60) if (tfMin > bR) tfMin = bR;
                level = 1;
            }
            if ((!ignoreBackRight_ && (bR < backRightVeryClosedThreshold_))) {
                if (display) logger().info() << "2 backRight= " << bR << logs::end;
                level = 2;
            }
        }
    }
    /*
     if (enableBackLeft_)
     if (this->robot()->asserv()->filtre_IsInsideTable(-bL, -1)) {
     if ((!ignoreBackLeft_ && (bL < backLeftVeryClosedThreshold_))) {
     logger().info() << "2 backLeft= " << bL << logs::end;
     level = 2;
     }
     }
     if (enableBackCenter_)
     if (this->robot()->asserv()->filtre_IsInsideTable(-bC, 0)) {
     if ((!ignoreBackCenter_ && (bC < backCenterVeryClosedThreshold_))) {
     logger().info() << "2 backCenter= " << bC << logs::end;
     level = 2;
     }
     }
     if (enableBackRight_)
     if (this->robot()->asserv()->filtre_IsInsideTable(-bR, 1)) {
     if ((!ignoreBackRight_ && (bR < backRightVeryClosedThreshold_))) {
     logger().info() << "2 backRight= " << bR << logs::end;
     level = 2;
     }
     }*/
    /*
     //Mise à jour de la position de l'adversaire
     if (level >= 1) {
     if (tfMin != 0) {
     //au centre
     x_adv_mm = -tfMin;
     y_adv_mm = 0.0;
     }
     }*/

    //Mise à jour de la position de l'adversaire via les capteurs de proximité
    if (level >= 1) {
        //logger().info() << "back  level >=1 tfMin= " << tfMin << logs::end;
        if (tfMin != 0) {
            //au centre
            //logger().info() << "back         tfMin= " << tfMin << logs::end;
            x_adv_mm = (float) -tfMin;
            y_adv_mm = 0.0;
        }
        else {
            x_adv_mm = -1.0;
            y_adv_mm = -1.0;
        }
    }

    return level;
}

void Sensors::addTimerSensors(int timeSpan_ms) {
    logger().debug() << "startSensors" << logs::end;
    this->actions().addTimer(new SensorsTimer(*this, timeSpan_ms, "Sensors"));
}

void Sensors::stopTimerSensors() {
    logger().debug() << "stopSensors" << logs::end;
    this->actions().stopTimer("Sensors");
}

void SensorsTimer::onTimer(utils::Chronometer chrono) {
    //logger().debug() << ">> SensorsTimer::onTimer" << logs::end;

    //get all data sync
    int err = sensors_.sync("beacon_sync");
    if (err < 0) {
        logger().error() << ">> SYNC BAD DATA! NO UPDATE" << logs::end;
        return;
    }

    if (sensors_.robot()->getID() == "LegoEV3Robot") {
        //traitement 2022 ADC
        if (sensors_.recordADC == true) {
            sensors_.index_adc++;
            int adc = sensors_.getADC();
            logger().error() << "recordADC index=" << sensors_.index_adc << " adc=" << adc << logs::end;

            //default =>510 => 0
            //4,7k    =>462 => 99
            //1k      =>340 =>10
            //470     =>246 =>10
            //courcircuit =>0 =>1
            if (adc < 20) sensors_.tabADC[sensors_.index_adc] = 1;

            else if (adc > 200 && adc < 380) sensors_.tabADC[sensors_.index_adc] = 10;
            else if (adc > 400 && adc < 486) sensors_.tabADC[sensors_.index_adc] = 99;
            else if (adc > 490 && adc < 520) sensors_.tabADC[sensors_.index_adc] = 0;
            else sensors_.tabADC[sensors_.index_adc] = -adc;

            if (sensors_.index_adc > 5 * 20) //si superieur a 5 fois par sec pour 5 secondes
                    {
                sensors_.index_adc = 0;
                sensors_.recordADC = false;
                logger().error() << " >>>> DEPASSEMENT DES ESSAIS" << logs::end;
            }

        }
    }

    //TODO mODIFIER LE FRONT POUR INCLURE LES POSITIONS adv
    //TODO mettre a jour les zones adv

    int frontLevel = sensors_.front(false);
    //logger().error() << "frontLevel=="<< frontLevel << logs::end;

//    if (frontLevel >= 1) {
//        //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
//                    sensors_.robot()->asserv()->warnFrontCollisionOnTraj(frontLevel, sensors_.x_adv_mm, sensors_.y_adv_mm);
//                    //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
//                    sensors_.robot()->asserv()->setLowSpeedForward(true, 10);
//    }

    if (frontLevel == 0) {
        nb_sensor_front_a_zero++;
    }
    else {
        nb_sensor_front_a_zero = 0;
    }
    if (frontLevel >= 2) {
        nb_sensor_level2++;
    }
    else {
        nb_sensor_level2 = 0;
    }

    if (frontLevel == 1) {

        if (lastfrontl2_temp_ == true) //si on vient de descendre du level 2
                {
            logger().error() << "front : si on vient de descendre du level 2" << logs::end;
            lastdetect_front_nb_ = 1;
            lastfrontl2_temp_ = false;
            sensors_.robot()->asserv()->resetEmergencyOnTraj("front descendre du level 2");
            sensors_.robot()->resetDisplayObstacle();
        }

//send collision to asserv
        if (lastdetect_front_nb_ == 0) {
            //logger().error() << "front :send collision to asserv!!!!!!!!!!!!!!!!!!!!!!" << logs::end;
            //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->warnFrontCollisionOnTraj(frontLevel, sensors_.x_adv_mm, sensors_.y_adv_mm);
            //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->setLowSpeedForward(true, 20);

        }
        lastdetect_front_nb_++;
    }

//cas nearest
    if (lastdetect_front_nb_ > 0) {
        if (nb_sensor_level2 >= 2) {
            //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->warnFrontCollisionOnTraj(frontLevel, sensors_.x_adv_mm, sensors_.y_adv_mm);
            //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            lastfrontl2_temp_ = true;
        }
        if (nb_sensor_front_a_zero >= 3) {
            lastdetect_front_nb_ = 0;
            sensors_.robot()->asserv()->setLowSpeedForward(false); //surcharge par robot utilisée
            sensors_.robot()->asserv()->resetEmergencyOnTraj("SensorsTimer front=0");
            sensors_.robot()->resetDisplayObstacle();
        }
    }
    sensors_.robot()->displayObstacle(frontLevel); //TODO a separer en displayObstacleFront et displayObstacleBack

//ARRIERE/////////////////////////////////////////////////////////////////
    int backLevel = sensors_.back(false);
    if (backLevel == 0) {
        nb_sensor_back_a_zero++;
    }
    else {
        nb_sensor_back_a_zero = 0;
    }
    if (backLevel == 1) {
        if (lastbackl2_temp_ == true) //si on vient de descendre du level 2
                {
            logger().error() << "back : si on vient de descendre du level 2" << logs::end;
            lastdetect_back_nb_ = 1;
            lastbackl2_temp_ = false;
            sensors_.robot()->asserv()->resetEmergencyOnTraj("back descendre du level 2");
        }
//send collision to asserv

        if (lastdetect_back_nb_ == 0) {
            //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->warnBackCollisionOnTraj(sensors_.x_adv_mm, sensors_.y_adv_mm);
            //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->setLowSpeedBackward(true, 20);

        }
        lastdetect_back_nb_++;
    }
//sensors_.robot()->displayObstacle(backLevel); //fusioner avec le display front ?
    if (lastdetect_back_nb_ > 0) {
        if (backLevel >= 2) {
            //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            sensors_.robot()->asserv()->warnBackCollisionOnTraj(sensors_.x_adv_mm, sensors_.y_adv_mm);
            //sensors_.robot()->asserv()->base()->motors().stopMotors(); //pour etre plus reactif sur l'arret sinon on touche
            lastbackl2_temp_ = true;
        }
        if (nb_sensor_back_a_zero >= 3) {
            lastdetect_back_nb_ = 0;
            sensors_.robot()->asserv()->setLowSpeedBackward(false); //surcharge par robot utilisée
            sensors_.robot()->asserv()->resetEmergencyOnTraj("SensorsTimer back=0");
//sensors_.robot()->resetDisplayObstacle();
        }
    }

    logger().debug() << "onTimer() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec()
    // << " us lastdetect_front_nb_ =" << lastdetect_front_nb_ << " front=" << frontLevel << " back=" << backLevel
            << logs::end;

}

void SensorsTimer::onTimerEnd(utils::Chronometer chrono) {
//    logger().debug() << "onTimerEnd() " << this->info() << "=" << chrono.getElapsedTimeInMicroSec() << " us"
//            << logs::end;

}

std::string SensorsTimer::info() {
    std::ostringstream oss;
    oss << "SensorsTimer [" << name() << "] for " << sensors_.robot()->getID();
    return oss.str();
}

