#ifndef OPOS6UL_SVGWRITEREXTENDED_HPP_
#define OPOS6UL_SVGWRITEREXTENDED_HPP_

#include <string>

#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"

class OPOS6UL_SvgWriterExtended: public SvgWriter
{
private:
    logs::Logger::LoggerBuffer *fLogBuffer;
public:

    OPOS6UL_SvgWriterExtended(std::string botId);

    ~OPOS6UL_SvgWriterExtended()
    {
    }

    void writePosition_AdvPos(float x_adv_mm, float y_adv_mm, float x_pos_mm, float y_pos_mm, int color = 0);

    void writePosition_Bot(float x_mm, float y_mm, float angle_rad, int color = 0);

    void writePosition_BotPos(float x_mm, float y_mm, float angle_rad);

    void writeZone(const char* name, float minX, float minY, float width, float height, float startX, float startY,
            float startAngle_rad);

    void writeIaPath(const char* zone1Name, const char* zone2Name, float x_mm, float y_mm);

    void pathPolyline(std::string points);
};

#endif

