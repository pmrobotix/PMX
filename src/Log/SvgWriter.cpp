/*!
 * \file
 * \brief Implémentation de la classe SvgWriter.
 * test
 */

#include "SvgWriter.hpp"

#include <stdlib.h>
#include <list>
#include <sstream>

#include "Level.hpp"
#include "Logger.hpp"
#include "LoggerFactory.hpp"

SvgWriter::SvgWriter(std::string id)
{
    id_ = id;
    done_ = false;
    beginDone_ = false;

    std::ostringstream s;
    s << "Svg4" << id_; //define the logger (ex : Svg4OPOS6UL_Robot / IAbyPath4OPOS6UL_Robot) to be used in LoggerInitialize
    fLogger = &logs::LoggerFactory::logger(s.str());
}

SvgWriter::~SvgWriter()
{
    endHeader();
}

void SvgWriter::beginHeader()
{
    int ymin = -2200;
    int ymax = 300;
    int xmin = -200;
    int xmax = 3200;
    this->lock();
    logger().info() << "<?xml version=\"1.0\" standalone=\"no\"?>" << logs::end;
    logger().info() << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20010904//EN\"" << logs::end;
    logger().info() << "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << logs::end;
    logger().info() << "<svg width=\"" << (abs(xmin) + std::abs(xmax)) << "px\" height=\""
            << (std::abs(ymin) + std::abs(ymax)) << "px\"" << logs::end;
    logger().info() << "xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">"
            << logs::end;

    //les defs
    logger().info() << "<defs>" << "<line id=\"Vline\" x1=\"0\" y1=\"" << ymin << "\" x2=\"0\" y2=\"" << ymax
            << "\" stroke=\"#eeeeee\" />" << "<line id=\"Hline\" x1=\"" << xmin << "\" y1=\"0\" x2=\"" << xmax
            << "\" y2=\"0\" stroke=\"#eeeeee\" />"

            << logs::end;
    /*
     logger().info() << "<symbol id=\"robot-pmx\">"
     << "<circle cx=\"150\" cy=\"150\" r=\"150\" fill=\"none\" stroke=\"blue\" />"
     << "<circle cx=\"150\" cy=\"150\" r=\"10\" fill=\"none\" stroke=\"blue\" />"
     << "<rect x=\"0\" y=\"0\" width=\"300\" height=\"300\" style=\"fill:none;stroke:slategray;stroke-width:2px;\" />"
     << "<line x1=\"0\" y1=\"30\" x2=\"0\" y2=\"300\" stroke=\"red\" stroke-width=\"4\"/>"
     //arriere du robot en position 0,0
     << "<line x1=\"0\" y1=\"30\" x2=\"50\" y2=\"0\" stroke=\"red\" stroke-width=\"4\"/>"
     << "<line x1=\"0\" y1=\"270\" x2=\"50\" y2=\"300\" stroke=\"red\" stroke-width=\"4\"/>"
     << "<line x1=\"300\" y1=\"0\" x2=\"300\" y2=\"300\" stroke=\"green\" stroke-width=\"4\"/>"
     //devant du robot en position 300,0
     << "</symbol>"
     << logs::end;
     */
    //utils::PointerList<std::string>::iterator i = symbol_list_.begin();
    /*while (i != symbol_list_.end())
     {
     logger().info() << i << logs::end;
     i++;
     }*/
    for (utils::PointerList<std::string>::iterator i = symbol_list_.begin(); i != symbol_list_.end(); i++) {
        std::string str = *i;
        logger().info() << str << logs::end;
    }

    logger().info() << "</defs>" << logs::end;

    logger().info() << "<g transform=\"translate(" << std::abs(xmin) << "," << std::abs(ymin) << ")\">" << logs::end;

    //affichage des lignes d'abcisse
    for (int i = ymax; i >= ymin; i -= 100) {
        logger().info() << "<use x=\"0\" y=\"" << i << "\" xlink:href=\"#Hline\" />" << logs::end;
        logger().info() << "<text x=\"0\" y=\"" << i << "\" dx=\"-80\" dy=\"-3\" fill=\"#444444\" font-size=\"30\">"
                << -i << "</text>" << logs::end;
    }
    //affichage des lignes d'ordonnée
    for (int i = xmin; i <= xmax; i += 100) {
        logger().info() << "<use x=\"" << i << "\" y=\"0\" xlink:href=\"#Vline\" />" << logs::end;
        logger().info() << "<text x=\"" << i << "\" y=\"0\" dx=\"1\" dy=\"20\" fill=\"#444444\" font-size=\"30\">" << i
                << "</text>" << logs::end;
    }
    logger().info() << "</g>" << logs::end;
    logger().info() << "<g transform=\"translate(" << std::abs(xmin) << "," << std::abs(ymin) << ")\">" << logs::end;
    this->unlock();
    beginDone_ = true;
}

void SvgWriter::addDefsSymbol(std::string symbol)
{
    symbol_list_.push_back(symbol);
}

void SvgWriter::endHeader()
{
    if (!done_) {
        if (beginDone_) {
            done_ = true;
            //usleep(100000);
            this->lock();
            logger().info() << "</g>" << logs::end;
            logger().info() << "</svg>" << logs::end;
            this->unlock();
        }
    }
}

void SvgWriter::writeText(float x, float y, std::string text)
{
    if (!done_) {
        if (logger().isActive(logs::Level::INFO)) {
            this->lock();
            // inversion du y pour affichage dans le bon sens dans le SVG
            logger().info() << "<text x='" << x << "' y='" << -y << "' font-size='5' fill='black'>" << text << "</text>"
                    << logs::end;
            this->unlock();
        }
    }
}

void SvgWriter::writeTextCustom(float x, float y, std::string text, std::string color, std::string fontsize)
{
    if (!done_) {
        if (logger().isActive(logs::Level::INFO)) {
            this->lock();
            // inversion du y pour affichage dans le bon sens dans le SVG
            logger().info() << "<text x='" << x << "' y='" << -y << "' font-size='" << fontsize << "' fill='" << color
                    << "'>" << text << "</text>" << logs::end;
            this->unlock();
        }
    }
}

