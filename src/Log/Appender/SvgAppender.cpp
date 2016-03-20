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
	this->ofs_.close();
}
