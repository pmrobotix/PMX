/*!
 * \file
 * \brief Définition de la classe SvgWriter.
 */

#ifndef SVGWRITER_HPP_
#define	SVGWRITER_HPP_

#include <sstream>
#include <string>

#include "../Common/Utils/PointerList.hpp"
#include "LoggerFactory.hpp"

/*!
 * \brief Wrapper pour la génération de fichier svg via le système de log.
 */
class SvgWriter
{
private:

	/*
	 * Id of the associated robot.
	 */
	std::string id_;

	utils::PointerList<std::string> symbol_list_;

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
		std::ostringstream s;
		s << "Svg4" << id_;
		const logs::Logger & svg_ = logs::LoggerFactory::logger(s.str());
		return svg_;
	}

	void addDefsSymbol(std::string symbol);

	void beginHeader();

	void endHeader();

	void writeText(double x_mm, double y_mm, std::string text);

	void writeTextCustom(double x_mm,
			double y_mm,
			std::string text,
			std::string color,
			std::string fontsize);

	virtual void writePosition(double x_mm, double y_mm, double angle_rad, std::string symbol) = 0;

	virtual void writeZone(const char* name,
			float minX_mm,
			float minY_mm,
			float width_mm,
			float height_mm,
			float startX_mm,
			float startY_mm,
			float startAngle_rad) = 0;

	virtual void writeIaPath(const char* zone1Name,
			const char* zone2Name,
			float x_mm,
			float y_mm) = 0;


};

#endif
