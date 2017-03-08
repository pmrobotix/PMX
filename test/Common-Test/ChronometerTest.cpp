/*!
 * \file
 * \brief Implémentation de la classe ChronometerTest.
 */

#include "../../src/Common/Utils/Chronometer.hpp"
#include "ChronometerTest.hpp"

#include <signal.h>
#include <unistd.h>
#include <cstdio>

#include "../../src/Log/Logger.hpp"

extern "C"
{

//refer to http://cc.byexamples.com/2007/05/25/nanosleep-is-better-than-sleep-and-usleep/
void sigfunc(int)
{

}

int nsleep(long miliseconds)
{
	struct timespec req, rem;

	if (miliseconds > 999)
	{
		req.tv_sec = (int) (miliseconds / 1000); /* Must be Non-Negative */
		req.tv_nsec = (miliseconds - ((long) req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
	}
	else
	{
		req.tv_sec = 0; /* Must be Non-Negative */
		req.tv_nsec = miliseconds * 1000000; /* Must be in range of 0 to 999999999 */
	}

	return nanosleep(&req, &rem);
}

int __nsleep(const struct timespec *req, struct timespec *rem)
{
	struct timespec temp_rem;
	if (nanosleep(req, rem) == -1)
	{
		__nsleep(rem, &temp_rem);
		return 0;
	}
	else
		return 1;
}

int msleep2(unsigned long milisec)
{
	struct timespec req =
	{ 0, 0 }, rem =
	{ 0, 0 };
	time_t sec = (int) (milisec / 1000);
	milisec = milisec - (sec * 1000);
	req.tv_sec = sec;
	req.tv_nsec = milisec * 1000000L;
	__nsleep(&req, &rem);
	return 1;
}

int msleep(unsigned long milisec)
{
	struct timespec req =
	{ 0, 0 };
	time_t sec = (int) (milisec / 1000);
	milisec = milisec - (sec * 1000);
	req.tv_sec = sec;
	req.tv_nsec = milisec * 1000000L;
	while (nanosleep(&req, &req) == -1)
		continue;
	return 1;
}

int _usleep(unsigned long microsec)
{
	struct timespec req =
	{ 0, 0 };
	time_t sec = (int) (microsec / 1000000);
	microsec = microsec - (sec * 1000000);
	req.tv_sec = sec;
	req.tv_nsec = microsec * 1000L;
	while (nanosleep(&req, &req) == -1)
		continue;
	return 1;
}

}

using namespace test;

void test::ChronometerTest::suite()
{
	testWithClock();
	testWithGettimeofday();
	testTimer();
	testTimerTickMin();
	testTimerMiPeriodCount();
	testTimerCheck();
	testTimerNanosleep();

}

void test::ChronometerTest::testTimerNanosleep()
{
	//recupere la resolution de la clock
	struct timespec res;

	if (clock_getres( CLOCK_REALTIME, &res) == -1)
	{
		//perror( "clock get resolution" );
		logger().error() << "ERROR clock get resolution" << logs::end;
	}
	//printf( "\n",res.tv_nsec / 1000 );
	logger().info() << "clock_getres() : Resolution minimum is "
			<< res.tv_nsec / 1000 << " micro seconds." << logs::end;

	logger().info()
			<< " testTimerNanosleep : Comparaison d'attente avec nanosleep/usleep"
			<< logs::end;

	struct sigaction sa;
	sa.sa_handler = &sigfunc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);

	utils::Chronometer chrono;
	chrono.start();
	_usleep(1000);
	chrono.stop();
	logger().info() << "Wait 1 ms with nanosleep() : "
			<< chrono.getElapsedTimeInMicroSec() << logs::end;

	chrono.start(); //reset
	_usleep(10000);
	chrono.stop();
	logger().info() << "Wait 10 ms with nanosleep() : "
			<< chrono.getElapsedTimeInMicroSec() << logs::end;

	chrono.start(); //reset
	_usleep(75000);
	chrono.stop();
	logger().info() << "Wait 75 ms with nanosleep() : "
			<< chrono.getElapsedTimeInMicroSec() << logs::end;

	chrono.start(); //reset
	_usleep(300000);
	chrono.stop();
	logger().info() << "Wait 300 ms with nanosleep() : "
			<< chrono.getElapsedTimeInMicroSec() << logs::end;

	chrono.start(); //reset
	usleep(1000);
	chrono.stop();
	logger().info() << "usleep(1000) : " << chrono.getElapsedTimeInMicroSec()
			<< logs::end;

	chrono.start(); //reset
	usleep(10000);
	chrono.stop();
	logger().info() << "usleep(10000) : " << chrono.getElapsedTimeInMicroSec()
			<< logs::end;

	chrono.start(); //reset
	usleep(75000);
	chrono.stop();
	logger().info() << "usleep(75000) : " << chrono.getElapsedTimeInMicroSec()
			<< logs::end;

	chrono.start(); //reset
	usleep(300000);
	chrono.stop();
	logger().info() << "usleep(300000) : " << chrono.getElapsedTimeInMicroSec()
			<< logs::end;

	logger().info()
			<< "Conclusion : nanosleep n'est pas plus précis. Le parametre High Res Timer doit etre activé à la compilation du noyau."
			<< logs::end;

	//todo : test clock_nanosleep et setitimer à faire

}

void test::ChronometerTest::testTimerCheck()
{
	logger().info()
			<< ":testTimerCheck : Attente de 1ms par wait system (while)"
			<< logs::end;

	utils::Chronometer chrono;
	chrono.setTimer(1000);

	//Test avec boucle while
	while (1)
	{
		if (chrono.checkTimer() == 1)
		{
			logger().info() << "endtime:" << chrono.getElapsedTimeInMicroSec()
					<< logs::end;
			break;
		}
	}
	logger().info() << "end" << logs::end;
}

/*!
 * \brief Test des temps avec la methode Clock(): donne le minimum de resolution
 */
void test::ChronometerTest::testWithClock()
{

	clock_t t1, t2;
	t1 = t2 = clock();

	// loop until t2 gets a different value
	while (t1 == t2)
	{
		t2 = clock();
	}

	// print resolution of clock()
	logger().info() << "testWithClock: Resolution minimum avec fct Clock():"
			<< (double) (t2 - t1) / CLOCKS_PER_SEC * 1000 << " ms."
			<< logs::end;

	logger().info() << "CLOCKS_PER_SEC: " << CLOCKS_PER_SEC << " ; CLOCK() : "
			<< clock() << logs::end;
}

/*!
 * \brief test des temps avec la methode Gettimeofday().
 */
void test::ChronometerTest::testWithGettimeofday()
{

	timeval t1, t2;
	double elapsedTime;

	// start timer
	gettimeofday(&t1, NULL);

	// do nothing

	// stop timer
	gettimeofday(&t2, NULL);

	// compute and print the elapsed time in millisec
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; // sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms

	logger().info()
			<< "testWithGettimeofday: Resolution minimum avec fct gettimeofday(): "
			<< elapsedTime << " ms." << logs::end;

	gettimeofday(&t1, NULL);
	gettimeofday(&t2, NULL);

	while ((t2.tv_sec * 1000000 + t2.tv_usec)
			< t1.tv_sec * 1000000 + t1.tv_usec + 1000)
	{
		gettimeofday(&t2, NULL);
	}

	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; // sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	logger().info()
			<< ":testWithGettimeofday: Resolution 1000us avec fct gettimeofday(): "
			<< elapsedTime << " ms." << logs::end;
}

/*!
 * \brief test de la classe Chronometer.
 */
void test::ChronometerTest::testTimer()
{
	logger().info()
			<< "testTimer: utilisation des fonctions du Chronometer start usleep stop"
			<< logs::end;
	utils::Chronometer chrono;

	// start timer
	chrono.start();
	// do something
	usleep(1000);
	// stop timer
	chrono.stop();
	// print the elapsed time
	logger().info() << "Wait 1000us with usleep(): "
			<< chrono.getElapsedTimeInSec() << " s. "
			<< chrono.getElapsedTimeInMilliSec() << " ms. "
			<< chrono.getElapsedTimeInMicroSec() << " us. " << logs::end;

	chrono.start();
	usleep(10000);
	chrono.stop();
	logger().info() << "Wait 10000us with usleep(): "
			<< chrono.getElapsedTimeInSec() << " s. "
			<< chrono.getElapsedTimeInMilliSec() << " ms. "
			<< chrono.getElapsedTimeInMicroSec() << " us. " << logs::end;

	chrono.start();
	usleep(75000);
	chrono.stop();
	logger().info() << "Wait 75000us with usleep(): "
			<< chrono.getElapsedTimeInSec() << " s. "
			<< chrono.getElapsedTimeInMilliSec() << " ms. "
			<< chrono.getElapsedTimeInMicroSec() << " us." << logs::end;

}

/*!
 * \brief affiche le temps minimum (tick) pendant 1ms.
 */
void test::ChronometerTest::testTimerTickMin()
{
	//std::cout.sync_with_stdio (false);
	logger().info()
			<< " testTimerTickMin: Check Chronometer.getElapsedTimeInMicroSec toutes les millisecondes "
			<< logs::end;

	utils::Chronometer timer;
	std::ostringstream oss;

	timer.start();

	double tick1 = timer.getElapsedTimeInMicroSec();
	double tick2 = tick1;

	while (timer.getElapsedTimeInMilliSec() < 1) // boucle sur 1 ms
	{
		oss << (tick2 - tick1) << " ";
		tick1 = tick2;
		tick2 = timer.getElapsedTimeInMicroSec();
	}

	logger().info() << oss.str() << " (en us)" << logs::end;
}

/*!
 * \brief affiche le temps minimum, maximum, moyenne de l'ensemble des demi-periodes generees
 */
void test::ChronometerTest::testTimerMiPeriodCount()
{
	//std::cout.sync_with_stdio(false);
	logger().info()
			<< " testTimerMiPeriodCount: Min,Max,Moy de la periode generee "
			<< logs::end;
	int tempsTotalEnMilliSec = 1000;
	int tempsMiPeriodeEnMicroSec = 1000;

	utils::Chronometer timer;
	utils::Chronometer timerTotal;
	bool value = false;
	int i = 0;
	long max = 0;
	long min = tempsMiPeriodeEnMicroSec * 10;
	long sommetot = 0;

	timerTotal.start();
	while (timerTotal.getElapsedTimeInMilliSec() <= tempsTotalEnMilliSec)
	{
		timer.start();
		while (timer.getElapsedTimeInMicroSec() <= tempsMiPeriodeEnMicroSec)
		{
		}

		if (value == true)
		{
			value = false;
		}
		else
		{
			value = true;
		}
		long miperiode = timer.getElapsedTimeInMicroSec();
		//strout << value << ":" << miperiode << " " << std::ends;
		if (miperiode > max)
		{
			max = miperiode;
		}
		else if (miperiode < min && miperiode != 0)
		{
			min = miperiode;
		}
		sommetot += miperiode;
		i++;
		timer.stop();
	}
	long tot = timerTotal.getElapsedTimeInMicroSec();
	timerTotal.stop();

	//calcul de la moyenne
	long moy = tot / i;

	// print the elapsed time in millisec
	logger().info() << "timerTotal:" << tot << " us." << logs::end;
	logger().info() << "sommetot  :" << sommetot << logs::end;
	logger().info() << "Nb        :" << i << logs::end;
	logger().info() << "Max       :" << max << logs::end;
	logger().info() << "Min       :" << min << logs::end;
	logger().info() << "moytimer  :" << moy << logs::end;
	logger().info() << "moysomme  :" << sommetot / i << logs::end;

}

