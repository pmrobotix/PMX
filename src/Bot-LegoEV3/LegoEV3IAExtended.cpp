#include "LegoEV3IAExtended.hpp"

#include <src/pmr_playground.h>

class Robot;

LegoEV3IAExtended::LegoEV3IAExtended(std::string botId, Robot *robot) :
        iaz_(robot), iap_(robot)
{
    //p_ = new Playground(0.0, 0.0, 3000.0, 2000.0, 1.0, 1.0);
    p_ = new SymmetricalPlayground(0.0, 0.0, 3400.0, 2500.0, 0.5, 1.0, 1500.0);

    initPlayground();

}

void LegoEV3IAExtended::initPlayground() {

    //bordure terrain
    p_->add_rectangle(1500, 0, 3000, 140, 0); //bottom
    p_->add_rectangle(1500, 2000, 3000, 40, 0); //top
    p_->add_rectangle(0, 1000, 140, 2000, 0); //left
    p_->add_rectangle(3000, 1000, 140, 2000, 0); //right

    //zone depart
    p_->add_rectangle_lower_left_symmetrical(this->area_start_yellow, this->area_start_violet, 0.0, 1000.0, 500.0, 650.0, 0.0);

    //zone de triangle piedestal
    p_->add_rectangle_symmetrical(this->area_triangle_yellow, this->area_triangle_violet, 200.0, 200.0, 300.0, 750.0, M_PI / 4.0);

    //zone aleatoire
    p_->add_rectangle_lower_left_symmetrical(this->area_alea_yellow, this->area_alea_violet, 700.0, 350.0, 550.0, 550.0, 0);

    //zone hexa depart
    p_->add_rectangle_lower_left_symmetrical(this->area_3_start_yellow, this->area_3_start_violet, 700.0, 1000.0, 400.0, 540.0, 0.0);

    //distrib
    p_->add_rectangle_symmetrical(this->area_distrib_yellow, this->area_distrib_violet, 100.0, 750.0, 200.0, 300.0, 0.0);

    //galerie
    p_->add_rectangle_lower_left_symmetrical(this->area_galerie_yellow, this->area_galerie_violet, 400.0, 1800.0, 1100.0, 200.0, 0.0);
    //barre milieu
    p_->add_rectangle(1500, 1850, 300, 550, 0);

    //TODO opponent ?

    p_->compute_edges();

    iap_.addPlayground(p_);
    iap_.toSVG();

}

