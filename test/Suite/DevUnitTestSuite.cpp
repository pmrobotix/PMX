/*!
 * \file
 * \brief Implementation de la classe UnitTestSuite.
 */

#include "DevUnitTestSuite.hpp"

#include <sstream>

#include "../../src/Log/Exception.hpp"
#include "../../src/Log/Logger.hpp"
#include "DevUnitTestAppender.hpp"

DevUnitTestSuite::DevUnitTestSuite()
		: tests_()
{
}

void DevUnitTestSuite::run()
{
	logger().info("Start Unit tests");
	DevUnitTestAppender* appender = (DevUnitTestAppender*) &logger().appender();

	appender->flush();
	utils::PointerList<DevUnitTest *>::iterator i = tests_.begin();
	for (; i != tests_.end(); i++)
	{
		appender->increaseIndent();
		DevUnitTest * test = *i;

		bool succeed = 0;
		try
		{
			test->suite();
			succeed = appender->messages().size() == 0;
		} catch (Exception* exception)
		{
			std::ostringstream oss;
			oss << "Le test à généré une exception: " << exception->what();
			test->fail(oss.str());
			succeed = false;
		} catch (std::exception* exception)
		{
			std::ostringstream oss;
			oss << "Le test à généré une exception: " << exception->what();
			test->fail(oss.str());
			succeed = false;
		}

		logger().info() << (succeed ? "[OK]   " : "[FAIL] ") << "Execution de <" << test->name() << ">" << logs::end;

		appender->decreaseIndent();
		appender->flush();

	}

	logger().info("End of Unit tests");
}
