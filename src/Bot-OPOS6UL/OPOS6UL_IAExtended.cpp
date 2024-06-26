#include "OPOS6UL_IAExtended.hpp"

#include <src/pmr_symmetrical_pg.h>

class Robot;

OPOS6UL_IAExtended::OPOS6UL_IAExtended(std::string botId, Robot *robot) :
        iaz_(robot), iap_(robot)
{
    opponent_1 = Playground::INVALID;
//    opponent_2 = Playground::INVALID;
//    opponent_3 = Playground::INVALID;
//    opponent_4 = Playground::INVALID;

    area_B4 = Playground::INVALID;
    area_C4 = Playground::INVALID;
    area_B3 = Playground::INVALID;

    //terrain horizontal
    p_ = new SymmetricalPlayground(0.0, 0.0, 3400.0, 2500.0, 0.5, 1.0, 1500.0);
    //terrain vertical
    //p_ = new SymmetricalPlayground(0.0, 0.0, 2400.0, 3200.0, 0.5, 1.0, 1000.0);

    initPlayground();

}

void OPOS6UL_IAExtended::initPlayground() {
    //bordure terrain horizontal
    p_->add_rectangle_lower_left(0, 0, 129, 2000, 0); //cote gauche
    p_->add_rectangle_lower_left(3000, 0, -129, 2000, 0); //cote droit
    p_->add_rectangle_lower_left(0, 0, 3000, 129, 0); //bas
    p_->add_rectangle_lower_left(0, 2000 - 129, 3000, 2000, 0); //haut
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

//2023

//
////zone pour eviter le petit robot
//    p_->add_rectangle_lower_left(500, 500, 350, 350, 0);
//
//    //zone bleu adv A3
//    p_->add_rectangle_lower_left(area_A3, 0, 800, 558, 650, 0);
//    //zone bleu adv D4
//    p_->add_rectangle_lower_left(1445, 1550, 558, 650, 0);
//
//    //marron C3
//    p_->add_rectangle(1275, 1125, 380, 380, 0);
//    //marron B3
//    p_->add_rectangle(area_B3, 725, 1125, 340, 340, 0);
//
//    //marron C4
//    p_->add_rectangle(area_C4, 1275, 1875, 380, 380, 0);
//
//    //marron B4
//    p_->add_rectangle(this->area_B4, 725, 1875, 340, 340, 0);
//




    //p_->add_rectangle_lower_left_symmetrical(this->area_start_green, this->area_start_blue, 1400.0, 0.0, 650.0, 650.0, 0.0);
   //p_->add_rectangle_lower_left(this->area_start_green, this->area_start_blue, 1400.0, 0.0, 650.0, 650.0, 0.0);

    //p_->enable(this->area_start_green, false);

//2022
    /*
    //zone de triangle piedestal
    p_->add_rectangle_symmetrical(this->area_triangle_yellow, this->area_triangle_violet, 200.0, 200.0, 300.0, 750.0, M_PI / 4.0);

    //zone aleatoire
    p_->add_rectangle_lower_left_symmetrical(this->area_alea_yellow, this->area_alea_violet, 700.0, 350.0, 550.0, 550.0, 0);

    //zone 3 hexagones devant depart
    //p_->add_rectangle_lower_left_symmetrical(this->area_3_start_yellow, this->area_3_start_violet, 700.0, 1000.0, 430.0, 640.0, 0.0);
    p_->add_rectangle_symmetrical(this->area_3_start_yellow, this->area_3_start_violet, 900.0, 1350.0, 400.0, 400.0, M_PI / 4.0);

    //distrib
    p_->add_rectangle_symmetrical(this->area_distrib_yellow, this->area_distrib_violet, 100.0, 750.0, 200.0, 300.0, 0.0);

    //galerie
    p_->add_rectangle_lower_left_symmetrical(this->area_galerie_yellow, this->area_galerie_violet, 400.0, 1800.0, 1100.0, 200.0, 0.0);
    //barre milieu
    p_->add_rectangle(1500, 1850, 260, 550, 0);
*/


    /*
     std::vector<Point*> relative_point_refs = std::vector<Point*>(3);
     Point p1={0,2000};
     Point p2={3000,2000};
     Point p3={3000,0};

     relative_point_refs[0] = &(p1);
     relative_point_refs[1] = &(p2);
     relative_point_refs[2] = &(p3);
     p_->add_convex_body(0.0, 0.0, relative_point_refs, 0);
     */
    /*
     //zone aleatoire
     p_->add_rectangle_lower_left_symmetrical(this->area_alea_yellow, this->area_alea_blue, 1000.0, 0.0, 500.0, 30.0, 0.0);

     //zone depart
     p_->add_circle_symmetrical(this->area_v_vert_gauche_yellow, this->area_v_vert_gauche_blue, 450, 510, 210, 5);
     p_->add_circle_symmetrical(this->area_v_rouge_droite_yellow, this->area_v_rouge_droite_blue, 450, 1080, 200, 4);
     p_->add_circle_symmetrical(this->area_v_vert_droite_zoneNS_yellow, this->area_v_vert_droite_zoneNS_blue, 300, 400, 200, 4);
     p_->add_circle_symmetrical(this->area_v_rouge_gauche_zoneSN_yellow, this->area_v_rouge_gauche_zoneSN_blue, 300, 1200, 180, 4);

     //centre terrain
     //p_->add_circle_symmetrical(this->area_v_vert_devant_yellow, this->area_v_vert_devant_blue, 1100, 800, 200, 5);
     p_->add_circle_symmetrical(this->area_v_rouge_devant_droite_yellow, this->area_v_rouge_devant_droite_blue, 950, 400, 200, 5);
     p_->add_circle_symmetrical(this->area_v_rouge_devant_gauche_milieu_terrain_yellow, this->area_v_rouge_devant_gouche_milieu_terrain_blue, 1270, 1200, 200, 5);

     */

    //opponent
    p_->add_circle(this->opponent_1, -100.0, -100.0, 350.0, 8);
//    p_->add_circle(this->opponent_2, -100.0, -100.0, 350.0, 8);
//    p_->add_circle(this->opponent_3, -100.0, -100.0, 350.0, 8);
//    p_->add_circle(this->opponent_4, -100.0, -100.0, 350.0, 8);
    p_->compute_edges();
    iap_.addPlayground(p_);
    iap_.toSVG();

}
void OPOS6UL_IAExtended::move_adv(float x_mm, float y_mm) {
    //p->move(me, 200.0, 0.0)->synchronize();
    //p_->movexy(this->opponent_1, x_mm, y_mm)->synchronize();
}
