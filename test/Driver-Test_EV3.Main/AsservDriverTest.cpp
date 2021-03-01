/*!
 * \file
 * \brief Implémentation de la classe AsservDriverTest.
 */

#include "AsservDriverTest.hpp"

#include <unistd.h>
#include <cstdint>
#include <chrono>

#include "../../src/Common/Utils/Chronometer.hpp"
#include "../../src/Log/Logger.hpp"

using namespace std::chrono;

void test::AsservDriverTest::suite()
{
    testcodeurs();
    //testcodeurs2();
    //this->benchmarkPosix();

    //benchmarkSpeedMotor();
}
void test::AsservDriverTest::benchmarkSpeedMotor()
{
    int lastCountD = 0, countD = 0, tempextD = 0, tempintD = 0, lastintCountD = 0;
    int lastCountG = 0, countG = 0, tempextG = 0, tempintG = 0, lastintCountG = 0;

    long tlastdiffperiod = 0;
    int tboucle = 40000;
    int diff = 0;
    int c = 0;

    logger().info() << "AsservDriverTest::benchmarkSpeedMotor() Go" << logs::end;
    asservdriver_->resetEncoders();

    logger().info() << "speedD_ticks/s ;" << " turn/s ;" << logs::end;

    utils::Chronometer chrono("AsservDriverTest::testcodeurs()");
    //appel 1ere fois
    tempextD = asservdriver_->getRightExternalEncoder();
    tempextG = asservdriver_->getLeftExternalEncoder();
    tempintD = asservdriver_->getRightInternalEncoder();
    tempintG = asservdriver_->getLeftInternalEncoder();

    //depart
    asservdriver_->setMotorLeftPower(1049, 0); //1050 max
    asservdriver_->setMotorRightPower(1049, 0); //1050 max

    //asservdriver_->setMotorLeftPower(100, -1); //1050 max
    //asservdriver_->setMotorRightPower(100, -1); //1050 max

    chrono.start();
    while (1) {
        long tstart = chrono.getElapsedTimeInMicroSec();

        tempextD = asservdriver_->getRightExternalEncoder();
        tempextG = asservdriver_->getLeftExternalEncoder();

        tempintD = asservdriver_->getRightInternalEncoder();
        tempintG = asservdriver_->getLeftInternalEncoder();

        int countintD = tempintD - lastintCountD;
        lastintCountD = tempintD;
        int countintG = tempintG - lastintCountG;
        lastintCountG = tempintG;

        countD = tempextD - lastCountD;
        lastCountD = tempextD;

        countG = tempextG - lastCountG;
        lastCountG = tempextG;

        //roue de 55mm
        //left= 2075 ; right= 2070 x=502 y=-9 a=-1.05115 mleft= -1073 ; mright= -1071
        //internal nb tick = 4150 pour un metre
        //external nb tick = 2145

        //Roue de 70cm
        //left= 2066 ; right= 2082 x=503 y=-3 a=0.261437 mleft= -858 ; mright= -856


        float speedextD_ticks_par_s = (float) countD * 1000000.0 / (float) (tlastdiffperiod);
        float speedextD_ticksm_par_s = speedextD_ticks_par_s * 858.0 / 2075.0;

        float speedintD_ticks_par_s = (float) countintD * 1000000.0 / (float) (tlastdiffperiod);
        float turn_per_secD = speedintD_ticks_par_s / 360.0;

        float speedextG_ticks_par_s = (float) countG * 1000000.0 / (float) (tlastdiffperiod);
        float speedextG_ticksm_par_s = speedextG_ticks_par_s * 858.0 / 2075.0;

        float speedintG_ticks_par_s = (float) countintG * 1000000.0 / (float) (tlastdiffperiod);
        float turn_per_secG = speedintG_ticks_par_s / 360.0;

        logger().info() << c << " " << tlastdiffperiod << " ; travail=" << diff
                //<< logs::end;

                //logger().info()
                << " ; diffextD= ; " << countD << " ; diffintD= ; " << countintD << " ; int/extD= ; "
                << speedintD_ticks_par_s << "; " << speedextD_ticksm_par_s << " ; " << turn_per_secD * 60.0
                //<< logs::end;
                //logger().info()
                << " ; diffextG= ; " << countG << " ; diffintG= ; " << countintG
                << " ; int/extG= ; "
                                << speedintG_ticks_par_s << "; " << speedextG_ticksm_par_s << " ; " << turn_per_secG * 60.0
                << logs::end;

        long tstop = chrono.getElapsedTimeInMicroSec();
        diff = tstop - tstart;
        if (diff <= tboucle)
            usleep(tboucle - diff - 300);

        tlastdiffperiod = chrono.getElapsedTimeInMicroSec() - tstart;

        /*
         long t10 = chrono.getElapsedTimeInMicroSec();
         long last_period = t10 - tlast;

         tempD = asservdriver_->getRightExternalEncoder();
         tempG = asservdriver_->getLeftExternalEncoder();

         tempintD = asservdriver_->getRightInternalEncoder();
         tempintG = asservdriver_->getLeftInternalEncoder();


         int countintD = tempintD - lastintCountD;
         lastintCountD = tempintD;

         countD = tempD - lastCountD;
         lastCountD = tempD;

         countG = tempG - lastCountG;
         lastCountG = tempG;

         float speedD_ticks_par_s = (float) countD * 1000000.0 / (float) (last_period);
         float turn_per_sec = speedD_ticks_par_s / 360.0;

         logger().info() << t10 << " ; " << last_period << " ; speed= " << speedD_ticks_par_s << " ; " << turn_per_sec
         << " ; diffextD= " << countD << " ; tempD= " << tempD << " ; diffintD= " << countintD << logs::end;

         int timeremaining = 20000 - (chrono.getElapsedTimeInMicroSec() - t10);
         //logger().info() << timeremaining << logs::end;
         if (timeremaining > 0)
         usleep(timeremaining);

         tlast = t10;
         */
        if (chrono.getElapsedTimeInSec() >= 1)
            break;

        c++;
    }
    asservdriver_->setMotorLeftPower(0, 0); //1050 max
    asservdriver_->setMotorRightPower(0, 0); //1050 max
    asservdriver_->stopMotorLeft();
    asservdriver_->stopMotorRight();
}
void test::AsservDriverTest::benchmarkPosix()
{
    logger().info() << "AsservDriverTest::benchmarkPosix() Go" << logs::end;

    utils::Chronometer chrono("AsservDriverTest::testcodeurs()");
    chrono.start();
    while (1) {
        long t0 = chrono.getElapsedTimeInMicroSec();
        asservdriver_->setMotorLeftPower(40, -1); //percent
        long t1 = chrono.getElapsedTimeInMicroSec();
        asservdriver_->setMotorRightPower(40, -1); //percent
        long t2 = chrono.getElapsedTimeInMicroSec();

        logger().info() << "t0-t1=" << t1 - t0 << " t2-t1=" << t2 - t1
        //<< " t3-t2=" << t3 - t2
                << logs::end;

        usleep(10000);
        if (chrono.getElapsedTimeInSec() >= 1)
            break;
    }

//    long left = 0;
//    long currentL = 0;
//    long currentR = 0;
//    long right = 0;
//    asservdriver_->resetExternalEncoders();
//    utils::Chronometer chrono("AsservDriverTest");
//    chrono.start();
//
//    asservdriver_->setMotorLeftPower(20, 0); //percent
//    asservdriver_->setMotorRightPower(10, 0); //percent
//
//    while (1) {
//
//        currentL = asservdriver_->getLeftExternalEncoder();
//        currentR = asservdriver_->getRightExternalEncoder();
//        if ((currentR != right) || (currentL != left)) {
//            logger().info() << "currentL = " << currentL << "   currentR = " << currentR << logs::end;
//            left = currentL;
//            right = currentR;
//        }
//
//        if (chrono.getElapsedTimeInSec() >= 10)
//            break;
//    }
//
    asservdriver_->stopMotorLeft();
    asservdriver_->stopMotorRight();

    /*sleep(1);
     asservdriver_->setMotorLeftPower(200, 1000); //1050 max
     asservdriver_->setMotorRightPower(1049, 1000); //1050 max
     sleep(3);
     asservdriver_->stopMotorLeft();
     asservdriver_->stopMotorRight();*/

    this->assert(true, "OK");
}

void test::AsservDriverTest::testcodeurs()
{
    logger().info() << "testcodeurs()..." << logs::end;
    int32_t lastCountD = 0, countD = 0;
    int32_t lastCountG = 0, countG = 0;

    utils::Chronometer chrono("AsservDriverTest::testcodeurs()");
    chrono.start();
    while (1) {

        long t0 = chrono.getElapsedTimeInMicroSec();
        int32_t tempD = asservdriver_->getRightExternalEncoder();

        long t1 = chrono.getElapsedTimeInMicroSec();

        int32_t tempG = asservdriver_->getLeftExternalEncoder();

        long t2 = chrono.getElapsedTimeInMicroSec();

        countD = tempD - lastCountD;
        lastCountD = tempD;

        countG = tempG - lastCountG;
        lastCountG = tempG;

        long t3 = chrono.getElapsedTimeInMicroSec();

        logger().info() << "t0-t1=" << t1 - t0 << " t2-t1=" << t2 - t1 << " t3-t2=" << t3 - t2 << "      D=" << countD
                << " currentD= " << tempD << "   ___   G=" << countG << " currentG= " << tempG << logs::end;

        usleep(10000);
        if (chrono.getElapsedTimeInSec() >= 1)
            break;
    }

}

void test::AsservDriverTest::testcodeurs2()
{
    logger().info() << "testcodeurs()..." << logs::end;
    int32_t lastCountD = 0, countD = 0;
    int32_t lastCountG = 0, countG = 0;

    //utils::Chronometer chrono("AsservDriverTest::testcodeurs()");
    //chrono.start();
    auto start = system_clock::now();
    while (1) {

        auto t0 = system_clock::now();
        int32_t tempD = asservdriver_->getRightExternalEncoder();

        auto t1 = system_clock::now();
        int32_t tempG = asservdriver_->getLeftExternalEncoder();

        auto t2 = system_clock::now();
        countD = tempD - lastCountD;
        lastCountD = tempD;

        countG = tempG - lastCountG;
        lastCountG = tempG;

        auto t3 = system_clock::now();
        logger().info() << "  t0-t1=" << (duration_cast<microseconds>(t1 - t0).count()) << " t2-t1="
                << (duration_cast<microseconds>(t2 - t1).count()) << " t3-t2="
                << (duration_cast<microseconds>(t3 - t2).count()) << "      D=" << countD << " currentD= " << tempD
                << "   ___   G=" << countG << " currentG= " << tempG << logs::end;

        usleep(10000);
        auto stop = system_clock::now();
        if ((duration_cast<microseconds>(stop - start).count()) >= 1000000)
            break;
    }

}
