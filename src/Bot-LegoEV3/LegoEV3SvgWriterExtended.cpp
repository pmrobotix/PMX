#include "LegoEV3SvgWriterExtended.hpp"

#include <cmath>
#include <sstream>

#include "../Log/Level.hpp"
#include "../Log/Logger.hpp"

LegoEV3SvgWriterExtended::LegoEV3SvgWriterExtended(std::string botId)
		: SvgWriter(botId) //on appelle le constructeur pere
{

	std::ostringstream symbol;

	symbol << "<symbol id=\"bot-legoev3\">"
			<< "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"green\" />"
			<< "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"green\" />"
			<< "<rect x=\"0\" y=\"0\" width=\"300\" height=\"300\" style=\"fill:none;stroke:slategray;stroke-width:2px;\" />"
			<< "<line x1=\"0\" y1=\"30\" x2=\"0\" y2=\"300\" stroke=\"red\" stroke-width=\"4\"/>"
			//arriere du robot en position 0,0
			<< "<line x1=\"0\" y1=\"30\" x2=\"50\" y2=\"0\" stroke=\"red\" stroke-width=\"4\"/>"
			<< "<line x1=\"0\" y1=\"270\" x2=\"50\" y2=\"300\" stroke=\"red\" stroke-width=\"4\"/>"
			<< "<line x1=\"300\" y1=\"0\" x2=\"300\" y2=\"300\" stroke=\"green\" stroke-width=\"4\"/>"
			//devant du robot en position 300,0
			<< "</symbol>";
	addDefsSymbol(symbol.str());

}

//angle en radian
void LegoEV3SvgWriterExtended::writePosition(double x, double y, double angle_rad, LegoEv3SvgSymbol svgcolor)
{
	if (logger().isActive(logs::Level::INFO))
	{
		double delta_y = 0.0;
		double delta_x = 0.0;

		switch (svgcolor)
		{

		case LEGOEV3_SVG_POS_ROBOT:
			logger().info() << "<use x=\""
					<< x - 150
					<< "\" y=\""
					<< -y - 150
					<< "\" xlink:href=\"#bot-legoev3\" transform=\"rotate("
					<< -((angle_rad * 180) / M_PI)
					<< ","
					<< x
					<< ","
					<< -y
					<< ")\" />"
					<< logs::end;
			break;
		case LEGOEV3_SVG_POS_FOLLOWING:
		default:
			// inversion du y pour affichage dans le bon sens dans le SVG
			logger().info() << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"1\" fill=\"green\" />" << logs::end;
			delta_y = 25.0 * sin(angle_rad);
			delta_x = 25.0 * cos(angle_rad);
			logger().info() << "<line x1=\""
					<< x
					<< "\" y1=\""
					<< -y
					<< "\" x2=\""
					<< x + delta_x
					<< "\" y2=\""
					<< -y - delta_y
					<< "\" stroke-width=\"0.1\" stroke=\"grey\"  />"
					<< logs::end;
			break;
		}
	}
}
