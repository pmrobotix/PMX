/*
 * Playground2017.cpp
 *
 *  Created on: 5 mai 2017
 *      Author: pmx
 */

#include "Playground2017.hpp"

Playground2017::Playground2017(float field_x1, float field_y1, float field_x2, float field_y2,
		float zone_escape_increment, float zone_escape_max_increment) :
		Playground(field_x1, field_y1, field_x2, field_y2, zone_escape_increment,
				zone_escape_max_increment) //on appelle le constructeur pere
{

}

void Playground2017::init()
{
	add_circle(test, 400.0, 525.0, 180.0, 8);
	add_circle(teammate, 400.0, 1575.0, 180.0, 8);
	add_circle(opponent_1, 2600.0, 525.0, 180.0, 8);
	add_circle(opponent_2, 2600.0, 1575.0, 180.0, 8);
	add_rectangle(1500.0, 1050.0, 200.0, 400.0, 0);
}
