/*!
 * \file
 * \brief Implémentation de la classe SvgAppender.
 */

#include "../Appender/SvgAppender.hpp"

#include <cstdlib>
#include <string>

logs::SvgAppender::SvgAppender(const std::string & filename) :
		ofs_(filename.c_str())
{
	int ymin = -2200;
	int ymax = +2200;
	int xmin = -200;
	int xmax = 3100;

	//entete xml
	//this->ofs_ << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\""
	//        << (abs(xmin) + std::abs(xmax))
	//        << "px\" height=\"" << (std::abs(ymin) + std::abs(ymax)) << "px\">"
	//        << std::endl;
	this->ofs_ << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
	this->ofs_ << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20010904//EN\"" << std::endl;
	this->ofs_ << "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">" << std::endl;
	this->ofs_ << "<svg width=\"" << (abs(xmin) + std::abs(xmax)) << "px\" height=\""
			<< (std::abs(ymin) + std::abs(ymax)) << "px\"" << std::endl;
	this->ofs_
			<< "xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">"
			<< std::endl;

	this->ofs_ << "<defs>" << "<line id=\"Vline\" x1=\"0\" y1=\"" << ymin << "\" x2=\"0\" y2=\""
			<< ymax << "\" stroke=\"#eeeeee\" />" << "<line id=\"Hline\" x1=\"" << xmin
			<< "\" y1=\"0\" x2=\"" << xmax << "\" y2=\"0\" stroke=\"#eeeeee\" />"
			<< "<symbol id=\"robot-pmx\">"
			<< "<circle cx=\"190\" cy=\"190\" r=\"190\" fill=\"none\" stroke=\"blue\" />"
			<< "<circle cx=\"190\" cy=\"190\" r=\"10\" fill=\"none\" stroke=\"blue\" />"
			//<< "<line x1=\"60\" y1=\"60\" x2=\"60\" y2=\"320\" stroke=\"blue\" />" //arriere du robot en position 0,0
			//<< "<line x1=\"340\" y1=\"80\" x2=\"340\" y2=\"300\" stroke=\"blue\" />" //devant du robot en position 0,0
			<< "</symbol>" << "</defs>" << std::endl;

	this->ofs_ << "<g transform=\"translate(" << std::abs(xmin) << "," << std::abs(ymin) << ")\">"
			<< std::endl;

	//affichage des lignes d'abcisse
	for (int i = ymax; i >= ymin; i -= 100)
	{
		//<use x="0" y="100" xlink:href="#Hline" />
		this->ofs_ << "<use x=\"0\" y=\"" << i << "\" xlink:href=\"#Hline\" />" << std::endl;
		//this->ofs_ << "<line x1=\"" << xmin << "\" y1=\"" << i << "\" x2=\"" << xmax << "\" y2=\"" << i << "\" stroke=\"#eeeeee\" />" << std::endl;
		this->ofs_ << "<text x=\"0\" y=\"" << i
				<< "\" dx=\"5\" dy=\"-5\" fill=\"#444444\" font-size=\"12\">" << -i << "</text>"
				<< std::endl;
	}
	//affichage des lignes d'ordonnée
	for (int i = xmin; i <= xmax; i += 100)
	{
		//<use x="100" y="0" xlink:href="#Vline" />
		this->ofs_ << "<use x=\"" << i << "\" y=\"0\" xlink:href=\"#Vline\" />" << std::endl;
		//this->ofs_ << "<line x1=\"" << i << "\" y1=\"" << ymin << "\" x2=\"" << i << "\" y2=\"" << ymax << "\" stroke=\"#eeeeee\" />" << std::endl;
		this->ofs_ << "<text x=\"" << i
				<< "\" y=\"0\" dx=\"5\" dy=\"17\" fill=\"#444444\" font-size=\"12\">" << i
				<< "</text>" << std::endl;
	}
	this->ofs_ << "</g>" << std::endl;

	this->ofs_ << "<g transform=\"translate(" << std::abs(xmin) << "," << std::abs(ymin) << ")\">"
			<< std::endl;

}

logs::SvgAppender::~SvgAppender()
{
	this->flush();
}

void logs::SvgAppender::writeMessage(const logs::Logger &, const logs::Level &,
		const std::string & message)
{
	this->lock();
	std::ostringstream out;
	this->ofs_ << message << std::endl;
	this->unlock();
}

void logs::SvgAppender::flush()
{
	this->ofs_ << "</g>" << std::endl << "</svg>" << std::endl;
	this->ofs_.close();
}
