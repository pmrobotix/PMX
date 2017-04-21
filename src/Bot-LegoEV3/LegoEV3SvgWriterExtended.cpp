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
			<< "<circle cx=\"90\" cy=\"90\" r=\"90\" fill=\"none\" stroke=\"orange\" />"
			<< "<circle cx=\"90\" cy=\"90\" r=\"10\" fill=\"none\" stroke=\"orange\" />"
			<< "<rect x=\"0\" y=\"0\" width=\"180\" height=\"180\" style=\"fill:none;stroke:orange;stroke-width:2px;\" />"
			<< "<line x1=\"0\" y1=\"30\" x2=\"0\" y2=\"180\" stroke=\"orange\" stroke-width=\"4\"/>"
			//arriere du robot en position 0,0
			<< "<line x1=\"0\" y1=\"30\" x2=\"50\" y2=\"0\" stroke=\"orange\" stroke-width=\"4\"/>"
			<< "<line x1=\"0\" y1=\"150\" x2=\"50\" y2=\"180\" stroke=\"orange\" stroke-width=\"4\"/>"
			<< "<line x1=\"180\" y1=\"0\" x2=\"180\" y2=\"180\" stroke=\"red\" stroke-width=\"4\"/>"
			//devant du robot en position 180,0
			<< "</symbol>";
	addDefsSymbol(symbol.str());

}

//angle en radian
void LegoEV3SvgWriterExtended::writePosition(double x,
		double y,
		double angle_rad,
		std::string symbol)
{
	if (!done_)
	if (logger().isActive(logs::Level::INFO))
	{
		double delta_y = 0.0;
		double delta_x = 0.0;

		if (symbol == "bot")
		{
			logger().info() << "<use x=\""
					<< x - 90
					<< "\" y=\""
					<< -y - 90
					<< "\" xlink:href=\"#bot-legoev3\" transform=\"rotate("
					<< -((angle_rad * 180) / M_PI)
					<< ","
					<< x
					<< ","
					<< -y
					<< ")\" />"
					<< logs::end;
		}
		else if (symbol == "bot-pos")
		{
			// inversion du y pour affichage dans le bon sens dans le SVG
			logger().info() << "<circle cx=\""
					<< x
					<< "\" cy=\""
					<< -y
					<< "\" r=\"1\" fill=\"blue\" />"
					<< logs::end;
			delta_y = 50.0 * sin(angle_rad);
			delta_x = 50.0 * cos(angle_rad);
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
		}
		else
		{
			//TODO Error
		}
	}
}

void LegoEV3SvgWriterExtended::writeZone(const char* name,
		float minX,
		float minY,
		float width,
		float height,
		float startX,
		float startY,
		float startAngle_rad)
{
	if (!done_)
	//ia
	logger().info() << "<g transform=\"scale(1,-1) \">"
			<< "<rect x=\""
			<< minX
			<< "\" y=\""
			<< minY
			<< "\" width=\""
			<< width
			<< "\" height=\""
			<< height
			<< "\" fill=\"none\" stroke=\"#33cc33\" stroke-width=\"4\" />"
			<< "<line x1 = \""
			<< minX
			<< "\" y1 = \""
			<< minY
			<< "\" x2 = \""
			<< minX + width
			<< "\" y2 = \""
			<< minY
			<< "\" stroke=\"#33cc33\" stroke-width=\"4\"/>"

//			<< "<line x1 = \""
//			<< minX + width
//			<< "\" y1 = \""
//			<< minY
//			<< "\" x2 = \""
//			<< minX + width
//			<< "\" y2 = \""
//			<< minY + height
//			<< "\" stroke=\"blue\" stroke-width=\"4\"/>"
//
//			<< "<line x1 = \""
//			<< minX + width
//			<< "\" y1 = \""
//			<< minY + height
//			<< "\" x2 = \""
//			<< minX
//			<< "\" y2 = \""
//			<< minY + height
//			<< "\" stroke=\"blue\" stroke-width=\"4\"/>"
//
//			<< "<line x1 = \""
//			<< minX
//			<< "\" y1 = \""
//			<< minY + height
//			<< "\" x2 = \""
//			<< minX
//			<< "\" y2 = \""
//			<< minY
//			<< "\" stroke=\"blue\" stroke-width=\"4\"/>"

			<< "<circle cx='"
			<< startX
			<< "' cy='"
			<< startY
			<< "' r='3' fill='none' stroke='#33cc33' />"
			<< "<line x1 = \""
			<< startX
			<< "\" y1 = \""
			<< startY
			<< "\" x2 = \""
			<< startX + 25 * cos(startAngle_rad)
			<< "\" y2 = \""
			<< startY + 25 * sin(startAngle_rad)
			<< "\" stroke = \"#33cc33\" stroke-width = \"4\"/>"
			<< "</g>"

			<< "<text x='"
			<< startX + 20
			<< "' y='"
			<< -startY - 10
			<< "' font-size='30' fill='#33cc33'>"
			<< name
			<< "</text>"
			<< logs::end;

}

void LegoEV3SvgWriterExtended::writeIaPath(const char* zone1Name,
		const char* zone2Name,
		float x_mm,
		float y_mm)
{
	if (!done_)
	logger().info()
			<< "<circle cx='"
			<< x_mm
			<< "' cy='"
			<< -y_mm
			<< "' r='15' fill='none' stroke='green' />"
			<< "<text x='"
			<< x_mm + 20
			<< "' y='"
			<< -y_mm + 20
			<< "' font-size='30' fill='green'>"
			<< zone1Name
			<< "-"
			<< zone2Name
			<< "</text>"
			<< logs::end;
}

