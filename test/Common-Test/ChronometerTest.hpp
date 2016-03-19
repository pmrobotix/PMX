/*!
 * \file
 * \brief Définition de la classe ChronometerTest.
 */

#ifndef TEST_CHRONOMETER_TEST_HPP
#define	TEST_CHRONOMETER_TEST_HPP

#include "../../src/Log/LoggerFactory.hpp"
#include "../Suite/DevUnitTest.hpp"

namespace test
{
    /*!
     * \brief Teste la classe \ref ChronometerTest.
     */
    class ChronometerTest : public DevUnitTest
    {
    private:

        /*!
         * \brief Retourne le \ref Logger associé à la classe \ref ChronometerTest.
         */
        static const logs::Logger & logger()
        {
            static const logs::Logger & instance = logs::LoggerFactory::logger("test::ChronometerTest");
            return instance;
        }

    public:

        /*!
         * \brief Constructeur par défaut.
         */
        ChronometerTest() : DevUnitTest("ChronometerTest")
        {
        }

        /*!
         * \brief Destructeur de la classe.
         */
        virtual inline ~ChronometerTest()
        {
        }

        void testWithClock();

        void testWithGettimeofday();

        void testTimer();

        void testTimerTickMin();

        void testTimerMiPeriodCount();

        void testTimerCheck();

        void testTimerNanosleep();

        virtual void suite();
    };
}

#endif

