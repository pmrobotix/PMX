//drivers...SIMU

#include "SensorsDriver.hpp"

#include <cmath>
#include <sstream>
#include <vector>

#include "../Log/Level.hpp"

using namespace std;

ASensorsDriver* ASensorsDriver::create(std::string, ARobotPositionShared *aRobotPositionShared)
{
    return new SensorsDriver(aRobotPositionShared);
}

SensorsDriver::SensorsDriver(ARobotPositionShared *aRobotPositionShared)
{
    pos_pour_calcul_ = { 0 };
    pos_pour_calcul_prec_ = { 0 };
    robotPositionShared_ = aRobotPositionShared;
    x_adv_ = 1000;
    y_adv_ = 1400;

}

SensorsDriver::~SensorsDriver()
{
}

bool SensorsDriver::is_connected()
{
    return true;
}

void SensorsDriver::displayNumber(int number)
{

}

//a deplacer dans RobotPositionShared ?transformPosREPTABLE_to_PosREPROBOT Ne sert uniquement a la simu des sensors ?
RobotPos SensorsDriver::transformPosTableToPosRobot(int nb, float x_table, float y_table)
{
    loggerSvg().info() << "<circle cx=\"" << x_table << "\" cy=\"" << -y_table << "\" r=\"5\" fill=\"red\" />"
            << logs::end;

    //coord table à transformer en coordonnées robot: 200,700 => position robot robot_
    //ROBOTPOSITION p = robotPositionShared_->getRobotPosition(); //robot_->asserv().pos_getPosition();
    ROBOTPOSITION p = pos_pour_calcul_;

    loggerSvg().info() << "<circle cx=\"" << p.x << "\" cy=\"" << -p.y << "\" r=\"3\" fill=\"blue\" />" << "<line x1=\""
            << p.x << "\" y1=\"" << -p.y << "\" x2=\"" << p.x + cos(p.theta) * 25 << "\" y2=\""
            << -p.y - sin(p.theta) * 25 << "\" stroke-width=\"0.1\" stroke=\"grey\"  />" << logs::end;

    float d = std::sqrt(square(y_table-p.y) + square(x_table - p.x));

    float x_rep_robot = 0;
    float y_rep_robot = 0;

    float b_rad = std::asin((y_table - p.y) / d);

    //pb quadrant car on ne connait pas les angles
//    if (p.x <= x_table) {
//        if (p.y <= y_table )
//        {
//           //
//        }
//        else
//        {
//            //b_rad = - b_rad ;
//        }
//    } else {
//        if (p.y <= y_table )
//        {
//            b_rad = -M_PI - b_rad;
//        }
//        else
//        {
//            b_rad = -M_PI - b_rad ;
//        }
//    }

    //cas du cadran en x
    if (p.x >= x_table) {
        b_rad = -M_PI - b_rad;
    }

    float alpha_rad = b_rad - p.theta + M_PI_2;

    alpha_rad = std::fmod(alpha_rad, 2.0 * M_PI);
    if (alpha_rad < -M_PI)
        alpha_rad += (2.0 * M_PI);
    if (alpha_rad > M_PI)
        alpha_rad -= (2.0 * M_PI);

    //angle entre l'axe devant le robot et le segment entre les milieu des 2 robots
    float alpha_deg = (alpha_rad * 180.0 / M_PI); //a_deg_rep_robot

    logger().debug() << __FUNCTION__ << "DEBUG (" << x_table << "," << y_table << ") b_deg=" << b_rad * 180.0 / M_PI
            << " p.theta=" << p.theta * 180.0 / M_PI << " RESULT alpha_deg=" << alpha_deg << logs::end;

    y_rep_robot = d * std::sin(alpha_rad);
    x_rep_robot = d * std::cos(alpha_rad);

    logger().debug() << __FUNCTION__ << "DEBUG (" << x_table << "," << y_table << ") x_rep_robot=" << x_rep_robot
            << " y_rep_robot=" << y_rep_robot << " a_deg_rep_robot=" << alpha_deg << logs::end;

    RobotPos pos = { nb, x_rep_robot, y_rep_robot, alpha_deg, d };

    return pos;
}

ASensorsDriver::bot_positions SensorsDriver::getvPositionsAdv()
{

    return vadv_;

}
int SensorsDriver::sync()
{

    //simulation de recuperation des données de la balise par la transaction i2c
    //prise en compte de la position du robot à se moment pour les futures calculs, voir la version precedentes
    //pos_pour_calcul_prec_ = pos_pour_calcul_;
    pos_pour_calcul_ = robotPositionShared_->getRobotPosition(0);


    //ASensorsDriver::bot_positions bot_pos;

    //coord table à transformer en coordonnées robot: 200,700 => position robot robot_
    //    int x_table = 1000.0;
    //    int y_table = 1500.0;

    //    x_adv_ -=15;
    //    y_adv_ -=10;
    //    x_adv_ = 1300.0;
    //    y_adv_ = 1000.0;

    int nb = 4;
    //    RobotPos pos1 = transformPosTableToPosRobot(nb, 700.0, 1800.0);
    //    RobotPos pos2 = transformPosTableToPosRobot(nb, 600.0, 1000.0);
    //    RobotPos pos3 = transformPosTableToPosRobot(nb, 1300.0, 300.0);
    //    RobotPos pos4 = transformPosTableToPosRobot(nb, 300.0, 400.0);

    vadv_.clear();
    if ((pos_pour_calcul_.x >= 0.1 || pos_pour_calcul_.y >= 0.1)) {

        RobotPos pos1 = transformPosTableToPosRobot(nb, x_adv_, y_adv_);
        RobotPos pos2 = transformPosTableToPosRobot(nb, 1000, 200);
        RobotPos pos3 = transformPosTableToPosRobot(nb, 800, 2000);
        RobotPos pos4 = transformPosTableToPosRobot(nb, 100, 1400);

        vadv_.push_back(pos1);
        vadv_.push_back(pos2);
        vadv_.push_back(pos3);
        vadv_.push_back(pos4);
        //simu des positions adverses
        //bot_pos = { };
        //vadv_ = { pos1 };
        //bot_pos = { pos1, pos2 };
        //bot_pos = { pos1, pos2, pos3, pos4 };

    }

    //usleep(20000); //temps de mise a jour des données
    //utils::Thread::sleep_for_micros(200000);
    return 0;
}

int SensorsDriver::rightSide()
{
    return 400;
}
int SensorsDriver::leftSide()
{
    return 400;
}

int SensorsDriver::frontLeft()
{
    return 999;
}
int SensorsDriver::frontCenter()
{
    return 999;

}
int SensorsDriver::frontRight()
{
    //TODO temp mettre un ifdef pour simulation gros robot et petit robot
    /*
     OPOS6UL_RobotExtended &robot = OPOS6UL_RobotExtended::instance();

     if (robot.asserv().pos_getX_mm() > 800)
     {
     robot.svgPrintPosition(3);
     return 300;
     }
     else*/
    return 999;
}

int SensorsDriver::backLeft()
{
    return 999;
}
int SensorsDriver::backCenter()
{
    return 999;
}
int SensorsDriver::backRight()
{
    return 999;
}

SvgWriterExtended::SvgWriterExtended(std::string botId) :
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
void SvgWriterExtended::writePosition_BotPos(float x, float y, float angle_rad)
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

void SvgWriterExtended::writePosition_Bot(float x, float y, float angle_rad, int color)
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

void SvgWriterExtended::writePosition_AdvPos(float x, float y, float x_pos_mm, float y_pos_mm, int color)
{

    if (!done_) {
        int r_adv = 400 / 2; //TODO rendre parametrable
        this->lock();

        if (color == 0) {
            *fLogBuffer //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL\" />"
            << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"" << r_adv
                    << "\" fill=\"none\" stroke=\"slategray\" />" << "<line x1=\"" << x_pos_mm << "\" y1=\""
                    << -y_pos_mm << "\" x2=\"" << x << "\" y2=\"" << -y
                    << "\" stroke=\"slategray\" stroke-width=\"1\"/>" << logs::flush;
        } else if (color == 1) {
            *fLogBuffer //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-ORANGE\"  />"
            << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"" << r_adv + 6
                    << "\" fill=\"none\" stroke=\"orange\" />" << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm
                    << "\" x2=\"" << x << "\" y2=\"" << -y << "\" stroke=\"ORANGE\" stroke-width=\"1\"/>"
                    << logs::flush;
        } else if (color == 2) {
            *fLogBuffer //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-RED\" />"
            << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"" << r_adv + 10
                    << "\" fill=\"none\" stroke=\"red\" />" << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\""
                    << 150 << "\" fill=\"none\" stroke=\"red\" />" << "<line x1=\"" << x_pos_mm << "\" y1=\""
                    << -y_pos_mm << "\" x2=\"" << x << "\" y2=\"" << -y << "\" stroke=\"RED\" stroke-width=\"1\"/>"
                    << logs::flush;
        } else if (color == 3) {
            *fLogBuffer //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-GREEN\"  />"
            << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"" << r_adv + 4
                    << "\" fill=\"none\" stroke=\"green\" />" << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm
                    << "\" x2=\"" << x << "\" y2=\"" << -y << "\" stroke=\"GREEN\" stroke-width=\"1\"/>" << logs::flush;
        } else if (color == 4) {
            *fLogBuffer //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-BLUE\" />"
            << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"" << r_adv + 2
                    << "\" fill=\"none\" stroke=\"blue\" />" << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm
                    << "\" x2=\"" << x << "\" y2=\"" << -y << "\" stroke=\"BLUE\" stroke-width=\"1\"/>" << logs::flush;
        } else if (color == 5) {
            *fLogBuffer //<< "<use x=\"" << x - dd << "\" y=\"" << -y - dd << "\" xlink:href=\"#bot-OPOS6UL-BLACK\" />"
            << "<circle cx=\"" << x << "\" cy=\"" << -y << "\" r=\"" << r_adv + 8
                    << "\" fill=\"none\" stroke=\"black\" />" << "<line x1=\"" << x_pos_mm << "\" y1=\"" << -y_pos_mm
                    << "\" x2=\"" << x << "\" y2=\"" << -y << "\" stroke=\"BLACK\" stroke-width=\"1\"/>" << logs::flush;
        }
        this->unlock();
    }

}

