#include "MovingBase.hpp"

#include <stdlib.h>
#include <cmath>
#include <thread>

#include "../../Log/Logger.hpp"
#include "../../Thread/Thread.hpp"
#include "../Interface.Driver/AAsservDriver.hpp"
#include "../Interface.Driver/ARobotPositionShared.hpp"

using namespace std;

/*!
 * \brief Constructor.
 */
MovingBase::MovingBase(std::string botId) :
        entraxe_mm(223), diam_mm(40), distTicks_(0), extEncoders_(botId, true, asservdriver_), encoders_(botId, false, asservdriver_), motors_(botId, asservdriver_)

//TODO Fichier de configuration pour distinction entre robot dans le simulateur + remonter info sur extended + ajouter info encoder
//ou en parametre dela creation dans les extended
{
    collisionDetected_ = false;

    aRobotPositionShared_ = ARobotPositionShared::create();
    asservdriver_ = AAsservDriver::create(botId, aRobotPositionShared_);

}

void MovingBase::moveDTime(int power, int timems)
{
    motors_.runMotorLeft(power, timems);
    motors_.runMotorRight(power, timems);
    utils::sleep_for_micros(1000 * timems);
}

void MovingBase::moveD(long destTick, long restTick, int power) //TODO backward ?
{
    distTicks_ = destTick;

    logger().debug() << "moveD GO : restTick=" << restTick << "  destTick=" << destTick << logs::end;
    motors_.setMotorLeftPosition(power, -restTick );
    motors_.setMotorRightPosition(power, -restTick);
}

//return le reste à parcourir
long MovingBase::moveDWaitTrajectory()
{
    int arrived = 0;
    long l = 0;
    long r = 0;
    long m = 0;

    //attente de la position
    while (!arrived) {
        //calculate arrived or not
        l = encoders_.getLeftEncoder();
        r = encoders_.getRightEncoder();
        m = ((abs(l) + abs(r)) / 2.0);
        if (m >= abs(distTicks_)) {
            arrived = 1;
            distTicks_ = 0;
            encoders_.reset();
        }

        //test if adversary then pause
        if (collisionDetected_ == 1) {
            logger().debug() << "moveDWaitTrajectory emergencyStop m =" << m << " distTicks_=" << distTicks_ << "l="
                    << l << "r=" << r << logs::end;
            this->emergencyStop();
//            sleep(1);
            return (distTicks_ - m);
        }

        utils::sleep_for_micros(10000);
        std::this_thread::yield();
    }

    //TODO use external encoders if needed
    l = encoders_.getLeftEncoder();
    r = encoders_.getRightEncoder();

    logger().debug() << "Arrived : l=" << l << " r=" << r << "   l=" << (l / 360.0 * (diam_mm * M_PI)) << "mm r="
            << (r / 360.0 * (diam_mm * M_PI)) << "mm" << logs::end;
    return 0; //nbre de ticks restant
}

void MovingBase::moveForward(int mm, int power) //TODO Backward?
{
    //convert
    //diametre chenille = 37mm
    //360 ticks => PI*D = PI*37mm
    // ?  <= distmm
    //mm * 360* ticks = 37PI
    float ticksf = (360.0 * mm) / (M_PI * diam_mm);
    int tick = (int) ticksf;

    long rest = tick; //reste à faire en tick
    while (rest > 0) {
        logger().debug() << "rest=" << rest << logs::end;
        moveD(tick, rest, power);
        rest = moveDWaitTrajectory();
        std::this_thread::yield();
    }

}

void MovingBase::turn(float degree, int power)
{
    //float entraxe = (360.0 * entraxemm) / (M_PI * diam);

    float ticksf = (entraxe_mm * degree) / diam_mm;
    int ticks = (int) ticksf;
    logger().debug() << "ticksf=" << ticksf << "  ticks=" << ticks << logs::end;
    motors_.setMotorLeftPosition(power, ticks);
    motors_.setMotorRightPosition(power, -ticks);

    int arrived = 0;
    long l = 0;
    long r = 0;
    //attente de la position
    while (!arrived) {
        l = encoders_.getLeftEncoder();
        r = encoders_.getRightEncoder();
        if (((abs(l) + abs(r)) / 2) + 1 >= abs(ticks)) {
            arrived = 1;
        }
        utils::sleep_for_micros(500);
        std::this_thread::yield();
    }

    l = encoders_.getLeftEncoder();
    r = encoders_.getRightEncoder();

    logger().debug() << "l=" << l << "  r=" << r << logs::end;
    logger().debug() << "l=" << (l / 360.0 * (diam_mm * M_PI)) << "mm  r=" << (r / 360.0 * (diam_mm * M_PI)) << "mm"
            << logs::end;

}

void MovingBase::turnLeft(float degree, int power)
{
    turn(degree, power);
}

void MovingBase::turnRight(float degree, int power)
{
    turn(-degree, power);
}

void MovingBase::turnRightTime(int power, int timems)
{
    motors_.runMotorLeft(power, timems);
    motors_.runMotorRight(power, timems);
    utils::sleep_for_micros(1000 * timems);
}

void MovingBase::turnLeftTime(int power, int timems)
{
    motors_.runMotorLeft(power, timems);
    motors_.runMotorRight(power, timems);
    utils::sleep_for_micros(1000 * timems);
}

void MovingBase::emergencyStop()
{
    motors_.stopMotors();
}
