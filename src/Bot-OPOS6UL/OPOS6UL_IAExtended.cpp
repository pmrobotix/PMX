#include "OPOS6UL_IAExtended.hpp"

#include <src/pmr_symmetrical_pg.h>

class Robot;

OPOS6UL_IAExtended::OPOS6UL_IAExtended(std::string botId, Robot *robot) :
        iaz_(robot), iap_(robot)
{
    opponent_1 = Playground::INVALID;
    //opponent_2 = Playground::INVALID;

    area_alea_violet = Playground::INVALID;
    area_alea_yellow = Playground::INVALID;
    area_palet_start_violet = Playground::INVALID;
    area_palet_start_yellow = Playground::INVALID;
    area_palet_start2_violet = Playground::INVALID;
    area_palet_start2_yellow = Playground::INVALID;

    p_ = new SymmetricalPlayground(0.0, 0.0, 3400.0, 2500.0, 0.5, 1.0, 1500.0);

    initPlayground();

}

void OPOS6UL_IAExtended::initPlayground()
{
    //bordure terrain
    p_->add_rectangle_lower_left(0, 0, 140, 2000, 0);
    p_->add_rectangle_lower_left(3000, 0, -140, 2000, 0);
    p_->add_rectangle_lower_left(0, 0, 3000, 140, 0);
    p_->add_rectangle_lower_left(350, 1450, 2300, 550, 0);
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
    //zone aleatoire
    p_->add_circle_symmetrical(this->area_alea_violet, this->area_alea_yellow, 1000.0, 1050.0, 300.0, 6);

    //zone palets depart
    p_->add_circle_symmetrical(this->area_palet_start_violet, this->area_palet_start_yellow, 500, 750, 180, 8);
    p_->add_circle_symmetrical(this->area_palet_start2_violet, this->area_palet_start2_yellow, 500, 1050, 180, 8);

    //opponent
    p_->add_circle(this->opponent_1, -100.0, -100.0, 350.0, 8);

    p_->compute_edges();
    iap_.addPlayground(p_);
    iap_.toSVG();

}
void OPOS6UL_IAExtended::move_adv(float x_mm, float y_mm)
{
    //p->move(me, 200.0, 0.0)->synchronize();
    //p_->movexy(this->opponent_1, x_mm, y_mm)->synchronize();
}
