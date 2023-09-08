/*!
 * \file
 * \brief Implémentation de la classe SensorDriverTest.
 */

#include "SensorDriverTest.hpp"

#include <string>
#include <vector>

#include "../../src/Log/Logger.hpp"

void test::SensorDriverTest::suite()
{
    psvgw_->beginHeader();
    //this->testSensors();
    this->testDifferentPositions();

    psvgw_->endHeader();
}

void test::SensorDriverTest::testSensors()
{
    logger().debug() << "testSensors()" << logs::end;
    bool front = false;
    bool back = false;

    for (int i = 0; i < 10; i++) {
        front = sensordriver_->frontCenter();
        back = sensordriver_->backCenter();
        logger().info() << "front=" << front << "  back=" << back << logs::end;
        //usleep(500000);
    }
    this->assert(true, "OK");
}

void test::SensorDriverTest::testDifferentPositions()
{
    ROBOTPOSITION p; //position du robot
    logger().debug() << "transform()" << logs::end;
    //on met à jour la position du robot par l'obj commun
    //cadran 1
    p = { 1400.0, 600.0, degToRad(-45.0) };
    transform(p, 1);
    p = { 1300.0, 600.0, degToRad(-90.0) };
    transform(p, 1);
    p = { 1500.0, 600.0, degToRad(135.0) };
    transform(p, 1);
    p = { 1600.0, 600.0, degToRad(0.0) };
    transform(p, 1);
    p = { 1700.0, 700.0, degToRad(90.0) };
    transform(p, 1);
    p = { 1800.0, 800.0, degToRad(-120.0) };
    transform(p, 1);

    //cadran 2
    p = { 1400.0, 1600.0, degToRad(-45.0) };
    transform(p, 2);
    p = { 1300.0, 1600.0, degToRad(-90.0) };
    transform(p, 2);
    p = { 1500.0, 1600.0, degToRad(135.0) };
    transform(p, 2);
    p = { 1600.0, 1600.0, degToRad(0.0) };
    transform(p, 2);
    p = { 1700.0, 1700.0, degToRad(90.0) };
    transform(p, 2);
    p = { 1800.0, 1800.0, degToRad(-120.0) };
    transform(p, 2);

    //cadran 3
    p = { 400.0, 1600.0, degToRad(-45.0) };
    transform(p, 3);
    p = { 300.0, 1600.0, degToRad(-90.0) };
    transform(p, 3);
    p = { 500.0, 1600.0, degToRad(135.0) };
    transform(p, 3);
    p = { 600.0, 1600.0, degToRad(0.0) };
    transform(p, 3);
    p = { 700.0, 1700.0, degToRad(90.0) };
    transform(p, 3);
    p = { 800.0, 1800.0, degToRad(-120.0) };
    transform(p, 3);
    //cadran 4
    p = { 400.0, 600.0, degToRad(-45.0) };
    transform(p, 4);
    p = { 300.0, 600.0, degToRad(-90.0) };
    transform(p, 4);
    p = { 500.0, 600.0, degToRad(135.0) };
    transform(p, 4);
    p = { 600.0, 600.0, degToRad(0.0) };
    transform(p, 4);
    p = { 700.0, 700.0, degToRad(90.0) };
    transform(p, 4);
    p = { 800.0, 800.0, degToRad(-120.0) };
    transform(p, 4);
}

void test::SensorDriverTest::transform(ROBOTPOSITION p, int color)
{

    aRobotPositionShared_->setRobotPosition(p);
    psvgw_->writePosition_Bot(p.x, p.y, p.theta, 0);

    ASensorsDriver::bot_positions vadv;
    vadv = sensordriver_->getvPositionsAdv();
    int c = 0;

    for (ASensorsDriver::bot_positions::size_type i = 0; i < vadv.size(); i++) {
        c++;
        logger().debug() << " vadv nb=" << vadv.size() << " detected=" << vadv[i].nbDetectedBots << " x=" << vadv[i].x
                << " y=" << vadv[i].y << " a_deg=" << vadv[i].theta_deg << " d=" << vadv[i].d << logs::end;

        float x_pos_adv_table = 0;
        float y_pos_adv_table = 0;

//      transcritption du repere robot vers repere table
        ROBOTPOSITION p2 = aRobotPositionShared_->convertPositionBeaconToRepereTable(vadv[i].d, vadv[i].x, vadv[i].y,
                vadv[i].theta_deg, &x_pos_adv_table, &y_pos_adv_table);

        logger().debug() << " CONVERT  x_pos_adv_table=" << x_pos_adv_table << " y_pos_adv_table=" << y_pos_adv_table
                << " p2.x=" << p2.x << " p2.y=" << p2.y << logs::end;

        psvgw_->writePosition_AdvPos(x_pos_adv_table, y_pos_adv_table, p2.x, p2.y, color);

        //on compare les position des balises (simu et recalculé par les 2 changements de repere)

//        RobotPos pos1 = transformPosTableToPosRobot(nb, x_adv_, y_adv_);
//        RobotPos pos2 = transformPosTableToPosRobot(nb, 1000, 200);
//        RobotPos pos3 = transformPosTableToPosRobot(nb, 800, 2000);
//        RobotPos pos4 = transformPosTableToPosRobot(nb, 100, 1400);
        if (c == 1) {
            assert(cmpf(x_pos_adv_table, 1200), "X 1200 NOK:" + to_string(x_pos_adv_table));
            assert(cmpf(y_pos_adv_table, 900), "Y 900 NOK" + to_string(y_pos_adv_table));
        }
        if (c == 2) {
            assert(cmpf(x_pos_adv_table, 1000), "X 1000 NOK:" + to_string(x_pos_adv_table));
            assert(cmpf(y_pos_adv_table, 200), "Y 200 NOK" + to_string(y_pos_adv_table));
        }
        if (c == 3) {
            assert(cmpf(x_pos_adv_table, 800), "X 800 NOK:" + to_string(x_pos_adv_table));
            assert(cmpf(y_pos_adv_table, 2000), "Y 2000 NOK" + to_string(y_pos_adv_table));
        }
        if (c == 4) {
            assert(cmpf(x_pos_adv_table, 100), "X 100 NOK:" + to_string(x_pos_adv_table));
            assert(cmpf(y_pos_adv_table, 1400), "Y 1400 NOK" + to_string(y_pos_adv_table));
        }

    }
}

