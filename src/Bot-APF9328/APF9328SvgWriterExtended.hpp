#ifndef APF9328_SVGWRITEREXTENDED_HPP_
#define APF9328_SVGWRITEREXTENDED_HPP_

#include <string>

#include "../Common/Asserv/MovingBase.hpp"
#include "../Log/SvgWriter.hpp"

enum APF9328SvgSymbol
{
	APF9328_SVG_POS_ROBOT,
	APF9328_SVG_POS_FOLLOWING
};

class APF9328SvgWriterExtended: public SvgWriter
{
private:

public:

	APF9328SvgWriterExtended(std::string botId);

	~APF9328SvgWriterExtended()
	{
	}

	void writePosition(double x, double y, double angle, APF9328SvgSymbol symbol);

};

#endif

