/*
 * IAbyPath.cpp
 *
 *  Created on: 4 mai 2017
 *      Author: pmx
 */

#include "IAbyPath.hpp"

#include <simple_svg_1.0.0.hpp>
#include <src/pmr_path_result.h>
#include <stddef.h>

#include "Playground2017.hpp"

IAbyPath::IAbyPath(Robot *robot)
{
	svg::Dimensions dimensions(100, 100);

	Playground2017 *p = new Playground2017(0.0, 0.0, 3000.0, 2000.0, 5.0, 10.0);

	p->init();


	FoundPath * path = NULL;
	Point startPoint =
	{ x : 250.0, y : 300.0 };
	Point secondPosition =
	{ x : 1700.0, y : 1050.0 };
	Point finalPosition =
	{ x : 2880.0, y : 300.0 };

	p->compute_edges();
	//toSVG(p, path, "testRobot0.svg");

}

