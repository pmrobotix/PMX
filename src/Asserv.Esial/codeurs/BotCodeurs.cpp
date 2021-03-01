#include "../../Common/Utils/Chronometer.hpp"
#include "../../Log/Logger.hpp"

#if CONFIG_MOTORCTRL_BOTMOTORS

#include "../../Common/Asserv/Asserv.hpp"
#include "../../Common/Asserv/ExtEncoderControl.hpp"
#include "../../Common/Asserv/MovingBase.hpp"
#include "../../Common/Robot.hpp"
#include "../config/config.h"
#include "BotCodeurs.h"

BotCodeurs::BotCodeurs(Robot * robot)
{
    robot_ = robot; //Reference vers le robot
    lastCountD = 0;
    lastCountG = 0;
}

BotCodeurs::~BotCodeurs()
{
}

void BotCodeurs::getCounts(int32_t* countG, int32_t* countD)
{
    // TODO !!! A OPTIMISER avec un seul appel ?

    /*
     * On récupère le compte des codeurs depuis le dernier refresh.
     * Pour ne pas avoir à resetter les compteurs à chaque refresh (et
     * donc risquer de perdre un ou deux ticks codeurs), on compare à la
     * valeur précédente des compteurs. Il *pourrait* y avoir un
     * overflow si l'un des codeurs génère plus de 2^31 ticks par
     * refresh, càd si le robot dépasse le mur du son...
     */

//    utils::Chronometer chrono;
//    chrono.start();

//    long long t0 = chrono.getElapsedTimeInMicroSec();
    int32_t tempD = robot_->asserv()->base()->extEncoders().getRightEncoder();

//    long long t1 = chrono.getElapsedTimeInMicroSec();

    int32_t tempG = robot_->asserv()->base()->extEncoders().getLeftEncoder();

//    long long t2 = chrono.getElapsedTimeInMicroSec();

    *countD = tempD - lastCountD;
    lastCountD = tempD;

    *countG = tempG - lastCountG;
    lastCountG = tempG;

    // On ajuste le sens des codeurs en cas de problème de cablage
    if (Config::swapCodeurs) { //inversion des deux codeurs
        int32_t temp = *countG;
        *countG = *countD;
        *countD = temp;
    }

    if (Config::inverseCodeurD) { //Changement de sens de rotation du codeur droit
        *countD = -(*countD);
    }

    if (Config::inverseCodeurG) { //Changement de sens de rotation du codeur gauche
        *countG = -(*countG);
    }

//    long long t3 = chrono.getElapsedTimeInMicroSec();

    //robot_->logger().info() << "t1=" << t1-t0 << " t2-t1=" << t2 - t1 << " t3-t2=" << t3 - t2 << logs::end;

}

#endif /* CONFIG_MOTORCTRL_BOTMOTORS */
