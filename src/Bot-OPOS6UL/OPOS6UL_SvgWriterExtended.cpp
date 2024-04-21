#include "OPOS6UL_SvgWriterExtended.hpp"

#include <cmath>
#include <sstream>

#include "../Log/Logger.hpp"

OPOS6UL_SvgWriterExtended::OPOS6UL_SvgWriterExtended(std::string botId) :
        SvgWriter(botId) //on appelle le constructeur pere
{

    std::ostringstream symbol;

    symbol << "<symbol id=\"bot-OPOS6UL\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"140\" fill=\"none\" stroke=\"slategray\" stroke-dasharray=\"2,8\" stroke-width=\"2px\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"slategray\" />"
            << "<line x1=\"95\" y1=\"21\" x2=\"205\" y2=\"21\" stroke=\"slategray\" stroke-width=\"4\"/>"
            << "<line x1=\"95\" y1=\"279\" x2=\"205\" y2=\"279\" stroke=\"slategray\" stroke-width=\"4\"/>"
            << "<line x1=\"22\" y1=\"95\" x2=\"22\" y2=\"205\" stroke=\"slategray\" stroke-width=\"4\"/>"

            << "<line x1=\"22\" y1=\"95\" x2=\"95\" y2=\"21\" stroke=\"slategray\" stroke-width=\"4\"/>"
            << "<line x1=\"21\" y1=\"205\" x2=\"95\" y2=\"279\" stroke=\"slategray\" stroke-width=\"4\"/>"
            << "<line x1=\"205\" y1=\"22\" x2=\"205\" y2=\"279\" stroke=\"slategray\" stroke-width=\"4\"/>"
            << "</symbol>";

    symbol << "<symbol id=\"bot-OPOS6UL-RED\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"140\" fill=\"none\" stroke=\"red\" stroke-dasharray=\"2,8\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"8\" fill=\"none\" stroke=\"red\" />"
            << "<line x1=\"95\" y1=\"21\" x2=\"205\" y2=\"21\" stroke=\"red\" stroke-width=\"2\"/>"
            << "<line x1=\"95\" y1=\"279\" x2=\"205\" y2=\"279\" stroke=\"red\" stroke-width=\"2\"/>"
            << "<line x1=\"23\" y1=\"95\" x2=\"23\" y2=\"205\" stroke=\"red\" stroke-width=\"2\"/>"
            << "<line x1=\"22\" y1=\"95\" x2=\"95\" y2=\"21\" stroke=\"red\" stroke-width=\"2\"/>"
            << "<line x1=\"21\" y1=\"205\" x2=\"95\" y2=\"279\" stroke=\"red\" stroke-width=\"2\"/>"
            << "<line x1=\"205\" y1=\"22\" x2=\"205\" y2=\"279\" stroke=\"red\" stroke-width=\"2\"/>" << "</symbol>";

    symbol << "<symbol id=\"bot-OPOS6UL-ORANGE\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"140\" fill=\"none\" stroke=\"orange\" stroke-dasharray=\"2,8\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"7\" fill=\"none\" stroke=\"orange\" />"
            << "<line x1=\"95\" y1=\"21\" x2=\"205\" y2=\"21\" stroke=\"orange\" stroke-width=\"2\"/>"
            << "<line x1=\"95\" y1=\"279\" x2=\"205\" y2=\"279\" stroke=\"orange\" stroke-width=\"2\"/>"
            << "<line x1=\"23\" y1=\"95\" x2=\"23\" y2=\"205\" stroke=\"orange\" stroke-width=\"2\"/>"
            << "<line x1=\"22\" y1=\"95\" x2=\"95\" y2=\"21\" stroke=\"orange\" stroke-width=\"2\"/>"
            << "<line x1=\"21\" y1=\"205\" x2=\"95\" y2=\"279\" stroke=\"orange\" stroke-width=\"2\"/>"
            << "<line x1=\"205\" y1=\"22\" x2=\"205\" y2=\"279\" stroke=\"orange\" stroke-width=\"2\"/>" << "</symbol>";

    symbol << "<symbol id=\"bot-OPOS6UL-GREEN\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"140\" fill=\"none\" stroke=\"green\" stroke-dasharray=\"2,8\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"6\" fill=\"none\" stroke=\"green\" />"
            << "<line x1=\"95\" y1=\"21\" x2=\"205\" y2=\"21\" stroke=\"green\" stroke-width=\"2\"/>"
            << "<line x1=\"95\" y1=\"279\" x2=\"205\" y2=\"279\" stroke=\"green\" stroke-width=\"2\"/>"
            << "<line x1=\"23\" y1=\"95\" x2=\"23\" y2=\"205\" stroke=\"green\" stroke-width=\"2\"/>"
            << "<line x1=\"22\" y1=\"95\" x2=\"95\" y2=\"21\" stroke=\"green\" stroke-width=\"2\"/>"
            << "<line x1=\"21\" y1=\"205\" x2=\"95\" y2=\"279\" stroke=\"green\" stroke-width=\"2\"/>"
            << "<line x1=\"205\" y1=\"22\" x2=\"205\" y2=\"279\" stroke=\"green\" stroke-width=\"2\"/>" << "</symbol>";

    symbol << "<symbol id=\"bot-OPOS6UL-BLUE\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"140\" fill=\"none\" stroke=\"blue\" stroke-dasharray=\"2,8\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"5\" fill=\"none\" stroke=\"blue\"/>"
            << "<line x1=\"95\" y1=\"21\" x2=\"205\" y2=\"21\" stroke=\"blue\" stroke-width=\"2\"/>"
            << "<line x1=\"95\" y1=\"279\" x2=\"205\" y2=\"279\" stroke=\"blue\" stroke-width=\"2\"/>"
            << "<line x1=\"23\" y1=\"95\" x2=\"23\" y2=\"205\" stroke=\"blue\" stroke-width=\"2\"/>"
            << "<line x1=\"22\" y1=\"95\" x2=\"95\" y2=\"21\" stroke=\"blue\" stroke-width=\"2\"/>"
            << "<line x1=\"21\" y1=\"205\" x2=\"95\" y2=\"279\" stroke=\"blue\" stroke-width=\"2\"/>"
            << "<line x1=\"205\" y1=\"22\" x2=\"205\" y2=\"279\" stroke=\"blue\" stroke-width=\"2\"/>" << "</symbol>";

    symbol << "<symbol id=\"bot-OPOS6UL-BLACK\">"
            << "<circle cx=\"150\" cy=\"150\" r=\"140\" fill=\"none\" stroke=\"black\" stroke-dasharray=\"2,8\" />"
            << "<circle cx=\"150\" cy=\"150\" r=\"4\" fill=\"none\" stroke=\"black\" />"
            << "<line x1=\"95\" y1=\"21\" x2=\"205\" y2=\"21\" stroke=\"black\" stroke-width=\"2\"/>"
            << "<line x1=\"95\" y1=\"279\" x2=\"205\" y2=\"279\" stroke=\"black\" stroke-width=\"2\"/>"
            << "<line x1=\"23\" y1=\"95\" x2=\"23\" y2=\"205\" stroke=\"black\" stroke-width=\"2\"/>"
            << "<line x1=\"22\" y1=\"95\" x2=\"95\" y2=\"21\" stroke=\"black\" stroke-width=\"2\"/>"
            << "<line x1=\"21\" y1=\"205\" x2=\"95\" y2=\"279\" stroke=\"black\" stroke-width=\"2\"/>"
            << "<line x1=\"205\" y1=\"22\" x2=\"205\" y2=\"279\" stroke=\"black\" stroke-width=\"2\"/>" << "</symbol>";
    /*
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
     << "<rect x=\"-140\" y=\"-140\" width=\"140\" height=\"140\" style=\"fill:none;stroke:orange;stroke-width:2px;\" />"

     //            << "<rect x=\"80\" y=\"0\" width=\"70\" height=\"300\" style=\"fill:none;stroke:orange;stroke-width:2px;\" />"
     //arriere du robot en position 0,0
     //            << "<line x1=\"150\" y1=\"0\" x2=\"400\" y2=\"150\" stroke=\"orange\" stroke-width=\"4\"/>"
     //            << "<line x1=\"150\" y1=\"300\" x2=\"400\" y2=\"150\" stroke=\"orange\" stroke-width=\"4\"/>"
     //            //devant du robot en position 300,0
     //            << "<line x1=\"400\" y1=\"60\" x2=\"400\" y2=\"240\" stroke=\"orange\" stroke-width=\"4\"/>"

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

     << "</symbol>";*/
    addDefsSymbol(symbol.str());

    //optimisation du log pour ne créer qu'un seul objet
    fLogBuffer = new logs::Logger::LoggerBuffer(logger(), logs::Level::INFO);
    fLogBufferSensors = new logs::Logger::LoggerBuffer(loggerSvgSensor(), logs::Level::INFO);
}

void OPOS6UL_SvgWriterExtended::writePosition_AdvPos(float x, float y, float x_pos_mm, float y_pos_mm, int color)
{
    if (!done_) {
        int r_adv = 400 /2; //TODO rendre parametrable
        this->lock();

        if (color == 0) {
            *fLogBufferSensors //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL\" />"
                    << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\""<< r_adv <<"\" fill=\"none\" stroke=\"slategray\" />"
                    << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm << "\" x2=\"" << x << "\" y2=\"" << -y
                    << "\" stroke=\"slategray\" stroke-width=\"1\"/>" << logs::flush;
        } else if (color == 1) {
            *fLogBufferSensors //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-ORANGE\"  />"
                    << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\""<< r_adv+6 <<"\" fill=\"none\" stroke=\"orange\" />"
                    << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm << "\" x2=\"" << x << "\" y2=\"" << -y
                    << "\" stroke=\"ORANGE\" stroke-width=\"1\"/>" << logs::flush;
        } else if (color == 2) {
            *fLogBufferSensors //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-RED\" />"
                    << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\""<< r_adv+10 <<"\" fill=\"none\" stroke=\"red\" />"
                    << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\""<< 150 <<"\" fill=\"none\" stroke=\"red\" />"
                    << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm << "\" x2=\"" << x << "\" y2=\"" << -y
                    << "\" stroke=\"RED\" stroke-width=\"1\"/>" << logs::flush;
        } else if (color == 3) {
            *fLogBufferSensors //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-GREEN\"  />"
                    << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\""<< r_adv+4 <<"\" fill=\"none\" stroke=\"green\" />"
                    << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm << "\" x2=\"" << x << "\" y2=\"" << -y
                    << "\" stroke=\"GREEN\" stroke-width=\"1\"/>" << logs::flush;
        } else if (color == 4) {
            *fLogBufferSensors //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-BLUE\" />"
                    << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\""<< r_adv+2 <<"\" fill=\"none\" stroke=\"blue\" />"
                    << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm << "\" x2=\"" << x << "\" y2=\"" << -y
                    << "\" stroke=\"BLUE\" stroke-width=\"1\"/>" << logs::flush;
        } else if (color == 5) {
            *fLogBufferSensors //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-BLACK\" />"
                    << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\""<< r_adv+8 <<"\" fill=\"none\" stroke=\"black\" />"
                    << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm << "\" x2=\"" << x << "\" y2=\"" << -y
                    << "\" stroke=\"BLACK\" stroke-width=\"1\"/>" << logs::flush;
        }
        this->unlock();
    }
}
/*
 void OPOS6UL_SvgWriterExtended::writePosition_AdvPos(float x, float y, int color)
 {
 //printf("writePosition_Bot done_=%d\n", done_);

 if (!done_) {
 this->lock();
 if (color == 0) {
 *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
 << "\" xlink:href=\"#bot-OPOS6UL\" />" << logs::flush;
 } else if (color == 1) {
 *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
 << "\" xlink:href=\"#bot-OPOS6UL-ORANGE\" />" << logs::flush;
 } else if (color == 2) {
 *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
 << "\" xlink:href=\"#bot-OPOS6UL-RED\" />" << logs::flush;
 } else if (color == 3) {
 *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
 << "\" xlink:href=\"#bot-OPOS6UL-GREEN\" />" << logs::flush;
 } else if (color == 4) {
 *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
 << "\" xlink:href=\"#bot-OPOS6UL-BLUE\" />" << logs::flush;
 } else if (color == 5) {
 *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
 << "\" xlink:href=\"#bot-OPOS6UL-BLACK\"  />" << logs::flush;
 }
 this->unlock();
 }
 }
 */

//angle en radian
void OPOS6UL_SvgWriterExtended::writePosition_Bot(float x, float y, float angle_rad, int color)
{
    //printf("writePosition_Bot done_=%d\n", done_);

    if (!done_) {
        this->lock();
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
        } else if (color == 4) {
            *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
                    << "\" xlink:href=\"#bot-OPOS6UL-BLUE\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        } else if (color == 5) {
            *fLogBuffer << "<use x=\"" << x - 150 << "\" y=\"" << -y - 150
                    << "\" xlink:href=\"#bot-OPOS6UL-BLACK\" transform=\"rotate(" << -((angle_rad * 180) / M_PI) << ","
                    << x << "," << -y << ")\" />" << logs::flush;
        }
        this->unlock();
    }
}

void OPOS6UL_SvgWriterExtended::writePosition_BotPos(float x, float y, float angle_rad)
{
    if (!done_) {
        //printf("\ndone_=%d\n", done_);
        double delta_y = 0.0;
        double delta_x = 0.0;

        delta_y = 50.0 * sin(angle_rad);
        delta_x = 50.0 * cos(angle_rad);
        this->lock();
        // inversion du y pour affichage dans le bon sens dans le SVG
        *fLogBuffer << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"1\" fill=\"blue\" />" << logs::flush;

        *fLogBuffer << "<line x1=\"" << x << "\" y1=\"" << -y << "\" x2=\"" << x + delta_x << "\" y2=\"" << -y - delta_y
                << "\" stroke-width=\"0.1\" stroke=\"grey\"  />" << logs::flush;
        this->unlock();
    }
}
void OPOS6UL_SvgWriterExtended::writeZone(const char *name, float minX, float minY, float width, float height,
        float startX, float startY, float startAngle_rad)
{
    if (!done_) {
        //ia
        this->lock();
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
        this->unlock();
    }
}

//display path for IAbyZone
void OPOS6UL_SvgWriterExtended::writeIaPath(const char *zone1Name, const char *zone2Name, float x_mm, float y_mm)
{
    if (!done_) {
        this->lock();
        *fLogBuffer << "<circle cx='" << x_mm << "' cy='" << -y_mm << "' r='15' fill='none' stroke='green' />"
                << "<text x='" << x_mm + 20 << "' y='" << -y_mm + 20 << "' font-size='30' fill='green'>" << zone1Name
                << "-" << zone2Name << "</text>" << logs::flush;
        this->unlock();
    }
}

void OPOS6UL_SvgWriterExtended::pathPolyline(std::string points) //TODO mettre dans SVGWriter ?
{
    if (!done_) {
        this->lock();
        *fLogBuffer << "<polyline points='" << points << "' style='fill:none;stroke:red;stroke-width:4' />"
                << logs::flush;
        this->unlock();
    }
}
