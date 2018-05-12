#include "LegoEV3IAExtended.hpp"

#include <src/pmr_playground.h>

class Robot;

LegoEV3IAExtended::LegoEV3IAExtended(std::string botId, Robot *robot) :
		iaz_(robot), iap_(robot)
{
/*
	p_ = new Playground(0.0, 0.0, 3000.0, 2000.0, 1.0, 1.0);
	initPlayground();
	iap_.toSVG();*/
}

LegoEV3IAExtended::~LegoEV3IAExtended()
{
}
/*
void LegoEV3IAExtended::initPlayground()
{

	PlaygroundObjectID area_start_yellow = Playground::INVALID;
	PlaygroundObjectID area_balls_yellow = Playground::INVALID;
	PlaygroundObjectID area_centre_depose = Playground::INVALID;

	//yellow parts
	p_->add_rectangle(area_start_yellow, 360, 380, 750.0, 80.0, 0);
	p_->add_circle(area_balls_yellow, 650.0, 530.0, 180.0, 8);
	p_->add_rectangle(area_centre_depose, 1500.0, 1400.0, 200.0, 400.0, 0);

	p_->compute_edges();

}

void LegoEV3IAExtended::playgroundComputeEdges()
{
	p_->compute_edges();
}

void LegoEV3IAExtended::playgroundFindPath(FoundPath * & path, Point& start, Point& end)
{
	p_->find_path(path, start, end);
}
*/
