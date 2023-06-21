/*!
 * \file
 * \brief Définition de la classe SvgWriter.
 */

#ifndef LOG_SVGWRITER_HPP_
#define	LOG_SVGWRITER_HPP_

#include <string>

#include "../Common/Utils/PointerList.hpp"
#include "../Thread/Mutex.hpp"

namespace logs {
class Logger;
} /* namespace logs */

/*!
 * \brief Wrapper pour la génération de fichier svg via le système de log.
 */
class SvgWriter: public utils::Mutex
{
private:

    /*
     * Id of the associated robot.
     */
    std::string id_;

    utils::PointerList<std::string> symbol_list_;

protected:
    bool done_;
    bool beginDone_;
    const  logs::Logger * fLogger;

public:

    /*!
     * \brief Constructeur de la classe.
     *
     * Ce constructeur est privé pour empécher la création d'une instance
     * de la classe.
     */
    SvgWriter(std::string id);

    /*!
     * \brief Destructeur de la classe.
     */
    virtual ~SvgWriter();

    inline const logs::Logger & logger()
    {
        return *fLogger;
    }

    void addDefsSymbol(std::string symbol);

    void beginHeader();

    void endHeader();

    void writeText(float x_mm, float y_mm, std::string text);

    void writeTextCustom(float x_mm, float y_mm, std::string text, std::string color, std::string fontsize);

    virtual void writePosition_Bot(float x_mm, float y_mm, float a_rad, int color = 0) = 0;
    virtual void writePosition_BotPos(float x_mm, float y_mm, float a_rad) = 0;

    virtual void writePosition_AdvPos(float x_adv_mm, float y_adv_mm, float x_pos_mm, float y_pos_mm, int color = 0) = 0;

    virtual void writeZone(const char* name, float minX_mm, float minY_mm, float width_mm, float height_mm,
            float startX_mm, float startY_mm, float startAngle_rad) = 0;

    virtual void writeIaPath(const char* zone1Name, const char* zone2Name, float x_mm, float y_mm) = 0;

    virtual void pathPolyline(std::string points) = 0;

};

#endif
