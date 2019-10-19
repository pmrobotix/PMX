#include "OPOS6UL_SvgWriterExtended.hpp"

#include <cmath>
#include <sstream>

#include "../Log/Logger.hpp"

OPOS6UL_SvgWriterExtended::OPOS6UL_SvgWriterExtended(std::string botId) :
        SvgWriter(botId) //on appelle le constructeur pere
{

    std::ostringstream symbol;

//    symbol << "<symbol id=\"bot-OPOS6UL\">" << "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"slategray\" />"
//            << "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"slategray\" />"
//            << "<rect x=\"0\" y=\"0\" width=\"300\" height=\"300\" style=\"fill:none;stroke:slategray;stroke-width:2px;\" />"
//            //arriere du robot en position 0,0
//            << "<line x1=\"0\" y1=\"30\" x2=\"0\" y2=\"300\" stroke=\"slategray\" stroke-width=\"4\"/>"
//            << "<line x1=\"0\" y1=\"30\" x2=\"50\" y2=\"0\" stroke=\"slategray\" stroke-width=\"4\"/>"
//            << "<line x1=\"0\" y1=\"270\" x2=\"50\" y2=\"300\" stroke=\"slategray\" stroke-width=\"4\"/>"
//            //devant du robot en position 300,0
//            << "<line x1=\"300\" y1=\"0\" x2=\"300\" y2=\"300\" stroke=\"blue\" stroke-width=\"4\"/>"

    symbol << "<symbol id=\"bot-OPOS6UL\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"slategray\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"slategray\" />"

            << "<rect x=\"80\" y=\"0\" width=\"70\" height=\"300\" style=\"fill:none;stroke:slategray;stroke-width:2px;\" />"
            //arriere du robot en position 0,0
            << "<line x1=\"150\" y1=\"0\" x2=\"400\" y2=\"150\" stroke=\"slategray\" stroke-width=\"4\"/>"
            << "<line x1=\"150\" y1=\"300\" x2=\"400\" y2=\"150\" stroke=\"slategray\" stroke-width=\"4\"/>"
            //devant du robot en position 300,0
            << "<line x1=\"400\" y1=\"60\" x2=\"400\" y2=\"240\" stroke=\"blue\" stroke-width=\"4\"/>"
            //<< "<circle cx=\"150\" cy=\"150\" r=\"250\" fill=\"none\" stroke=\"red\" stroke-width=\"1\"/>"
            << "</symbol>";

    symbol << "<symbol id=\"bot-OPOS6UL-RED\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"red\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"red\" />"

            << "<rect x=\"80\" y=\"0\" width=\"70\" height=\"300\" style=\"fill:none;stroke:red;stroke-width:2px;\" />"
            //arriere du robot en position 0,0
            << "<line x1=\"150\" y1=\"0\" x2=\"400\" y2=\"150\" stroke=\"red\" stroke-width=\"4\"/>"
            << "<line x1=\"150\" y1=\"300\" x2=\"400\" y2=\"150\" stroke=\"red\" stroke-width=\"4\"/>"
            //devant du robot en position 300,0
            << "<line x1=\"400\" y1=\"60\" x2=\"400\" y2=\"240\" stroke=\"red\" stroke-width=\"4\"/>"

            << "</symbol>";

    symbol << "<symbol id=\"bot-OPOS6UL-ORANGE\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"orange\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"orange\" />"

            << "<rect x=\"80\" y=\"0\" width=\"70\" height=\"300\" style=\"fill:none;stroke:orange;stroke-width:2px;\" />"
            //arriere du robot en position 0,0
            << "<line x1=\"150\" y1=\"0\" x2=\"400\" y2=\"150\" stroke=\"orange\" stroke-width=\"4\"/>"
            << "<line x1=\"150\" y1=\"300\" x2=\"400\" y2=\"150\" stroke=\"orange\" stroke-width=\"4\"/>"
            //devant du robot en position 300,0
            << "<line x1=\"400\" y1=\"60\" x2=\"400\" y2=\"240\" stroke=\"orange\" stroke-width=\"4\"/>"

            << "</symbol>";
    symbol << "<symbol id=\"bot-OPOS6UL-GREEN\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"green\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"green\" />"

            << "<rect x=\"80\" y=\"0\" width=\"70\" height=\"300\" style=\"fill:none;stroke:green;stroke-width:2px;\" />"
            //arriere du robot en position 0,0
            << "<line x1=\"150\" y1=\"0\" x2=\"400\" y2=\"150\" stroke=\"green\" stroke-width=\"4\"/>"
            << "<line x1=\"150\" y1=\"300\" x2=\"400\" y2=\"150\" stroke=\"green\" stroke-width=\"4\"/>"
            //devant du robot en position 300,0
            << "<line x1=\"400\" y1=\"60\" x2=\"400\" y2=\"240\" stroke=\"green\" stroke-width=\"4\"/>"

            << "</symbol>";
    addDefsSymbol(symbol.str());

    //optimisation du log pour ne créer qu'un seul objet
    fLogBuffer = new logs::Logger::LoggerBuffer(logger(), logs::Level::INFO);
}

//angle en radian
void OPOS6UL_SvgWriterExtended::writePosition_Bot(float x, float y, float angle_rad, int color)
{
    //printf("writePosition_Bot done_=%d\n", done_);

    if (!done_) {
        if (color == 0) {
            *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
                    << "\" xlink:href=\"#bot-OPOS6UL\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << "," << x
                    << "," << -y << ")\" />" << logs::flush;
        } else if (color == 1) {
            *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
                    << "\" xlink:href=\"#bot-OPOS6UL-ORANGE\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        } else if (color == 2) {
            *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
                    << "\" xlink:href=\"#bot-OPOS6UL-RED\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        } else if (color == 3) {
            *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
                    << "\" xlink:href=\"#bot-OPOS6UL-GREEN\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        }
    }
}

void OPOS6UL_SvgWriterExtended::writePosition_BotPos(float x, float y, float angle_rad)
{
    if (!done_) {

        double delta_y = 0.0;
        double delta_x = 0.0;

        // inversion du y pour affichage dans le bon sens dans le SVG
        *fLogBuffer << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"1\" fill=\"blue\" />" << logs::flush;
        delta_y = 50.0 * sin(angle_rad);
        delta_x = 50.0 * cos(angle_rad);
        *fLogBuffer << "<line x1=\"" << x << "\" y1=\"" << -y << "\" x2=\"" << x + delta_x << "\" y2=\"" << -y - delta_y
                << "\" stroke-width=\"0.1\" stroke=\"grey\"  />" << logs::flush;

    }
}
void OPOS6UL_SvgWriterExtended::writeZone(const char *name, float minX, float minY, float width, float height,
        float startX, float startY, float startAngle_rad)
{
    if (!done_) {
        //ia
        *fLogBuffer << "<g transform=\"scale(1,-1) \">" << "<rect x=\"" << minX << "\" y=\"" << minY << "\" width=\""
                << width << "\" height=\"" << height << "\" fill=\"none\" stroke=\"#cc00cc\" stroke-width=\"4\" />"
                << "<line x1 = \"" << minX << "\" y1 = \"" << minY << "\" x2 = \"" << minX + width << "\" y2 = \""
                << minY << "\" stroke=\"#cc00cc\" stroke-width=\"4\"/>"

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

                << "<circle cx='" << startX << "' cy='" << startY << "' r='3' fill='none' stroke='#cc00cc' />"
                << "<line x1 = \"" << startX << "\" y1 = \"" << startY << "\" x2 = \""
                << startX + 25 * cos(startAngle_rad) << "\" y2 = \"" << startY + 25 * sin(startAngle_rad)
                << "\" stroke = \"#cc00cc\" stroke-width = \"4\"/>" << "</g>"

                << "<text x='" << startX + 20 << "' y='" << -startY - 10 << "' font-size='30' fill='#cc00cc'>" << name
                << "</text>" << logs::flush;
    }
}

//display path for IAbyZone
void OPOS6UL_SvgWriterExtended::writeIaPath(const char *zone1Name, const char *zone2Name, float x_mm, float y_mm)
{
    if (!done_) {
        *fLogBuffer << "<circle cx='" << x_mm << "' cy='" << -y_mm << "' r='15' fill='none' stroke='green' />"
                << "<text x='" << x_mm + 20 << "' y='" << -y_mm + 20 << "' font-size='30' fill='green'>" << zone1Name
                << "-" << zone2Name << "</text>" << logs::flush;
    }
}

void OPOS6UL_SvgWriterExtended::pathPolyline(std::string points) //TODO mettre dans SVGWriter ?
{
    if (!done_) {
        *fLogBuffer << "<polyline points='" << points << "' style='fill:none;stroke:red;stroke-width:4' />"
                << logs::flush;
    }
}
