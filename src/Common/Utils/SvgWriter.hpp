/*!
 * \file
 * \brief Définition de la classe SvgWriter.
 */

#ifndef SVGWRITER_HPP_
#define	SVGWRITER_HPP_


#include <string>

#include "../../Log/LoggerFactory.hpp"


/*!
 * \brief Enumération des sens de la base.
 */
enum SvgPositionColor
{
	SVG_POS_GREEN, SVG_POS_BLUE, SVG_POS_ANGLE, SVG_POS_BLACK, SVG_POS_RED, SVG_POS_YELLOW, SVG_POS_END, SVG_POS_ROBOT
};

/*!
 * \brief Wrapper pour la génération de fichier svg via le système de log.
 */
class SvgWriter
{
private:

	/*!
	 * \brief Retourne le \ref Logger nommé svg associé à la classe \ref SvgWriter.
	 */
	static inline const logs::Logger & svg()
	{
		static const logs::Logger & svg_ = logs::LoggerFactory::logger("SvgWriter::svg");
		return svg_;
	}

private:

	/*!
	 * \brief Constructeur de la classe.
	 *
	 * Ce constructeur est privé pour empécher la création d'une instance
	 * de la classe.
	 */
	SvgWriter();

public:

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual ~SvgWriter();

	/*!
	 * \brief Affiche la position actuel du robot sur le svg.
	 * \param x
	 *        Position en abscisse du robot.
	 * \param y
	 *        Position en ordonnée du robot.
	 * \param angle
	 *        Orientation en radian du robot.
	 */
	static void writePosition(double x, double y, double angle, SvgPositionColor svgcolor);

	static void writeText(double x, double y, std::string text);

	static void writeTextCustom(double x, double y, std::string text, std::string color, std::string fontsize);

	//TODO deprecated
	static void writePawn(double x, double y);

	static void writeZone(const char* name, float minX, float minY, float width,
			float height, float startX, float startY, float startAngle);

	static void writeIaPath(const char* zone1Name, const char* zone2Name, float x, float y );
};


#endif
