#include "LegoEV3IAExtended.hpp"

#include <src/pmr_playground.h>

class Robot;

LegoEV3IAExtended::LegoEV3IAExtended(std::string botId, Robot *robot) :
        iaz_(robot), iap_(robot)
{
    //p_ = new Playground(0.0, 0.0, 3000.0, 2000.0, 1.0, 1.0);
    p_ = new Playground(0.0, 0.0, 3400.0, 2500.0, 0.5, 1.0);

    initPlayground();

}

void LegoEV3IAExtended::initPlayground()
{
    //bordure terrain
    p_->add_rectangle(1500, 0, 3000, 140, 0); //bottom
    p_->add_rectangle(1500, 2000, 3000, 140, 0); //top
    p_->add_rectangle(0, 1000, 140, 2000, 0); //left
    p_->add_rectangle(3000, 1000, 140, 2000, 0); //right

    //zone aleatoire
    p_->add_circle(this->area_alea_violet, 1000.0, 1050.0, 300.0, 6);
    p_->add_circle(this->area_alea_yellow, 2000.0, 1050.0, 300.0, 6);

    //zone palets depart
    p_->add_rectangle(this->area_palet_start_violet, 500.0, 600.0, 300.0, 1400.0, 0.0);
    p_->add_rectangle(this->area_palet_start_yellow, 3000-500.0, 600.0, 300.0, 1400.0, 0.0);

    /*
     //orange parts
     //cubes
     p_->add_circle(robot.ia().oarea_cube1, 850.0, 530.0, 300.0, 6);
     p_->add_circle(robot.ia().oarea_cube2high, 1100.0, 1500.0, 300.0, 6);
     p_->add_circle(robot.ia().oarea_cube3left, 300.0, 1200.0, 250.0, 6);

     //distributeurs
     p_->add_rectangle(robot.ia().oarea_distribadverse, 600, 1900, 200.0, 200.0, 0);

     //zone de construction
     p_->add_rectangle(robot.ia().oarea_buildzone, 650, 180, 500.0, 220.0, 0);
     if (robot.getMyColor() == PMXVIOLET)
     p_->enable(robot.ia().oarea_buildzone, 0);

     //Green parts
     //cubes
     p_->add_circle(robot.ia().garea_cube1, 2150.0, 530.0, 300.0, 6);
     p_->add_circle(robot.ia().garea_cube2high, 1900.0, 1500.0, 300.0, 6);
     p_->add_circle(robot.ia().garea_cube3left, 2700.0, 1200.0, 250.0, 6);

     //distributeurs
     p_->add_rectangle(robot.ia().garea_distribadverse, 2400, 1900, 200.0, 200.0, 0);

     //zone de construction
     p_->add_rectangle(robot.ia().garea_buildzone, 2350, 180, 500.0, 220.0, 0);
     if (robot.getMyColor() != PMXVIOLET)
     p_->enable(robot.ia().garea_buildzone, 0);

     //stations d'épuration
     p_->add_rectangle(robot.ia().area_stations, 1500.0, 1850.0, 1400.0, 300.0, 0);
     //p_->add_circle(robot.ia().opponent_1, 1200.0, 1200.0, 100.0, 6);

     p_->compute_edges();

     robot.ia().iAbyPath().addPlayground(p_);
     robot.ia().iAbyPath().toSVG();
     */
    p_->compute_edges();

    iap_.addPlayground(p_);
    iap_.toSVG();

}

