/*!
 * \file
 * \brief Définition de la classe TimerTest.
 */

#ifndef TEST_TIMER_TEST_HPP
#define	TEST_TIMER_TEST_HPP

#include "../../src/Common/Utils/Timer.hpp"

#include "../../src/Common/Utils/ITimerListener.hpp"
#include "../Suite/UnitTest.hpp"

namespace test
{
/*!
 * \brief Teste la classe \ref TimerTest.
 */
class TimerTest: public UnitTest, utils::ITimerListener
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref test::TimerTest.
	 */
	static const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("test::TimerTest");
		return instance;
	}

	utils::Timer timer_;

public:

	/*!
	 * \brief Constructeur par défaut.
	 */
	TimerTest();

	/*!
	 * \brief Destructeur de la classe.
	 */
	virtual inline ~TimerTest()
	{
	}

	virtual void suite();

	void gettime();

	virtual void onTimer(utils::Chronometer chrono);

	virtual void onTimerEnd(utils::Chronometer chrono);

	void startTimer();

	void stopTimer();
};
}

#endif

