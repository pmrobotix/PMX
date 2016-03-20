/*!
 * \file
 * \brief Implémentation de la classe LoggerTest.
 */

#include "../../src/Log/Logger.hpp"
#include "LoggerTest.hpp"

#include <cmath>

#include "../../src/Log/Appender/SvgWriter.hpp"
#include "../../src/Log/LoggerFactory.hpp"

void test::LoggerTest::testLogger()
{
	const logs::Logger & logger = logs::LoggerFactory::logger("test::LoggerTest");

	logger.debug("debug - message de test classique");
	logger.debug("debug - 2ème message de test classique");
	logger.info("info - message de test classique");
	logger.warn("warn - message de test classique");
	logger.error("error - message de test classique");

	logger.debug() << "debug - message de test en flux" << logs::end;
	logger.info() << "info - message de test en flux" << logs::end;
	logger.warn() << "warn - message de test en flux" << logs::end;
	logger.error() << "error - message de test en flux" << logs::end;

	this->expectedError("test::LoggerTest ERROR error - message de test classique");
	this->expectedError("test::LoggerTest ERROR error - message de test en flux");

}

void test::LoggerTest::testLoggerSvg()
{
	const logs::Logger & svg = logs::LoggerFactory::logger("test::LoggerTestSvg");

	if (svg.isActive(logs::Level::INFO))
	{
		SvgWriter::beginHeader();
		svg.info() << "<circle cx=\""
				<< 20
				<< "\" cy=\""
				<< 20
				<< "\" r=\"20\" stroke=\"blue\" fill=\"none\"  />"
				<< logs::end;

		SvgWriter::writePosition(150, 1250, 0, SVG_POS_ROBOT);

		SvgWriter::writePosition(200, 500, (90 * M_PI) / 180, SVG_POS_ROBOT);

		SvgWriter::writePosition(500, 800, (190 * M_PI) / 180, SVG_POS_ROBOT);
		SvgWriter::endHeader();
	}

}

void test::LoggerTest::suite()
{
	testLogger();
	testLoggerSvg();
}
