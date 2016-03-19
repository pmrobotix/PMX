/*!
 * \file
 * \brief Implémentation de la classe LoggerTest.
 */

#include "../../src/Log/Logger.hpp"
#include "LoggerTest.hpp"

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

void test::LoggerTest::suite()
{
	testLogger();
}
