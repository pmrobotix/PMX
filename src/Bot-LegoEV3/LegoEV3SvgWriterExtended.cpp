#include "LegoEV3SvgWriterExtended.hpp"

#include <cmath>
#include <sstream>

#include "../Log/Level.hpp"
#include "../Log/Logger.hpp"

LegoEV3SvgWriterExtended::LegoEV3SvgWriterExtended(std::string botId) :
        SvgWriter(botId) //on appelle le constructeur pere
{

    std::ostringstream symbol;

    symbol << "<symbol id=\"bot-legoev3\">"
            << "<circle cx=\"140\" cy=\"140\" r=\"140\" fill=\"none\" stroke=\"slategray\" stroke-width=\"0.5\" stroke-dasharray=\"4,8\"/>"
            << "<circle cx=\"140\" cy=\"140\" r=\"10\" fill=\"none\" stroke=\"slategray\" />"
            << "<line x1=\"100\" y1=\"15\" x2=\"180\" y2=\"15\" stroke=\"slategray\"/>"
            << "<line x1=\"100\" y1=\"265\" x2=\"180\" y2=\"265\" stroke=\"slategray\"/>"
            << "<line x1=\"15\" y1=\"100\" x2=\"15\" y2=\"180\" stroke=\"slategray\"/>"
            << "<line x1=\"265\" y1=\"100\" x2=\"265\" y2=\"180\" stroke=\"slategray\" stroke-width=\"2\"/>"
            << "<path d=\"M100,15A85,85 0 0,0 15,100\" fill=\"none\" stroke=\"slategray\" ></path>"
            << "<path d=\"M180,15A85,85 0 0,1 265,100\" fill=\"none\" stroke=\"slategray\" ></path>"
            << "<path d=\"M265,180A85,85 0 0,1 180,265\" fill=\"none\" stroke=\"slategray\" ></path>"
            << "<path d=\"M15,180A85,85 0 0,0 100,265\" fill=\"none\" stroke=\"slategray\" ></path>" << "</symbol>";
    symbol << "<symbol id=\"bot-legoev3-RED\">"
            << "<circle cx=\"140\" cy=\"140\" r=\"140\" fill=\"none\" stroke=\"red\" stroke-width=\"0.5\" stroke-dasharray=\"4,8\"/>"
            << "<circle cx=\"140\" cy=\"140\" r=\"8\" fill=\"none\" stroke=\"red\" />"
            << "<line x1=\"100\" y1=\"15\" x2=\"180\" y2=\"15\" stroke=\"red\" stroke-width=\"2\"/>"
            << "<line x1=\"100\" y1=\"265\" x2=\"180\" y2=\"265\" stroke=\"red\" stroke-width=\"2\"/>"
            << "<line x1=\"15\" y1=\"100\" x2=\"15\" y2=\"180\" stroke=\"red\" stroke-width=\"2\"/>"
            << "<line x1=\"265\" y1=\"100\" x2=\"265\" y2=\"180\" stroke=\"red\" stroke-width=\"4\"/>"
            << "<path d=\"M100,15A85,85 0 0,0 15,100\" fill=\"none\" stroke=\"red\" ></path>"
            << "<path d=\"M180,15A85,85 0 0,1 265,100\" fill=\"none\" stroke=\"red\" ></path>"
            << "<path d=\"M265,180A85,85 0 0,1 180,265\" fill=\"none\" stroke=\"red\" ></path>"
            << "<path d=\"M15,180A85,85 0 0,0 100,265\" fill=\"none\" stroke=\"red\" ></path>" << "</symbol>";
    symbol << "<symbol id=\"bot-legoev3-ORANGE\">"
            << "<circle cx=\"140\" cy=\"140\" r=\"140\" fill=\"none\" stroke=\"orange\" stroke-width=\"0.5\" stroke-dasharray=\"4,8\"/>"
            << "<circle cx=\"140\" cy=\"140\" r=\"7\" fill=\"none\" stroke=\"orange\" />"
            << "<line x1=\"100\" y1=\"15\" x2=\"180\" y2=\"15\" stroke=\"orange\" stroke-width=\"2\"/>"
            << "<line x1=\"100\" y1=\"265\" x2=\"180\" y2=\"265\" stroke=\"orange\" stroke-width=\"2\"/>"
            << "<line x1=\"15\" y1=\"100\" x2=\"15\" y2=\"180\" stroke=\"orange\" stroke-width=\"2\"/>"
            << "<line x1=\"265\" y1=\"100\" x2=\"265\" y2=\"180\" stroke=\"orange\" stroke-width=\"4\"/>"
            << "<path d=\"M100,15A85,85 0 0,0 15,100\" fill=\"none\" stroke=\"orange\" ></path>"
            << "<path d=\"M180,15A85,85 0 0,1 265,100\" fill=\"none\" stroke=\"orange\" ></path>"
            << "<path d=\"M265,180A85,85 0 0,1 180,265\" fill=\"none\" stroke=\"orange\" ></path>"
            << "<path d=\"M15,180A85,85 0 0,0 100,265\" fill=\"none\" stroke=\"orange\" ></path>" << "</symbol>";
    symbol << "<symbol id=\"bot-legoev3-GREEN\">"
            << "<circle cx=\"140\" cy=\"140\" r=\"140\" fill=\"none\" stroke=\"green\" stroke-width=\"0.5\" stroke-dasharray=\"4,8\"/>"
            << "<circle cx=\"140\" cy=\"140\" r=\"6\" fill=\"none\" stroke=\"green\" />"
            << "<line x1=\"100\" y1=\"15\" x2=\"180\" y2=\"15\" stroke=\"green\" stroke-width=\"2\"/>"
            << "<line x1=\"100\" y1=\"265\" x2=\"180\" y2=\"265\" stroke=\"green\" stroke-width=\"2\"/>"
            << "<line x1=\"15\" y1=\"100\" x2=\"15\" y2=\"180\" stroke=\"green\" stroke-width=\"2\"/>"
            << "<line x1=\"265\" y1=\"100\" x2=\"265\" y2=\"180\" stroke=\"green\" stroke-width=\"4\"/>"
            << "<path d=\"M100,15A85,85 0 0,0 15,100\" fill=\"none\" stroke=\"green\" ></path>"
            << "<path d=\"M180,15A85,85 0 0,1 265,100\" fill=\"none\" stroke=\"green\" ></path>"
            << "<path d=\"M265,180A85,85 0 0,1 180,265\" fill=\"none\" stroke=\"green\" ></path>"
            << "<path d=\"M15,180A85,85 0 0,0 100,265\" fill=\"none\" stroke=\"green\" ></path>" << "</symbol>";
    symbol << "<symbol id=\"bot-legoev3-BLUE\">"
            << "<circle cx=\"140\" cy=\"140\" r=\"140\" fill=\"none\" stroke=\"blue\" stroke-width=\"0.5\" stroke-dasharray=\"4,8\"/>"
            << "<circle cx=\"140\" cy=\"140\" r=\"5\" fill=\"none\" stroke=\"blue\" />"
            << "<line x1=\"100\" y1=\"15\" x2=\"180\" y2=\"15\" stroke=\"blue\" stroke-width=\"2\"/>"
            << "<line x1=\"100\" y1=\"265\" x2=\"180\" y2=\"265\" stroke=\"blue\" stroke-width=\"2\"/>"
            << "<line x1=\"15\" y1=\"100\" x2=\"15\" y2=\"180\" stroke=\"blue\" stroke-width=\"2\"/>"
            << "<line x1=\"265\" y1=\"100\" x2=\"265\" y2=\"180\" stroke=\"blue\" stroke-width=\"4\"/>"
            << "<path d=\"M100,15A85,85 0 0,0 15,100\" fill=\"none\" stroke=\"blue\" ></path>"
            << "<path d=\"M180,15A85,85 0 0,1 265,100\" fill=\"none\" stroke=\"blue\" ></path>"
            << "<path d=\"M265,180A85,85 0 0,1 180,265\" fill=\"none\" stroke=\"blue\" ></path>"
            << "<path d=\"M15,180A85,85 0 0,0 100,265\" fill=\"none\" stroke=\"blue\" ></path>" << "</symbol>";
    symbol << "<symbol id=\"bot-legoev3-BLACK\">"
            << "<circle cx=\"140\" cy=\"140\" r=\"140\" fill=\"none\" stroke=\"black\" stroke-width=\"0.5\" stroke-dasharray=\"4,8\"/>"
            << "<circle cx=\"140\" cy=\"140\" r=\"4\" fill=\"none\" stroke=\"black\" />"
            << "<line x1=\"100\" y1=\"15\" x2=\"180\" y2=\"15\" stroke=\"black\" stroke-width=\"2\"/>"
            << "<line x1=\"100\" y1=\"265\" x2=\"180\" y2=\"265\" stroke=\"black\" stroke-width=\"2\"/>"
            << "<line x1=\"15\" y1=\"100\" x2=\"15\" y2=\"180\" stroke=\"black\" stroke-width=\"2\"/>"
            << "<line x1=\"265\" y1=\"100\" x2=\"265\" y2=\"180\" stroke=\"black\" stroke-width=\"4\"/>"
            << "<path d=\"M100,15A85,85 0 0,0 15,100\" fill=\"none\" stroke=\"black\" ></path>"
            << "<path d=\"M180,15A85,85 0 0,1 265,100\" fill=\"none\" stroke=\"black\" ></path>"
            << "<path d=\"M265,180A85,85 0 0,1 180,265\" fill=\"none\" stroke=\"black\" ></path>"
            << "<path d=\"M15,180A85,85 0 0,0 100,265\" fill=\"none\" stroke=\"black\" ></path>" << "</symbol>";

    symbol << "<symbol id=\"botadv-legoev3\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"slategray\" stroke-width=\"0.5\" stroke-dasharray=\"4,8\"/>"
            << "<circle cx=\"140\" cy=\"140\" r=\"10\" fill=\"none\" stroke=\"slategray\" />" << "</symbol>";
    symbol << "<symbol id=\"botadv-legoev3-RED\">"
                << "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"slategray\" stroke-width=\"0.5\" stroke-dasharray=\"4,8\"/>"
                << "<circle cx=\"140\" cy=\"140\" r=\"10\" fill=\"none\" stroke=\"slategray\" />" << "</symbol>";

//<circle cx="140" cy="140" r="140" fill="none" stroke="slategray" stroke-width="0.5" stroke-dasharray="4,8"/>
//<circle cx="140" cy="140" r="4" fill="none" stroke="slategray" />
//<!--rect x="15" y="15" width="250" height="250" style="fill:none;stroke:orange;stroke-width:2px;" /-->
//<line x1="100" y1="15" x2="180" y2="15" stroke="slategray"/>
//<line x1="100" y1="265" x2="180" y2="265" stroke="slategray"/>
//
//<line x1="15" y1="100" x2="15" y2="180" stroke="slategray"/>
//
//<line x1="265" y1="100" x2="265" y2="180" stroke="slategray" stroke-width="4"/>
//
//<path d="M100,15A85,85 0 0,0 15,100" fill="none" stroke="slategray" ></path>
//<path d="M180,15A85,85 0 0,1 265,100" fill="none" stroke="slategray" ></path>
//<path d="M265,180A85,85 0 0,1 180,265" fill="none" stroke="slategray" ></path>
//<path d="M15,180A85,85 0 0,0 100,265" fill="none" stroke="slategray" ></path>

    addDefsSymbol(symbol.str());

    //optimisation du log pour ne créer qu'un seul objet
    fLogBuffer = new logs::Logger::LoggerBuffer(logger(), logs::Level::INFO);

}

//angle en radian
void LegoEV3SvgWriterExtended::writePosition_BotPos(float x, float y, float angle_rad)
{
    if (!done_) {
        float delta_y = 50.0 * sin(angle_rad);
        float delta_x = 50.0 * cos(angle_rad);
        // inversion du y pour affichage dans le bon sens dans le SVG
        this->lock();
        *fLogBuffer << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"1\" fill=\"blue\"/>" "<line x1=\"" << x
                << "\" y1=\"" << -y << "\" x2=\"" << x + delta_x << "\" y2=\"" << -y - delta_y
                << "\" stroke-width=\"0.1\" stroke=\"grey\"/>" << logs::flush;
        this->unlock();
    }
}
void LegoEV3SvgWriterExtended::writePosition_Bot(float x, float y, float angle_rad, int color)
{
    if (!done_) {
        this->lock();
        if (color == 0) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << "," << x
                    << "," << -y << ")\" />" << logs::flush;
        } else if (color == 1) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-ORANGE\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        } else if (color == 2) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-RED\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        } else if (color == 3) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-GREEN\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        } else if (color == 4) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-BLUE\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        } else if (color == 5) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-BLACK\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        }
        this->unlock();
    }
}

void LegoEV3SvgWriterExtended::writePosition_AdvPos(float x, float y, int color)
{
    if (!done_) {
        this->lock();

        if (color == 0) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3\" />" << logs::flush;
        } else if (color == 1) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-ORANGE\"  />" << logs::flush;
        } else if (color == 2) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-RED\" />" << logs::flush;
        } else if (color == 3) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-GREEN\"  />" << logs::flush;
        } else if (color == 4) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-BLUE\" />" << logs::flush;
        } else if (color == 5) {
            *fLogBuffer << "<use x=\"" << x - 140 << "\" y=\"" << -y - 140
                    << "\" xlink:href=\"#bot-legoev3-BLACK\" />" << logs::flush;
        }
        this->unlock();
    }
}

void LegoEV3SvgWriterExtended::writeZone(const char *name, float minX, float minY, float width, float height,
        float startX, float startY, float startAngle_rad)
{
    if (!done_)
        //ia
        this->lock();
    *fLogBuffer << "<g transform=\"scale(1,-1) \">" << "<rect x=\"" << minX << "\" y=\"" << minY << "\" width=\""
            << width << "\" height=\"" << height << "\" fill=\"none\" stroke=\"#33cc33\" stroke-width=\"4\" />"
            << "<line x1 = \"" << minX << "\" y1 = \"" << minY << "\" x2 = \"" << minX + width << "\" y2 = \"" << minY
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

            << "<circle cx='" << startX << "' cy='" << startY << "' r='3' fill='none' stroke='#33cc33' />"
            << "<line x1 = \"" << startX << "\" y1 = \"" << startY << "\" x2 = \"" << startX + 25 * cos(startAngle_rad)
            << "\" y2 = \"" << startY + 25 * sin(startAngle_rad) << "\" stroke = \"#33cc33\" stroke-width = \"4\"/>"
            << "</g>"

            << "<text x='" << startX + 20 << "' y='" << -startY - 10 << "' font-size='30' fill='#33cc33'>" << name
            << "</text>" << logs::flush;
    this->unlock();
}

void LegoEV3SvgWriterExtended::writeIaPath(const char *zone1Name, const char *zone2Name, float x_mm, float y_mm)
{
    if (!done_)
        this->lock();
    *fLogBuffer << "<circle cx='" << x_mm << "' cy='" << -y_mm << "' r='15' fill='none' stroke='green' />"
            << "<text x='" << x_mm + 20 << "' y='" << -y_mm + 20 << "' font-size='30' fill='green'>" << zone1Name << "-"
            << zone2Name << "</text>" << logs::flush;
    this->unlock();
}

void LegoEV3SvgWriterExtended::pathPolyline(std::string points) //TODO mettre dans SVGWriter ?
{
    if (!done_) {
        this->lock();
        *fLogBuffer << "<polyline points='" << points << "' style='fill:none;stroke:red;stroke-width:4' />"
                << logs::flush;
        this->unlock();
    }
}
