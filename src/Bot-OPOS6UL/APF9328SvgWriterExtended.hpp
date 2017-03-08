#ifndef APF9328_SVGWRITEREXTENDED_HPP_
#define APF9328_SVGWRITEREXTENDED_HPP_

#include <string>

#include "../Log/SvgWriter.hpp"

class APF9328SvgWriterExtended: public SvgWriter
{
private:

public:

	APF9328SvgWriterExtended(std::string botId);

	~APF9328SvgWriterExtended()
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
};

#endif

