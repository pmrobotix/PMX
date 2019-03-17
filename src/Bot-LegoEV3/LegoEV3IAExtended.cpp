#include "LegoEV3IAExtended.hpp"

#include <src/pmr_playground.h>

class Robot;

LegoEV3IAExtended::LegoEV3IAExtended(std::string botId, Robot *robot) :
        iaz_(robot), iap_(robot)
{
    p_ = new Playground(0.0, 0.0, 3000.0, 2000.0, 1.0, 1.0);
    initPlayground();

}

LegoEV3IAExtended::~LegoEV3IAExtended()
{
}

void LegoEV3IAExtended::initPlayground()
{
    //bordure terrain
    p_->add_rectangle(1500, 0, 3000, 140, 0); //bottom
    p_->add_rectangle(1500, 2000, 3000, 140, 0); //top
    p_->add_rectangle(0, 1000, 140, 2000, 0); //left
    p_->add_rectangle(3000, 1000, 140, 2000, 0); //right

    p_->compute_edges();

    iap_.addPlayground(p_);
    iap_.toSVG();

}

