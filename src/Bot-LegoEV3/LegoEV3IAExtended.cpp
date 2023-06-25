#include "LegoEV3IAExtended.hpp"

#include <src/pmr_playground.h>

class Robot;

LegoEV3IAExtended::LegoEV3IAExtended(std::string botId, Robot *robot) :
        iaz_(robot), iap_(robot)
{
    //robot_ = robot;
    opponent_1 = Playground::INVALID;
    opponent_2 = Playground::INVALID;
    opponent_3 = Playground::INVALID;
    opponent_4 = Playground::INVALID;

    area_B4 = Playground::INVALID;
    area_C4 = Playground::INVALID;
    area_B3 = Playground::INVALID;

    //terrain horizontal
    //p_ = new SymmetricalPlayground(0.0, 0.0, 3400.0, 2500.0, 0.5, 1.0, 1500.0);
    //terrain vertical
    p_ = new SymmetricalPlayground(0.0, 0.0, 2400.0, 3200.0, 0.5, 1.0, 1000.0);

    initPlayground();
}

void LegoEV3IAExtended::initPlayground()
{
    //bordure terrain horizontal
    //    p_->add_rectangle_lower_left(0, 0, 129, 2000, 0); //cote gauche
    //    p_->add_rectangle_lower_left(3000, 0, -129, 2000, 0); //cote droit
    //    p_->add_rectangle_lower_left(0, 0, 3000, 129, 0); //bas
    //    p_->add_rectangle_lower_left(0, 2000 - 129, 3000, 2000, 0); //haut
    //bordure terrain vertical
//    p_->add_rectangle_lower_left(0, 0, 129, 3000, 0); //cote gauche
//    p_->add_rectangle_lower_left(2000, 0, -129, 3000, 0); //cote droit
//    p_->add_rectangle_lower_left(0, 0, 2000, 129, 0); //bas
//    p_->add_rectangle_lower_left(0, 3000 - 129, 2000, 3000, 0); //haut

    //bordure terrain lego
    //    p_->add_rectangle(1500, 0, 3000, 140, 0); //bottom
    //    p_->add_rectangle(1500, 2000, 3000, 140, 0); //top
    //    p_->add_rectangle(0, 1000, 140, 2000, 0); //left
    //    p_->add_rectangle(3000, 1000, 140, 2000, 0); //right
    //bordure terrain vertical
    p_->add_rectangle_lower_left(0, 0, 125, 3000, 0); //cote gauche
    p_->add_rectangle_lower_left(2000, 0, -125, 3000, 0); //cote droit
    p_->add_rectangle_lower_left(0, 0, 2000, 125, 0); //bas
    p_->add_rectangle_lower_left(0, 3000 - 125, 2000, 3000, 0); //haut


//zone green
        //zone bleu adv A3
        p_->add_rectangle_lower_left(area_A3, 0, 800, 600, 650, 0);
        //zone bleu adv D4
        p_->add_rectangle_lower_left(area_D4, 1275, 1550, 750, 650, 0);

        //marron B3
//        p_->add_rectangle(area_B3, 725, 1125, 340, 340, 0);

        //marron B4
//        p_->add_rectangle(area_B4, 725, 1875, 340, 340, 0);

//zone blue
        //zone verte adv D3 (qd on est bleu)
        p_->add_rectangle_lower_left(area_D3, 1400, 800, 600, 650, 0);
        //zone verte adv A4
        p_->add_rectangle_lower_left(area_A4, 0, 1550, 750, 650, 0);

        //marron C3
//        p_->add_rectangle(area_C3, 1275, 1125, 380, 380, 0);

        //marron C4
//        p_->add_rectangle(area_C4, 1275, 1875, 380, 380, 0);


    /*
     //zone depart
     p_->add_rectangle_lower_left_symmetrical(this->area_start_yellow, this->area_start_violet, 0.0, 1000.0, 500.0,
     650.0, 0.0);

     //zone de triangle piedestal
     p_->add_rectangle_symmetrical(this->area_triangle_yellow, this->area_triangle_violet, 200.0, 200.0, 300.0, 750.0,
     M_PI / 4.0);

     //zone aleatoire
     p_->add_rectangle_lower_left_symmetrical(this->area_alea_yellow, this->area_alea_violet, 700.0, 350.0, 550.0, 550.0,
     0);

     //zone hexa depart
     p_->add_rectangle_lower_left_symmetrical(this->area_3_start_yellow, this->area_3_start_violet, 700.0, 1000.0, 400.0,
     540.0, 0.0);

     //distrib
     p_->add_rectangle_symmetrical(this->area_distrib_yellow, this->area_distrib_violet, 100.0, 750.0, 200.0, 300.0,
     0.0);

     //galerie
     p_->add_rectangle_lower_left_symmetrical(this->area_galerie_yellow, this->area_galerie_violet, 400.0, 1800.0,
     1100.0, 200.0, 0.0);
     //barre milieu
     p_->add_rectangle(1500, 1850, 300, 550, 0);
     */

    //TODO opponent ?
    p_->add_circle(this->opponent_1, -200.0, -200.0, 350.0, 8);
    p_->add_circle(this->opponent_2, -200.0, -200.0, 350.0, 8);
    p_->add_circle(this->opponent_3, -200.0, -200.0, 350.0, 8);
    p_->add_circle(this->opponent_4, -200.0, -200.0, 350.0, 8);

    p_->compute_edges();

    iap_.addPlayground(p_);
    iap_.toSVG();

}

