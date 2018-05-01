#ifndef OPOS6UL_SVGWRITEREXTENDED_HPP_
#define OPOS6UL_SVGWRITEREXTENDED_HPP_

#include <string>

#include "../Log/SvgWriter.hpp"

class OPOS6UL_SvgWriterExtended: public SvgWriter
{
private:

public:

	OPOS6UL_SvgWriterExtended(std::string botId);

	~OPOS6UL_SvgWriterExtended()
	{
	}

	void writePosition(double x_mm, double y_mm, double angle_rad, std::string symbol);

	void writeZone(const char* name,
			float minX,
			float minY,
			float width,
			float height,
			float startX,
			float startY,
			float startAngle_rad);

	void writeIaPath(const char* zone1Name,
			const char* zone2Name,
			float x_mm,
			float y_mm);

	void pathPolyline(std::string points);
};

#endif

