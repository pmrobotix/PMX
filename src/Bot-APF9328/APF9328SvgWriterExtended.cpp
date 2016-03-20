#include <cmath>
#include <sstream>

#include "../Log/Level.hpp"
#include "../Log/Logger.hpp"
#include "APF9328SvgWriterExtended.hpp"

APF9328SvgWriterExtended::APF9328SvgWriterExtended(std::string botId)
		: SvgWriter(botId) //on appelle le constructeur pere
{

	std::ostringstream symbol;

	symbol << "<symbol id=\"bot-apf9328\">"
			<< "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"blue\" />"
			<< "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"blue\" />"
			<< "<rect x=\"0\" y=\"0\" width=\"300\" height=\"300\" style=\"fill:none;stroke:slategray;stroke-width:2px;\" />"
			<< "<line x1=\"0\" y1=\"30\" x2=\"0\" y2=\"300\" stroke=\"red\" stroke-width=\"4\"/>"
			//arriere du robot en position 0,0
			<< "<line x1=\"0\" y1=\"30\" x2=\"50\" y2=\"0\" stroke=\"red\" stroke-width=\"4\"/>"
			<< "<line x1=\"0\" y1=\"270\" x2=\"50\" y2=\"300\" stroke=\"red\" stroke-width=\"4\"/>"
			<< "<line x1=\"300\" y1=\"0\" x2=\"300\" y2=\"300\" stroke=\"blue\" stroke-width=\"4\"/>"
			//devant du robot en position 300,0
			<< "</symbol>";
	addDefsSymbol(symbol.str());

}

//angle en radian
void APF9328SvgWriterExtended::writePosition(double x, double y, double angle, APF9328SvgSymbol svgcolor)
{
	if (logger().isActive(logs::Level::INFO))
	{
		double delta_y = 0.0;
		double delta_x = 0.0;

		switch (svgcolor)
		{

		case APF9328_SVG_POS_ROBOT:
			logger().info() << "<use x=\""
					<< x - 150
					<< "\" y=\""
					<< -y - 150
					<< "\" xlink:href=\"#bot-apf9328\" transform=\"rotate("
					<< -((angle * 180) / M_PI)
					<< ","
					<< x
					<< ","
					<< -y
					<< ")\" />"
					<< logs::end;
			break;
		case APF9328_SVG_POS_FOLLOWING:
		default:
			// inversion du y pour affichage dans le bon sens dans le SVG
			logger().info() << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"1\" fill=\"blue\" />" << logs::end;
			delta_y = 25.0 * sin(angle);
			delta_x = 25.0 * cos(angle);
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
