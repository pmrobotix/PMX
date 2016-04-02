#ifndef LegoEV3_SVGWRITEREXTENDED_HPP_
#define LegoEV3_SVGWRITEREXTENDED_HPP_

#include <string>

#include "../Common/Asserv/MovingBase.hpp"
#include "../Log/SvgWriter.hpp"

enum LegoEv3SvgSymbol
{
	LEGOEV3_SVG_POS_ROBOT,
	LEGOEV3_SVG_POS_FOLLOWING
};

class LegoEV3SvgWriterExtended: public SvgWriter
{
private:

public:

	LegoEV3SvgWriterExtended(std::string botId);

	~LegoEV3SvgWriterExtended()
	{
	}

	void writePosition(double x, double y, double a_rad, LegoEv3SvgSymbol symbol);

};

#endif

