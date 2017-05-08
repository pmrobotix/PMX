/*
 * Playground2017.hpp
 *
 *  Created on: 4 mai 2017
 *      Author: pmx
 */

#ifndef COMMON_IA_PLAYGROUND2017_HPP_
#define COMMON_IA_PLAYGROUND2017_HPP_

#include <src/pmr_playground.h>

class Playground2017: public Playground
{
protected:
	PlaygroundObjectID test = Playground::INVALID;

	PlaygroundObjectID teammate = Playground::INVALID;
	PlaygroundObjectID opponent_1 = Playground::INVALID;
	PlaygroundObjectID opponent_2 = Playground::INVALID;

public:

	Playground2017(float field_x1, float field_y1, float field_x2, float field_y2,
			float zone_escape_increment, float zone_escape_max_increment);

	~Playground2017()
	{

	}
	void init();
};

#endif /* COMMON_IA_PLAYGROUND2017_HPP_ */
