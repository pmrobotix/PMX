#ifndef LegoEV3_SVGWRITEREXTENDED_HPP_
#define LegoEV3_SVGWRITEREXTENDED_HPP_

#include <string>

#include "../Log/Logger.hpp"
#include "../Log/SvgWriter.hpp"

class LegoEV3SvgWriterExtended: public SvgWriter
{
private:
    logs::Logger::LoggerBuffer *fLogBuffer; //optimisation des logs pour ecrire sans recréer un objet
public:

    LegoEV3SvgWriterExtended(std::string botId);

    ~LegoEV3SvgWriterExtended()
    {
    }

    void writePosition_AdvPos(float x_mm, float y_mm, int color = 0);

    void writePosition_Bot(float x_mm, float y_mm, float a_rad, int color = 0);

    void writePosition_BotPos(float x_mm, float y_mm, float a_rad);

    void writeZone(const char* name, float minX, float minY, float width, float height, float startX, float startY,
            float startAngle_rad);

    void writeIaPath(const char* zone1Name, const char* zone2Name, float x_mm, float y_mm);

    void pathPolyline(std::string points);
};

#endif

