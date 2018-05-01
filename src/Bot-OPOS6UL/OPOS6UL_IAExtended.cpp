#include "OPOS6UL_IAExtended.hpp"

class Robot;

OPOS6UL_IAExtended::OPOS6UL_IAExtended(std::string botId, Robot *robot) :
        iaz_(robot), iap_(robot)
{
    opponent_1 = Playground::INVALID;
    opponent_2 = Playground::INVALID;
    oarea_cube1 = Playground::INVALID;
    oarea_cube2high = Playground::INVALID;
    oarea_cube3left = Playground::INVALID;
    oarea_distribadverse = Playground::INVALID;
    garea_cube1 = Playground::INVALID;
    garea_cube2high = Playground::INVALID;
    garea_cube3left = Playground::INVALID;
    garea_distribadverse = Playground::INVALID;
    area_stations = Playground::INVALID;
}

OPOS6UL_IAExtended::~OPOS6UL_IAExtended()
{
}
/*
 void OPOS6UL_IAExtended::initPlayground()
 {
 PlaygroundObjectID advervary = Playground::INVALID;

 PlaygroundObjectID oarea_cube1 = Playground::INVALID;
 PlaygroundObjectID oarea_cube2high = Playground::INVALID;
 PlaygroundObjectID oarea_cube3left = Playground::INVALID;
 PlaygroundObjectID oarea_distribadverse = Playground::INVALID;
 PlaygroundObjectID garea_cube1 = Playground::INVALID;
 PlaygroundObjectID garea_cube2high = Playground::INVALID;
 PlaygroundObjectID garea_cube3left = Playground::INVALID;
 PlaygroundObjectID garea_distribadverse = Playground::INVALID;
 PlaygroundObjectID area_stations = Playground::INVALID;

 //orange parts
 //cubes
 p_->add_circle(oarea_cube1, 850.0, 530.0, 300.0, 6);
 p_->add_circle(oarea_cube2high, 1100.0, 1500.0, 300.0, 6);
 p_->add_circle(oarea_cube3left, 300.0, 1200.0, 300.0, 6);

 //distributeurs
 p_->add_rectangle(oarea_distribadverse, 600, 1900, 200.0, 200.0, 0);

 //stations d'épuration
 p_->add_rectangle(area_stations, 1500.0, 1850.0, 1400.0, 300.0, 0);

 //Green parts
 //cubes
 p_->add_circle(garea_cube1, 2150.0, 530.0, 300.0, 6);
 p_->add_circle(garea_cube2high, 1900.0, 1500.0, 300.0, 6);
 p_->add_circle(garea_cube3left, 2700.0, 1200.0, 300.0, 6);

 //distributeurs
 p_->add_rectangle(garea_distribadverse, 2400, 1900, 200.0, 200.0, 0);

 p_->compute_edges();

 }*/
/*
 void OPOS6UL_IAExtended::playgroundComputeEdges()
 {
 p_->compute_edges();
 }*/

