/*!
 * \file
 * \brief Implémentation de la classe DevUnitTest.
 */

#include "DevUnitTest.hpp"

#include "../../src/Log/Logger.hpp"
#include "../../src/Log/LoggerFactory.hpp"
#include "DevUnitTestAppender.hpp"

DevUnitTest::DevUnitTest(const std::string & name)
{
	name_ = name;
}

void DevUnitTest::assert(bool test, const std::string & message)
{
	if (!test)
	{
		fail(message);
	}
}

void DevUnitTest::fail(const std::string & message)
{
	logger().error() << name() << ":" << message << logs::end;
}

void DevUnitTest::expectedError(const std::string & expected)
{
	this->expectedError(expected, "An error was expected byt was not raised");
}

void DevUnitTest::expectedError(const std::string & expected, const std::string & message)
{
	logs::Logger* rootLogger = logs::LoggerFactory::instance().rootLogger();
	DevUnitTestAppender* appender = (DevUnitTestAppender*) &rootLogger->appender();

	if (!appender->expectedError(expected))
	{
		this->fail(message);
	}
}
