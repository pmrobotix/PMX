/*!
 * \file
 * \brief Implementation de la classe UnitTestSuite.
 */

#include <cstdio>
#include <sstream>

#include "../../src/Log/Appender/MemoryAppender.hpp"
#include "../../src/Log/Exception.hpp"
#include "../../src/Log/Logger.hpp"
#include "UnitTestSuite.hpp"
#include "UnitTestAppender.hpp"

UnitTestSuite::UnitTestSuite() :
		tests_()
{
}

void UnitTestSuite::run()
{
	logger().info("Start Unit tests");
	UnitTestAppender* appender = (UnitTestAppender*) &logger().appender();

	appender->flush();
	utils::PointerList<UnitTest *>::iterator i = tests_.begin();
	for (; i != tests_.end(); i++)
	{
		UnitTest *test = *i;

		appender->increaseIndent();

		logger().info() << "Début d'éxecution de <" << test->name() << "> "
						<< logs::end;
		appender->increaseIndent();
		bool succeed = 0;

		try
		{
			test->suite();
			succeed = appender->messages().size() == 0;
		} catch (logs::Exception* exception)
		{
			std::ostringstream oss;
			oss << "Le test a généré une exception: " << exception->what();
			test->fail(oss.str());
			succeed = false;
		} catch (std::exception* exception)
		{
			std::ostringstream oss;
			oss << "Le test a généré une exception: " << exception->what();
			test->fail(oss.str());
			succeed = false;
		}
		appender->decreaseIndent();
		logger().info() << "Fin d'éxecution de <" << test->name() << "> "
				//<< (succeed ? "[OK]   " : "[FAIL] ")
				<< logs::end;

		appender->decreaseIndent();
		appender->flush();

	}

	logger().info("End of Unit tests");
}
