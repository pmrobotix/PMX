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
			<< "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"slategray\" />"
			<< "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"slategray\" />"
			<< "<rect x=\"0\" y=\"0\" width=\"300\" height=\"300\" style=\"fill:none;stroke:slategray;stroke-width:2px;\" />"
			<< "<line x1=\"0\" y1=\"30\" x2=\"0\" y2=\"300\" stroke=\"slategray\" stroke-width=\"4\"/>"
			//arriere du robot en position 0,0
			<< "<line x1=\"0\" y1=\"30\" x2=\"50\" y2=\"0\" stroke=\"slategray\" stroke-width=\"4\"/>"
			<< "<line x1=\"0\" y1=\"270\" x2=\"50\" y2=\"300\" stroke=\"slategray\" stroke-width=\"4\"/>"
			<< "<line x1=\"300\" y1=\"0\" x2=\"300\" y2=\"300\" stroke=\"blue\" stroke-width=\"4\"/>"
			//devant du robot en position 300,0
			<< "</symbol>";
	addDefsSymbol(symbol.str());

}

//angle en radian
void APF9328SvgWriterExtended::writePosition(double x,
		double y,
		double angle_rad,
		std::string symbol)
{
	if (logger().isActive(logs::Level::INFO))
	{
		double delta_y = 0.0;
		double delta_x = 0.0;

		if (symbol == "bot")
		{
			logger().info() << "<use x=\""
					<< x - 150
					<< "\" y=\""
					<< -y - 150
					<< "\" xlink:href=\"#bot-apf9328\" transform=\"rotate("
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

void APF9328SvgWriterExtended::writeZone(const char* name,
		float minX,
		float minY,
		float width,
		float height,
		float startX,
		float startY,
		float startAngle_rad)
{
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
			<< "\" fill=\"none\" stroke=\"#cc00cc\" stroke-width=\"4\" />"
			<< "<line x1 = \""
			<< minX
			<< "\" y1 = \""
			<< minY
			<< "\" x2 = \""
			<< minX + width
			<< "\" y2 = \""
			<< minY
			<< "\" stroke=\"#cc00cc\" stroke-width=\"4\"/>"

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
			<< "' r='3' fill='none' stroke='#cc00cc' />"
			<< "<line x1 = \""
			<< startX
			<< "\" y1 = \""
			<< startY
			<< "\" x2 = \""
			<< startX + 25 * cos(startAngle_rad)
			<< "\" y2 = \""
			<< startY + 25 * sin(startAngle_rad)
			<< "\" stroke = \"#cc00cc\" stroke-width = \"4\"/>"
			<< "</g>"

			<< "<text x='"
			<< startX + 20
			<< "' y='"
			<< -startY - 10
			<< "' font-size='30' fill='#cc00cc'>"
			<< name
			<< "</text>"
			<< logs::end;

}

void APF9328SvgWriterExtended::writeIaPath(const char* zone1Name,
		const char* zone2Name,
		float x_mm,
		float y_mm)
{

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
