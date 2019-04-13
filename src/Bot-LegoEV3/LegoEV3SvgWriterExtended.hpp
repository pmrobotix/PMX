#ifndef LegoEV3_SVGWRITEREXTENDED_HPP_
#define LegoEV3_SVGWRITEREXTENDED_HPP_

#include <string>

#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"

class LegoEV3SvgWriterExtended: public SvgWriter
{
private:
    logs::Logger::LoggerBuffer *fLogBuffer; //optimisation des logs

public:

    LegoEV3SvgWriterExtended(std::string botId);

    ~LegoEV3SvgWriterExtended()
    {
    }

    void writePosition_Bot(float x, float y, float a_rad);
    void writePosition_BotPos(float x, float y, float a_rad);

    void writeZone(const char* name, float minX, float minY, float width, float height, float startX, float startY,
            float startAngle_rad);

    void writeIaPath(const char* zone1Name, const char* zone2Name, float x_mm, float y_mm);

    void pathPolyline(std::string points);
};

#endif

