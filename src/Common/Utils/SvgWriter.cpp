/*!
 * \file
 * \brief Implémentation de la classe SvgWriter.
 */

#include "SvgWriter.hpp"
#include <cmath>
#include "../../Log/Level.hpp"
#include "../../Log/Logger.hpp"

SvgWriter::SvgWriter()
{
}

SvgWriter::~SvgWriter()
{
}

void SvgWriter::writePosition(double x, double y, double angle, SvgPositionColor svgcolor)
{
	if (svg().isActive(logs::Level::INFO))
	{
		double delta_y = 0.0;
		double delta_x = 0.0;
		double delta_y2 = 0.0;
		double delta_x2 = 0.0;

		double delta_by1 = 0.0;
		double delta_bx1 = 0.0;
		double delta_by2 = 0.0;
		double delta_bx2 = 0.0;

		switch (svgcolor)
		{
		case SVG_POS_END:
			svg().info() << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"4\" stroke=\"blue\" fill=\"none\"  />"
					<< logs::end;
			break;
		case SVG_POS_RED:
			svg().info() << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"3\" stroke=\"red\" fill=\"none\"  />"
					<< logs::end;
			break;
		case SVG_POS_YELLOW:
			svg().info() << "<circle cx=\"" << x << "\" cy=\"" << -y - 6
					<< "\" r=\"1\" stroke=\"yellow\" fill=\"none\"  />" << logs::end;
			break;
		case SVG_POS_BLACK:
			// inversion du y pour affichage dans le bon sens dans le SVG
			svg().info() << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"2\" stroke=\"black\" fill=\"none\"  />"
					<< logs::end;
			//std::ostringstream log;
			//log << "x:"<< x << " y:" << y << " a:" << angle;
			svg().info() << "<text x='" << x + 5 << "' y='" << -y << "' font-size='5' fill='black'>" << "x:" << x
					<< " y:" << y << " a:" << (angle * 180) / M_PI << "°</text>" << logs::end;
			break;
		case SVG_POS_BLUE:
			svg().info() << "<circle cx=\"" << x << "\" cy=\"" << -y - 3 << "\" r=\"1\" fill=\"blue\" />" << logs::end;
			break;
		case SVG_POS_ANGLE:
			delta_y = -100.0 * sin(angle);
			delta_x = -100.0 * cos(angle);
			svg().info() << "<line x1=\"" << x << "\" y1=\"" << -y << "\" x2=\"" << x + delta_x << "\" y2=\""
					<< -y - delta_y << "\" stroke-width=\"0.1\" stroke=\"grey\"  />" << logs::end;
			svg().info() << "<circle cx=\"" << x + delta_x << "\" cy=\"" << -y - delta_y << "\" r=\"1\" fill=\"red\" />"
					<< logs::end;
			break;
		case SVG_POS_ROBOT:
			delta_by1 = -185.0 * sin(angle + (M_PI / 4.0)); //ligne arriere du robot
			delta_bx1 = -185.0 * cos(angle + (M_PI / 4.0));
			delta_by2 = -185.0 * sin(angle - (M_PI / 4.0));
			delta_bx2 = -185.0 * cos(angle - (M_PI / 4.0));

			delta_y = 185.0 * sin(angle + (M_PI * 36 / 180.0)); //ligne devant le robot
			delta_x = 185.0 * cos(angle + (M_PI * 36 / 180.0));
			delta_y2 = 185.0 * sin(angle - (M_PI * 36 / 180.0));
			delta_x2 = 185.0 * cos(angle - (M_PI * 36 / 180.0));

			svg().info() << "<use x=\"0\" y=\"0\" xlink:href=\"#robot-pmx\" transform=\"   translate(" << x - 190 << ","
					<< -y - 190 << ")  \"/>"
					<< "<line x1=\"" << x + delta_bx2 << "\" y1=\"" << -y - delta_by2 << "\" x2=\"" << x + delta_bx1	<< "\" y2=\"" << -y - delta_by1 << "\" stroke-width=\"1\" stroke=\"red\"  />"
					<< "<line x1=\"" << x + delta_x2 << "\" y1=\"" << -y - delta_y2 << "\" x2=\"" << x + delta_x	<< "\" y2=\"" << -y - delta_y << "\" stroke-width=\"1\" stroke=\"green\"  />"
					//<< "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"5\" fill=\"none\" stroke=\"red\"/>"
					<< logs::end;
			break;
		case SVG_POS_GREEN:
		default:
			// inversion du y pour affichage dans le bon sens dans le SVG
			svg().info() << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"1\" fill=\"green\" />" << logs::end;
			delta_y = 25.0 * sin(angle);
			delta_x = 25.0 * cos(angle);
			svg().info() << "<line x1=\"" << x << "\" y1=\"" << -y << "\" x2=\"" << x + delta_x << "\" y2=\""
					<< -y - delta_y << "\" stroke-width=\"0.1\" stroke=\"grey\"  />" << logs::end;
			break;
		}
	}
}

void SvgWriter::writeText(double x, double y, std::string text)
{
	if (svg().isActive(logs::Level::INFO))
	{
		// inversion du y pour affichage dans le bon sens dans le SVG
		svg().info() << "<text x='" << x << "' y='" << -y << "' font-size='5' fill='black'>" << text << "</text>"
				<< logs::end;
	}
}

void SvgWriter::writeTextCustom(double x, double y, std::string text, std::string color, std::string fontsize)
{
	if (svg().isActive(logs::Level::INFO))
	{
		// inversion du y pour affichage dans le bon sens dans le SVG
		svg().info() << "<text x='" << x << "' y='" << -y << "' font-size='" << fontsize << "' fill='" << color << "'>"
				<< text << "</text>" << logs::end;
	}
}

void SvgWriter::writeZone(const char* name, float minX, float minY, float width,
		float height, float startX, float startY, float startAngle ) //angle en radian
{
	if (svg().isActive(logs::Level::INFO))
	{
		svg().info() << "</g>"<< logs::end;
		//symbols
		svg().info() <<
			"<defs><symbol id=\"iaZones"<< name << "\" ><rect x=\""<< minX << "\" y=\""<< minY <<"\" width=\""<< width<<"\" height=\""<< height <<"\" fill=\"none\" stroke=\"blue\" stroke-width=\"3\" />"
			"<circle cx='" << startX << "' cy='" << startY << "' r='3' fill='none' stroke='blue' />"
			<< "<line x1 = \""<<startX<<"\" y1 = \""<<startY<<"\" x2 = \""<< startX + 25 * cos(startAngle)<<"\" y2 = \""<< startY + 25 * sin(startAngle)<< "\" stroke = \"blue\" stroke-width = \"3\"/>"
			<< "<text x='" << startX + 30 << "' y='" << startY + 40<< "' font-size='30' fill='blue'>"
							<< name << "</text>"
			<< "</symbol></defs>"
			<< logs::end;

		//en mode jaune puis rouge
		svg().info() <<
			" <g transform=\"translate(200,2200)\">  <use x=\"0\" y=\"0\" xlink:href=\"#iaZones"<< name << "\" /> </g>"
		<< " <g transform=\"translate(200,2200) scale(1,-1) \">  <use x=\"0\" y=\"0\" xlink:href=\"#iaZones"<< name << "\" /> </g>"
		<< logs::end;


		svg().info() << "<g transform=\"translate(200,2200)\">" << logs::end;
	}
}
void SvgWriter::writeIaPath(const char* zone1Name, const char* zone2Name, float x, float y )
{
	if (svg().isActive(logs::Level::INFO))
	{
		svg().info() << "</g>"<< logs::end;
		//symbols
		svg().info() <<
			"<defs><symbol id=\"iaPath"<< zone1Name << "-"  << zone2Name << "\" >"

			<< "<circle cx='" << x << "' cy='" << y << "' r='15' fill='none' stroke='green' />"
			<< "<text x='" << x + 30 << "' y='" << y + 40<< "' font-size='30' fill='green'>"
							<< zone1Name << "-" <<  zone2Name << "</text>"
			<< "</symbol></defs>"
			<< logs::end;

		//en mode jaune puis rouge
		svg().info() <<
			" <g transform=\"translate(200,2200)\">  <use x=\"0\" y=\"0\" xlink:href=\"#iaPath"<< zone1Name << "-"  << zone2Name << "\" /> </g>"
		<< " <g transform=\"translate(200,2200) scale(1,-1) \">  <use x=\"0\" y=\"0\" xlink:href=\"#iaPath"<< zone1Name << "-"  << zone2Name << "\" /> </g>"
		<< logs::end;


		svg().info() << "<g transform=\"translate(200,2200)\">" << logs::end;
	}
}



void SvgWriter::writePawn(double x, double y)
{
	if (svg().isActive(logs::Level::INFO))
	{
		// inversion du y pour affichage dans le bon sens dans le SVG
		svg().info() << "<circle cx='" << x << "' cy='" << -y << "' r='100' fill='green' stroke='yellow' />"
				<< logs::end;
	}
}

