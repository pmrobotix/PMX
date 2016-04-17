#ifndef LegoEV3_SVGWRITEREXTENDED_HPP_
#define LegoEV3_SVGWRITEREXTENDED_HPP_

#include <string>

#include "../Common/Asserv/MovingBase.hpp"
#include "../Log/SvgWriter.hpp"

class LegoEV3SvgWriterExtended: public SvgWriter
{
private:

public:

	LegoEV3SvgWriterExtended(std::string botId);

	~LegoEV3SvgWriterExtended()
	{
	}

	void writePosition(double x, double y, double a_rad, std::string symbol);

	void writeZone(const char* name,
			float minX,
			float minY,
			float width,
			float height,
			float startX,
			float startY,
			float startAngle_rad);

	void writeIaPath(const char* zone1Name, const char* zone2Name, float x_mm, float y_mm);
};

#endif

