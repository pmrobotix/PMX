//#include "ch.h"
//#include "hal.h"
//#include "shell.h"
//#include <chprintf.h>
//#include <cstdlib>
//#include <cstring>
//#include <cstdio>
//#include <cfloat>
//
////#include "../../blockingDetector/SpeedErrorBlockingDetector.h"
//#include "blockingDetector/OldSchoolBlockingDetector.h"
//#include "raspIO.h"
//#include "util/asservMath.h"
//#include "util/chibiOsAllocatorWrapper.h"
//#include "AsservMain.h"
//#include "commandManager/CommandManager.h"
//#include "SpeedController/SpeedController.h"
//#include "SpeedController/AdaptativeSpeedController.h"
//#include "Encoders/QuadratureEncoder.h"
//#include "motorController/Md22.h"
//#include "Odometry.h"
//#include "USBStream.h"
//#include "AccelerationLimiter/SimpleAccelerationLimiter.h"
//#include "AccelerationLimiter/AdvancedAccelerationLimiter.h"
//#include "Pll.h"
//#include "Encoders/MagEncoders.h"
//
//
//#define ENABLE_SHELL
//
//#define ASSERV_THREAD_FREQUENCY (200) //200=>5ms 300=>3ms
//#define ASSERV_THREAD_PERIOD_S (1.0/ASSERV_THREAD_FREQUENCY)
//#define ASSERV_POSITION_DIVISOR (5)
//
//#define ENCODERS_WHEELS_RADIUS_MM (39.93/2.0) // le rayon de vos roues codeuses 39.88, 39.93
//#define ENCODERS_WHEELS_DISTANCE_MM (234.4) //distance entre les 2 roues codeuses
//#define ENCODERS_TICKS_BY_TURN (16384) //nombre de ticks par tour de vos encodeurs.
//
//#define MAX_SPEED_MM_PER_SEC (1500)
//
//#define DIST_REGULATOR_KP (1.95)
//#define DIST_REGULATOR_MAX_ACC (900)
//#define DIST_REGULATOR_MIN_ACC (500)
//#define DIST_REGULATOR_HIGH_SPEED_THRESHOLD (500)
//
//
//#define ANGLE_REGULATOR_KP (400) //480
//#define ANGLE_REGULATOR_MAX_ACC (900)
//
//float speed_controller_right_Kp[NB_PI_SUBSET] = { 0.3, 0.2, 0.1};
//float speed_controller_right_Ki[NB_PI_SUBSET] = { 3.0, 4.2, 1.5};
//float speed_controller_right_SpeedRange[NB_PI_SUBSET] = { 20, 50, 60};
//
//float speed_controller_left_Kp[NB_PI_SUBSET] = { 0.3, 0.2, 0.1}; //0.08
//float speed_controller_left_Ki[NB_PI_SUBSET] = { 3.0, 4.2, 1.5}; //1.0
//float speed_controller_left_SpeedRange[NB_PI_SUBSET] = { 20, 50, 60};
//
//#define PLL_BANDWIDTH (100) //verifpour garder un minimum de variation sur la vitesse
//
//
//#define COMMAND_MANAGER_ARRIVAL_ANGLE_THRESHOLD_RAD (0.02)
//#define COMMAND_MANAGER_ARRIVAL_DISTANCE_THRESHOLD_mm (2.5)
//
//#define COMMAND_MANAGER_GOTO_RETURN_THRESHOLD_mm (20)
//#define COMMAND_MANAGER_GOTO_ANGLE_THRESHOLD_RAD (M_PI/8)
//#define COMMAND_MANAGER_GOTO_PRECISE_ARRIVAL_DISTANCE_mm (3)
//Goto::GotoConfiguration preciseGotoConf  = {COMMAND_MANAGER_GOTO_RETURN_THRESHOLD_mm, COMMAND_MANAGER_GOTO_ANGLE_THRESHOLD_RAD, COMMAND_MANAGER_GOTO_PRECISE_ARRIVAL_DISTANCE_mm};
//
//#define COMMAND_MANAGER_GOTO_WAYPOINT_ARRIVAL_DISTANCE_mm (20)
//Goto::GotoConfiguration waypointGotoConf  = {COMMAND_MANAGER_GOTO_RETURN_THRESHOLD_mm, COMMAND_MANAGER_GOTO_ANGLE_THRESHOLD_RAD, COMMAND_MANAGER_GOTO_WAYPOINT_ARRIVAL_DISTANCE_mm};
//
//#define COMMAND_MANAGER_GOTONOSTOP_TOO_BIG_ANGLE_THRESHOLD_RAD (M_PI/2)
//GotoNoStop::GotoNoStopConfiguration gotoNoStopConf = {COMMAND_MANAGER_GOTO_ANGLE_THRESHOLD_RAD, COMMAND_MANAGER_GOTONOSTOP_TOO_BIG_ANGLE_THRESHOLD_RAD, (100/DIST_REGULATOR_KP), 85};
//
//Md22::I2cPinInit md22PMXCardPinConf_SCL_SDA = {GPIOB, 6, GPIOB, 7};
//QuadratureEncoder::GpioPinInit qePMXCardPinConf_E1ch1_E1ch2_E2ch1_E2ch2 = {GPIOC, 6, GPIOA, 7, GPIOA, 5, GPIOB, 9};
//
//QuadratureEncoder *encoders;
//MagEncoders *encoders_ext;
//Md22 *md22MotorController;
//
//
//Regulator *angleRegulator;
//Regulator *distanceRegulator;
//
//Odometry *odometry;
//
//AdaptativeSpeedController *speedControllerRight;
//AdaptativeSpeedController *speedControllerLeft;
//
//Pll *rightPll;
//Pll *leftPll;
//
////SpeedErrorBlockingDetector *blockingDetection;
//OldSchoolBlockingDetector *blockingDetection;
//
//SimpleAccelerationLimiter *angleAccelerationlimiter;
//AdvancedAccelerationLimiter *distanceAccelerationLimiter;
//
//CommandManager *commandManager;
//AsservMain *mainAsserv;
//
//
//static void initAsserv()
//{
//    md22MotorController= new Md22(&md22PMXCardPinConf_SCL_SDA, false, false, false, 400000); //100k ou 400k
//    encoders = new QuadratureEncoder(&qePMXCardPinConf_E1ch1_E1ch2_E2ch1_E2ch2, false, true, false);
//    encoders_ext = new MagEncoders(false, false, true);
//
//
//    angleRegulator = new Regulator(ANGLE_REGULATOR_KP, MAX_SPEED_MM_PER_SEC);
//    distanceRegulator = new Regulator(DIST_REGULATOR_KP, MAX_SPEED_MM_PER_SEC);
//
//
//
//    rightPll = new Pll (PLL_BANDWIDTH);
//    leftPll = new Pll(PLL_BANDWIDTH);
//
//    odometry = new Odometry (ENCODERS_WHEELS_DISTANCE_MM, 0, 0);
//
//    speedControllerRight = new AdaptativeSpeedController(speed_controller_right_Kp, speed_controller_right_Ki, speed_controller_right_SpeedRange, 100, MAX_SPEED_MM_PER_SEC, ASSERV_THREAD_FREQUENCY);
//    speedControllerLeft = new AdaptativeSpeedController(speed_controller_left_Kp, speed_controller_left_Ki, speed_controller_left_SpeedRange, 100, MAX_SPEED_MM_PER_SEC, ASSERV_THREAD_FREQUENCY);
//
//
//    angleAccelerationlimiter = new SimpleAccelerationLimiter(ANGLE_REGULATOR_MAX_ACC);
//    distanceAccelerationLimiter = new AdvancedAccelerationLimiter(DIST_REGULATOR_MAX_ACC, DIST_REGULATOR_MIN_ACC, DIST_REGULATOR_HIGH_SPEED_THRESHOLD);
//
//    commandManager = new CommandManager( COMMAND_MANAGER_ARRIVAL_DISTANCE_THRESHOLD_mm, COMMAND_MANAGER_ARRIVAL_ANGLE_THRESHOLD_RAD,
//                                   preciseGotoConf, waypointGotoConf, gotoNoStopConf,
//                                   *angleRegulator, *distanceRegulator);
//
//    //blockingDetection = new SpeedErrorBlockingDetector(ASSERV_THREAD_PERIOD_S, *speedControllerRight, *speedControllerLeft, 1.0f, 666.f);
//    //threshold in mm/s
//    blockingDetection = new OldSchoolBlockingDetector(ASSERV_THREAD_PERIOD_S, md22MotorController, odometry, degToRad(1.0), 3, 200);
//
//    mainAsserv = new AsservMain( ASSERV_THREAD_FREQUENCY, ASSERV_POSITION_DIVISOR,
//                           ENCODERS_WHEELS_RADIUS_MM, ENCODERS_WHEELS_DISTANCE_MM, ENCODERS_TICKS_BY_TURN,
//                           *commandManager, *md22MotorController, *encoders_ext, *odometry,
//                           *angleRegulator, *distanceRegulator,
//                           *angleAccelerationlimiter, *distanceAccelerationLimiter,
//                           *speedControllerRight, *speedControllerLeft,
//                           *rightPll, *leftPll);
//
//
//}
//
//BaseSequentialStream *outputStream;
//BaseSequentialStream *outputStreamSd4;
//
//
///*
// *  As the dynamic allocation is disabled after init,
// *  use this semaphore to ensure that init is finished before
// *  disabling further dynamic allocation
// */
//static binary_semaphore_t asservStarted_semaphore;
//
//static THD_WORKING_AREA(waAsservThread, 512);
//static THD_FUNCTION(AsservThread, arg)
//{
//    (void) arg;
//    chRegSetThreadName("AsservThread");
//
//
//    md22MotorController->init();
//
//    encoders->init();
//    encoders->start();
//
//    encoders_ext->init();
//    encoders_ext->start();
//
//    USBStream::init();
//
//    chBSemSignal(&asservStarted_semaphore);
//
//    //desactivation au demarraga
//    mainAsserv->enableMotors(false);
//
//    mainAsserv->mainLoop();
//
////    while (true)
////    {
////        palClearPad(GPIOA, GPIOA_ARD_D12);
////        chThdSleepMilliseconds(200);
////        palSetPad(GPIOA, GPIOA_ARD_D12);
////        chThdSleepMilliseconds(200);
////    }
//}
//
//
//THD_WORKING_AREA(wa_shell, 2048);
//THD_WORKING_AREA(wa_controlPanel, 256);
//THD_WORKING_AREA(wa_shell_serie, 2048);
//THD_WORKING_AREA(wa_controlPanel_serie, 256);
//THD_FUNCTION(ControlPanelThread, p);
//
//char history_buffer[SHELL_MAX_HIST_BUFF];
//char *completion_buffer[SHELL_MAX_COMPLETIONS];
//
//float config_buffer[30];
//void asservCommandUSB(BaseSequentialStream *chp, int argc, char **argv);
//
//void asservCommandSerial();
//
//
//
//int main(void)
//{
//
//    halInit();
//    chSysInit();
//    //Config des PINs pour LEDs
//    palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL );
//    palSetPadMode(GPIOA, 9, PAL_MODE_OUTPUT_PUSHPULL );
//
//    //init de l'USB debug
//    sdStart(&SD2, NULL);
//    outputStream = reinterpret_cast<BaseSequentialStream*>(&SD2);
//
//    //config UART4 for raspIO
//    palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(8));
//    palSetPadMode(GPIOA, 1, PAL_MODE_ALTERNATE(8));
//    sdStart(&SD4, NULL);
//    outputStreamSd4 = reinterpret_cast<BaseSequentialStream*>(&SD4);
//    chprintf(outputStreamSd4,"Start OK\r\n");
//
//    //creation de tous les objets
//    initAsserv();
//
//    chBSemObjectInit(&asservStarted_semaphore, true);
//    chThdCreateStatic(waAsservThread, sizeof(waAsservThread), HIGHPRIO, AsservThread, NULL);
//    chBSemWait(&asservStarted_semaphore);
//
//
//    shellInit();
//
//    // Custom commands
//    const ShellCommand shellCommands[] = { { "asserv", &(asservCommandUSB) }, { nullptr, nullptr } };
//    ShellConfig shellCfg =
//    {
//        /* sc_channel */outputStream,
//        /* sc_commands */shellCommands,
//#if (SHELL_USE_HISTORY == TRUE)
//        /* sc_histbuf */history_buffer,
//        /* sc_histsize */sizeof(history_buffer),
//#endif
//#if (SHELL_USE_COMPLETION == TRUE)
//        /* sc_completion */completion_buffer
//#endif
//    };
//
//#ifdef ENABLE_SHELL
//    bool startShell = true;
//#else
//    bool startShell = false;
//#endif
//    if (startShell)
//    {
//
//        thread_t *shellThd = chThdCreateStatic(wa_shell, sizeof(wa_shell), LOWPRIO, shellThread, &shellCfg);
//        chRegSetThreadNameX(shellThd, "shell");
//
//        // Le thread controlPanel n'a de sens que quand le shell tourne
//        thread_t *controlPanelThd = chThdCreateStatic(wa_controlPanel, sizeof(wa_controlPanel), LOWPRIO, ControlPanelThread, nullptr);
//        chRegSetThreadNameX(controlPanelThd, "controlPanel");
//
//
//        thread_t *asserCmdSerialThread = chThdCreateStatic(wa_shell_serie, sizeof(wa_shell_serie), LOWPRIO, asservCommandSerial, nullptr);
//        chRegSetThreadNameX(asserCmdSerialThread, "asserv Command serial");
//
//        thread_t *controlPanelThdSerial = chThdCreateStatic(wa_controlPanel_serie, sizeof(wa_controlPanel_serie), LOWPRIO, asservPositionSerial, nullptr);
//        chRegSetThreadNameX(controlPanelThdSerial, "asserv position update serial");
//
//    }
//
//    deactivateHeapAllocation();
//
//    chThdSetPriority(LOWPRIO);
//
//    while (true)
//    {
//        palClearPad(GPIOA, GPIOA_ARD_D8);
//        palClearPad(GPIOA, GPIOA_ARD_D12);
//        chThdSleepMilliseconds(1000);
//        palSetPad(GPIOA, GPIOA_ARD_D8);
//        palSetPad(GPIOA, GPIOA_ARD_D12);
//        chThdSleepMilliseconds(1000);
//
//
//    }
//}
//
//
//void asservCommandUSB(BaseSequentialStream *chp, int argc, char **argv)
//{
//    auto printUsage = []()
//    {
//        chprintf(outputStream,"Usage :");
//        chprintf(outputStream," - asserv enablemotor 0|1\r\n");
//        chprintf(outputStream," - asserv enablepolar 0|1\r\n");
//        chprintf(outputStream," - asserv coders \r\n");
//        chprintf(outputStream," - asserv ext (coders) \r\n");
//        chprintf(outputStream," - asserv reset \r\n");
//        chprintf(outputStream," - asserv motorspeed [r|l] speed \r\n");
//        chprintf(outputStream," -------------- \r\n");
//        chprintf(outputStream," - asserv wheelspeedstep [r|l] [speed] [step time] \r\n");
//        chprintf(outputStream," -------------- \r\n");
//        chprintf(outputStream," - asserv robotfwspeedstep [speed] [step time] \r\n");
//        chprintf(outputStream," - asserv robotangspeedstep [speed] [step time] \r\n");
//        chprintf(outputStream," - asserv speedcontrol [r|l] [Kp] [Ki] \r\n");
//        chprintf(outputStream," - asserv angleacc delta_speed \r\n");
//        chprintf(outputStream," - asserv distacc delta_speed \r\n");
//        chprintf(outputStream," ------------------- \r\n");
//        chprintf(outputStream," - asserv addangle angle_rad \r\n");
//        chprintf(outputStream," - asserv anglereset\r\n");
//        chprintf(outputStream," - asserv anglecontrol Kp\r\n");
//        chprintf(outputStream," ------------------- \r\n");
//        chprintf(outputStream," - asserv adddist mm \r\n");
//        chprintf(outputStream," - asserv distreset\r\n");
//        chprintf(outputStream," - asserv distcontrol Kp\r\n");
//        chprintf(outputStream," -------------- \r\n");
//        chprintf(outputStream," - asserv addgoto X Y\r\n");
//        chprintf(outputStream," - asserv gototest\r\n");
//    };
//    (void) chp;
//
//    if (argc == 0)
//    {
//        printUsage();
//        return;
//    }
//
//    if (!strcmp(argv[0], "wheelspeedstep"))
//    {
//        char side = *argv[1];
//        float speedGoal = atof(argv[2]);
//        int time = atoi(argv[3]);
//        chprintf(outputStream, "setting fw robot speed %.2f rad/s for %d ms\r\n", speedGoal, time);
//
//        chprintf(outputStream, "setting wheel %s to speed %.2f rad/s for %d ms \r\n", (side == 'r') ? "right" : "left", speedGoal, time);
//
//        float speedRight = speedGoal;
//        float speedLeft = 0;
//        if (side == 'l')
//        {
//            speedLeft = speedGoal;
//            speedRight = 0;
//        }
//
//        mainAsserv->setWheelsSpeed(speedRight, speedLeft);
//        chThdSleepMilliseconds(time);
//        mainAsserv->setWheelsSpeed(0, 0);
//    }
//    else if (!strcmp(argv[0], "robotfwspeedstep"))
//    {
//        float speedGoal = atof(argv[1]);
//        int time = atoi(argv[2]);
//        chprintf(outputStream, "setting fw robot speed %.2f rad/s for %d ms\r\n", speedGoal, time);
//
//        mainAsserv->setRegulatorsSpeed(speedGoal, 0);
//        chThdSleepMilliseconds(time);
//        mainAsserv->setRegulatorsSpeed(0, 0);
//    }
//    else if (!strcmp(argv[0], "robotangspeedstep"))
//    {
//        float speedGoal = atof(argv[1]);
//        int time = atoi(argv[2]);
//        chprintf(outputStream, "setting angle robot speed %.2f rad/s for %d ms\r\n", speedGoal, time);
//
//        mainAsserv->setRegulatorsSpeed(0, speedGoal);
//        chThdSleepMilliseconds(time);
//        mainAsserv->setRegulatorsSpeed(0, 0);
//    }
//    else if (!strcmp(argv[0], "speedcontrol"))
//    {
//        char side = *argv[1];
//        float Kp = atof(argv[2]);
//        float Ki = atof(argv[3]);
//        uint8_t range = atof(argv[4]);
//
//        chprintf(outputStream, "setting speed control Kp:%.2f Ki:%.2f range:%d to side %c \r\n", Kp, Ki, range, side);
//
//        if (side == 'r')
//            speedControllerRight->setGains(Kp, Ki, range);
//        else if (side == 'l')
//            speedControllerLeft->setGains(Kp, Ki, range);
//    }
//    else if (!strcmp(argv[0], "angleacc"))
//    {
//        float acc = atof(argv[1]);
//        chprintf(outputStream, "setting angle acceleration limit to %.2f \r\n", acc);
//
//        angleAccelerationlimiter->setMaxAcceleration(acc);
//    }
//    else if (!strcmp(argv[0], "distacc"))
//    {
//        float acc_max = atof(argv[1]);
//        float acc_min = atof(argv[2]);
//        float acc_threshold = atof(argv[3]);
//        chprintf(outputStream, "setting distance acceleration limiter max %.2f min %.2f threshold %.2f \r\n", acc_max, acc_min, acc_threshold);
//
//        distanceAccelerationLimiter->setMaxAcceleration(acc_max);
//        distanceAccelerationLimiter->setMinAcceleration(acc_min);
//        distanceAccelerationLimiter->setHighSpeedThreshold(acc_threshold);
//    }
//    else if (!strcmp(argv[0], "addangle"))
//    {
//        float angle = atof(argv[1]);
//        chprintf(outputStream, "Adding angle %.2frad \r\n", angle);
//
//        mainAsserv->resetToNormalMode();
//        commandManager->addTurn(angle);
//    }
//    else if (!strcmp(argv[0], "anglereset"))
//    {
//        chprintf(outputStream, "Reseting angle accumulator \r\n");
//        angleRegulator->reset();
//    }
//    else if (!strcmp(argv[0], "distreset"))
//    {
//        chprintf(outputStream, "Reseting distance accumulator \r\n");
//        distanceRegulator->reset();
//    }
//    else if (!strcmp(argv[0], "adddist"))
//    {
//        float dist = atof(argv[1]);
//
//        mainAsserv->resetToNormalMode();
//        bool ok = commandManager->addStraightLine(dist);
//        chprintf(outputStream, "Adding distance %.2fmm %d\r\n", dist, ok );
//
//    }
//    else if (!strcmp(argv[0], "anglecontrol"))
//    {
//        float Kp = atof(argv[1]);
//        chprintf(outputStream, "setting angle Kp to %.2f \r\n", Kp);
//
//        angleRegulator->setGain(Kp);
//    }
//    else if (!strcmp(argv[0], "distcontrol"))
//    {
//        float Kp = atof(argv[1]);
//        chprintf(outputStream, "setting dist Kp to %.2f \r\n", Kp);
//
//        distanceRegulator->setGain(Kp);
//    }
//    else if (!strcmp(argv[0], "enablemotor"))
//    {
//        bool enable = !(atoi(argv[1]) == 0);
//        chprintf(outputStream, "%s motor output\r\n", (enable ? "enabling" : "disabling"));
//        mainAsserv->enableMotors(enable);
//    }
//    else if (!strcmp(argv[0], "coders"))
//    {
//        float deltaEncoderRight;
//        float deltaEncoderLeft;
//        encoders->getValues(&deltaEncoderRight, &deltaEncoderLeft);
//        chprintf(outputStream, "Encoders count R %d L %d \r\n", encoders->getRightEncoderTotalCount(), encoders->getLeftEncoderTotalCount());
//
//    }
//    else if (!strcmp(argv[0], "ext"))
//        {
//            float deltaEncoderRight;
//            float deltaEncoderLeft;
//            int32_t encoderRight, encoderLeft;
//            encoders_ext->getValues(&deltaEncoderRight, &deltaEncoderLeft);
//            encoders_ext->getEncodersTotalCount(&encoderRight, &encoderLeft);
//            chprintf(outputStream, "Encoders count R %d  L %d \r\n", encoderRight, encoderLeft);
//        }
//    else if (!strcmp(argv[0], "reset"))
//    {
//        mainAsserv->reset();
//        chprintf(outputStream, "asserv resetted \r\n");
//    }
//    else if (!strcmp(argv[0], "motorspeed"))
//    {
//        char side = *argv[1];
//        float speedGoal = atof(argv[2]);
//
//        chprintf(outputStream, "setting wheel %s to speed %.2f \r\n", (side == 'r') ? "right" : "left", speedGoal);
//
//        if (side == 'l')
//            md22MotorController->setMotorLeftSpeed(speedGoal);
//        else
//            md22MotorController->setMotorRightSpeed(speedGoal);
//    }
//    else if (!strcmp(argv[0], "enablepolar"))
//    {
//        bool enable = !(atoi(argv[1]) == 0);
//        chprintf(outputStream, "%s polar control\r\n", (enable ? "enabling" : "disabling"));
//
//        mainAsserv->enablePolar(enable);
//
//    }
//    else if (!strcmp(argv[0], "addgoto"))
//    {
//        float X = atof(argv[1]);
//        float Y = atof(argv[2]);
//        chprintf(outputStream, "Adding goto(%.2f,%.2f) consign\r\n", X, Y);
//
//        mainAsserv->resetToNormalMode();
//        commandManager->addGoTo(X, Y);
//    }
//    else if (!strcmp(argv[0], "gototest"))
//    {
//        mainAsserv->resetToNormalMode();
//        mainAsserv->limitMotorControllerConsignToPercentage(50);
//
//
//        commandManager->addGoTo(200, 0);
//        commandManager->addGoTo(300, -50);
//        commandManager->addGoTo(400, 100);
//        commandManager->addGoTo(400, 300);
//        commandManager->addGoTo(0, 300);
//
//        commandManager->addGoTo(0, 0);
//        commandManager->addGoToAngle(100,0);
//
//        commandManager->addGoToNoStop(200, 0);
//        commandManager->addGoToNoStop(300, -50);
//        commandManager->addGoToNoStop(400, 100);
//        commandManager->addGoToNoStop(400, 300);
//        commandManager->addGoToNoStop(0, 300);
//
//        commandManager->addGoTo(0, 0);
//        commandManager->addGoToAngle(100,0);
//
//    }
//    else if (!strcmp(argv[0], "get_config"))
//    {
//        uint8_t index = 0;
//
//        // SpeedControllerLeft
//        for( int i=0; i<NB_PI_SUBSET; i++)
//        {
//            speedControllerLeft->getGainsForRange(i, &config_buffer[index], &config_buffer[index+1], &config_buffer[index+2] );
//            index += 3;
//        }
//
//        // SpeedControllerRight
//        for( int i=0; i<NB_PI_SUBSET; i++)
//        {
//            speedControllerRight->getGainsForRange(i, &config_buffer[index], &config_buffer[index+1], &config_buffer[index+2]);
//            index += 3;
//        }
//
//        //Regulators
//        config_buffer[index++] = distanceRegulator->getGain();
//        config_buffer[index++] = angleRegulator->getGain();
//
//        // accel limiter
//        config_buffer[index++] = angleAccelerationlimiter->getMaxAcceleration();
//        config_buffer[index++] = distanceAccelerationLimiter->getMaxAcceleration();
//        config_buffer[index++] = distanceAccelerationLimiter->getMinAcceleration();
//        config_buffer[index++] = distanceAccelerationLimiter->getHighSpeedThreshold();
//
//        chprintf(outputStream, "sending %d float of config !\r\n", index);
//        USBStream::instance()->sendConfig((uint8_t*)config_buffer, index*sizeof(config_buffer[0]));
//    }
//    else
//    {
//        printUsage();
//    }
//}
//
//
//THD_FUNCTION(ControlPanelThread, p)
//{
//    (void) p;
//    void *ptr = nullptr;
//    uint32_t size = 0;
//    char *firstArg = nullptr;
//    char *argv[7];
//    while (!chThdShouldTerminateX())
//    {
//        USBStream::instance()->getFullBuffer(&ptr, &size);
//        if (size > 0)
//        {
//            char *buffer = (char*) ptr;
//
//            /*
//             *  On transforme la commande recu dans une version argv/argc
//             *    de manière a utiliser les commandes shell déjà définie...
//             */
//            bool prevWasSpace = false;
//            firstArg = buffer;
//            int nb_arg = 0;
//            for (uint32_t i = 0; i < size; i++)
//            {
//                if (prevWasSpace && buffer[i] != ' ')
//                {
//                    argv[nb_arg++] = &buffer[i];
//                }
//
//                if (buffer[i] == ' ' || buffer[i] == '\r' || buffer[i] == '\n')
//                {
//                    prevWasSpace = true;
//                    buffer[i] = '\0';
//                }
//                else
//                {
//                    prevWasSpace = false;
//                }
//            }
//
//            // On évite de faire appel au shell si le nombre d'arg est mauvais ou si la 1ière commande est mauvaise...
//            if (nb_arg > 0 && !strcmp(firstArg, "asserv"))
//            {
//                asservCommandUSB(nullptr, nb_arg, argv);
//            }
//            USBStream::instance()->releaseBuffer();
//        }
//    }
//}
//
//
///*
//#define ASSERV_THREAD_FREQUENCY (500)
//#define ASSERV_THREAD_PERIOD_S (1.0/ASSERV_THREAD_FREQUENCY)
//#define ASSERV_POSITION_DIVISOR (5)
//
//#define ENCODERS_WHEELS_RADIUS (47.2/2.0)
//#define ENCODERS_WHEELS_DISTANCE_MM (297)
//#define ENCODERS_TICKS_BY_TURN (1024*4)
//
//#define MAX_SPEED (500)
//
//#define DIST_REGULATOR_KP (9)
//#define DIST_REGULATOR_MAX_DELTA (8/ASSERV_THREAD_PERIOD_S)
//#define ANGLE_REGULATOR_KP (1400)
//#define ANGLE_REGULATOR_MAX_DELTA (8/ASSERV_THREAD_PERIOD_S)
//
//float speed_controller_right_Kp[NB_PI_SUBSET] = { 0.25, 0.25, 0.25 };
//float speed_controller_right_Ki[NB_PI_SUBSET] = { 0.45, 0.45, 0.45 };
//float speed_controller_right_speed_set[NB_PI_SUBSET] = { 500.0, 500.0, 500.0 };
//
//float speed_controller_left_Kp[NB_PI_SUBSET] = { 0.25, 0.25, 0.25 };
//float speed_controller_left_Ki[NB_PI_SUBSET] = { 0.45, 0.45, 0.45 };
//float speed_controller_left_speed_set[NB_PI_SUBSET] = { 500.0, 500.0, 500.0 };
//
//#define PLL_BANDWIDTH (250)
//
//#define COMMAND_MANAGER_ARRIVAL_ANGLE_THRESHOLD_RAD (0.1)
//#define COMMAND_MANAGER_ARRIVAL_DISTANCE_THRESHOLD_mm (1)
//#define COMMAND_MANAGER_GOTO_ANGLE_THRESHOLD_RAD (M_PI/8)
//#define COMMAND_MANAGER_GOTO_CHAIN_NEXT_CMD_DIST_mm (50)
//
//QuadratureEncoder encoders_int(false, true, false);
//
//MagEncoders encoders(false, false, true);
//
//Md22::I2cPinInit PMXCardPinConf_SCL_SDA = { GPIOB, 8, GPIOB, 9 };
////Md22::I2cPinInit PMXCardPinConf_SCL_SDA = {GPIOB, 6, GPIOB, 7};
//Md22 md22MotorController(false, true, true, &PMXCardPinConf_SCL_SDA, 400000);
//
//Regulator angleRegulator(ANGLE_REGULATOR_KP, MAX_SPEED);
//Regulator distanceRegulator(DIST_REGULATOR_KP, MAX_SPEED);
//
//Odometry odometry(ENCODERS_WHEELS_DISTANCE_MM, 0, 0);
//
//SpeedController speedControllerRight(speed_controller_right_Kp, speed_controller_right_Ki,
//        speed_controller_right_speed_set, 100, MAX_SPEED, ASSERV_THREAD_FREQUENCY);
//SpeedController speedControllerLeft(speed_controller_left_Kp, speed_controller_left_Ki, speed_controller_left_speed_set,
//        100, MAX_SPEED, ASSERV_THREAD_FREQUENCY);
//
//Pll rightPll(PLL_BANDWIDTH);
//Pll leftPll(PLL_BANDWIDTH);
//
//SlopeFilter angleSlopeFilter(ANGLE_REGULATOR_MAX_DELTA);
//SlopeFilter distSlopeFilter(DIST_REGULATOR_MAX_DELTA);
//
//CommandManager commandManager(COMMAND_MANAGER_ARRIVAL_ANGLE_THRESHOLD_RAD,
//COMMAND_MANAGER_ARRIVAL_DISTANCE_THRESHOLD_mm,
//COMMAND_MANAGER_GOTO_ANGLE_THRESHOLD_RAD, COMMAND_MANAGER_GOTO_CHAIN_NEXT_CMD_DIST_mm, angleRegulator,
//        distanceRegulator);
//
//AsservMain mainAsserv(ASSERV_THREAD_FREQUENCY, ASSERV_POSITION_DIVISOR,
//ENCODERS_WHEELS_RADIUS, ENCODERS_WHEELS_DISTANCE_MM, ENCODERS_TICKS_BY_TURN, commandManager, md22MotorController,
//        encoders_int, odometry, angleRegulator, distanceRegulator, angleSlopeFilter, distSlopeFilter,
//        speedControllerRight, speedControllerLeft, rightPll, leftPll);
//
//BaseSequentialStream *outputStream;
//
//static THD_WORKING_AREA(waAsservThread, 512);
//static THD_FUNCTION(AsservThread, arg)
//{
//    (void) arg;
//    chRegSetThreadName("AsservThread");
//    encoders.init();
//    encoders.start();
//    encoders_int.init();
//    encoders_int.start();
//    md22MotorController.init();
//    USBStream::init();
//
////    while (1) //desactivation du mainloop afin de tester MD22 en direct
////    {
////        chThdSleep(chTimeMS2I(200));
////    }
//
//    mainAsserv.mainLoop();
//}
//
//THD_WORKING_AREA(wa_shell, 512);
//THD_WORKING_AREA(wa_controlPanel, 512);
//THD_FUNCTION(ControlPanelThread, p);
//binary_semaphore_t controlPanelSemaphore;
//
//char history_buffer[SHELL_MAX_HIST_BUFF];
//char *completion_buffer[SHELL_MAX_COMPLETIONS];
//
//void asservCommand(BaseSequentialStream *chp, int argc, char **argv);
//
//int main(void)
//{
//
//    halInit();
//    chSysInit();
//
//    sdStart(&SD2, NULL);
//    shellInit();
//
//    outputStream = reinterpret_cast<BaseSequentialStream*>(&SD2);
//
//    // Custom commands
//    const ShellCommand shellCommands[] = { { "asserv", &(asservCommand) }, { nullptr, nullptr } };
//    ShellConfig shellCfg = { outputStream, shellCommands,
//#if (SHELL_USE_HISTORY == TRUE)
//            history_buffer,
//            sizeof(history_buffer),
//#endif
//#if (SHELL_USE_COMPLETION == TRUE)
//            completion_buffer
//#endif
//        };
//
//    thread_t *shellThd = chThdCreateStatic(wa_shell, sizeof(wa_shell), LOWPRIO, shellThread, &shellCfg);
//    chRegSetThreadNameX(shellThd, "shell");
//
//    chThdCreateStatic(waAsservThread, sizeof(waAsservThread), HIGHPRIO, AsservThread, NULL);
//
//    chBSemObjectInit(&controlPanelSemaphore, true);
//
//    chThdSleep(chTimeMS2I(200)); // Histoire d'être sur que l'usb soit lancé une fois que ce thread tourne...
//    thread_t *controlPanelThd = chThdCreateStatic(wa_controlPanel, sizeof(wa_controlPanel), LOWPRIO, ControlPanelThread,
//            nullptr);
//    chRegSetThreadNameX(controlPanelThd, "controlPanel");
//
//    deactivateHeapAllocation();
//
//    chThdSetPriority (LOWPRIO);
//    while (true) {
//        palClearPad(GPIOA, GPIOA_LED_GREEN);
//        chThdSleepMilliseconds(250);
//        palSetPad(GPIOA, GPIOA_LED_GREEN);
//        chThdSleepMilliseconds(250);
//    }
//}
//
//void asservCommand(BaseSequentialStream *chp, int argc, char **argv)
//{
//    auto printUsage = []() {
//        chprintf(outputStream,"Usage :\r\n");
//        chprintf(outputStream," - asserv enablemotor 0|1\r\n");
//        chprintf(outputStream," - asserv enablepolar 0|1\r\n");
//        chprintf(outputStream," - asserv startPanel \r\n");
//        chprintf(outputStream," -------------- \r\n");
//        chprintf(outputStream," - asserv wheelspeedstep [r|l] [speed] [step time] \r\n");
//        chprintf(outputStream," -------------- \r\n");
//        chprintf(outputStream," - asserv robotfwspeedstep [speed] [step time] \r\n");
//        chprintf(outputStream," - asserv robotangspeedstep [speed] [step time] \r\n");
//        chprintf(outputStream," - asserv speedcontrol [r|l] [Kp] [Ki] \r\n");
//        chprintf(outputStream," - asserv angleSlope delta_speed \r\n");
//        chprintf(outputStream," - asserv distSlope delta_speed \r\n");
//        chprintf(outputStream," ------------------- \r\n");
//        chprintf(outputStream," - asserv addangle angle_rad \r\n");
//        chprintf(outputStream," - asserv anglereset\r\n");
//        chprintf(outputStream," - asserv anglecontrol Kp\r\n");
//        chprintf(outputStream," ------------------- \r\n");
//        chprintf(outputStream," - asserv adddist mm \r\n");
//        chprintf(outputStream," - asserv distreset\r\n");
//        chprintf(outputStream," - asserv distcontrol Kp\r\n");
//        chprintf(outputStream," -------------- \r\n");
//        chprintf(outputStream," - asserv addgoto X Y\r\n");
//        chprintf(outputStream," - asserv gototest\r\n");
//        chprintf(outputStream," -------------- \r\n");
//        chprintf(outputStream," - asserv md22speedlr [-100=>100] [-100=>100]\r\n");
//        chprintf(outputStream," - asserv magvalues 0|1\r\n");
//        chprintf(outputStream," - asserv intencodersval 0|1\r\n");
//        chprintf(outputStream," - asserv extencodersval 0|1\r\n");
//        chprintf(outputStream," - asserv test\r\n");
//
//    };
//    (void) chp;
//
//    if (argc == 0) {
//        printUsage();
//        return;
//    }
//
//    if (!strcmp(argv[0], "wheelspeedstep")) {
//        char side = *argv[1];
//        float speedGoal = atof(argv[2]);
//        int time = atoi(argv[3]);
//        chprintf(outputStream, "setting fw robot speed %.2f rad/s for %d ms\r\n", speedGoal, time);
//
//        chprintf(outputStream, "setting wheel %s to speed %.2f rad/s for %d ms \r\n", (side == 'r') ? "right" : "left",
//                speedGoal, time);
//
//        float speedRight = speedGoal;
//        float speedLeft = 0;
//        if (side == 'l') {
//            speedLeft = speedGoal;
//            speedRight = 0;
//        }
//
//        mainAsserv.setWheelsSpeed(speedRight, speedLeft);
//        chThdSleepMilliseconds(time);
//        mainAsserv.setWheelsSpeed(0, 0);
//    } else if (!strcmp(argv[0], "robotfwspeedstep")) {
//        float speedGoal = atof(argv[1]);
//        int time = atoi(argv[2]);
//        chprintf(outputStream, "setting fw robot speed %.2f rad/s for %d ms\r\n", speedGoal, time);
//
//        mainAsserv.setRegulatorsSpeed(speedGoal, 0);
//        chThdSleepMilliseconds(time);
//        mainAsserv.setRegulatorsSpeed(0, 0);
//    } else if (!strcmp(argv[0], "robotangspeedstep")) {
//        float speedGoal = atof(argv[1]);
//        int time = atoi(argv[2]);
//        chprintf(outputStream, "setting angle robot speed %.2f rad/s for %d ms\r\n", speedGoal, time);
//
//        mainAsserv.setRegulatorsSpeed(0, speedGoal);
//        chThdSleepMilliseconds(time);
//        mainAsserv.setRegulatorsSpeed(0, 0);
//    } else if (!strcmp(argv[0], "speedcontrol")) {
//        char side = *argv[1];
//        float Kp = atof(argv[2]);
//        float Ki = atof(argv[3]);
//
//        chprintf(outputStream, "setting speed control Kp:%.2f Ki:%.2f to side %c \r\n", Kp, Ki, side);
//
//        if (side == 'r')
//            speedControllerRight.setGains(Kp, Ki);
//        else if (side == 'l')
//            speedControllerLeft.setGains(Kp, Ki);
//    } else if (!strcmp(argv[0], "angleSlope")) {
//        float slope = atof(argv[1]);
//        chprintf(outputStream, "setting angle slope delta %.2f \r\n", slope);
//
//        angleSlopeFilter.setSlope(slope);
//    } else if (!strcmp(argv[0], "distSlope")) {
//        float slope = atof(argv[1]);
//        chprintf(outputStream, "setting distance slope delta %.2f \r\n", slope);
//
//        distSlopeFilter.setSlope(slope);
//    } else if (!strcmp(argv[0], "addangle")) {
//        float angle = atof(argv[1]);
//        chprintf(outputStream, "Adding angle %.2frad \r\n", angle);
//
//        mainAsserv.resetToNormalMode();
//        commandManager.addTurn(angle);
//    } else if (!strcmp(argv[0], "anglereset")) {
//        chprintf(outputStream, "Reseting angle accumulator \r\n");
//        angleRegulator.reset();
//    } else if (!strcmp(argv[0], "distreset")) {
//        chprintf(outputStream, "Reseting distance accumulator \r\n");
//        distanceRegulator.reset();
//    } else if (!strcmp(argv[0], "adddist")) {
//        float dist = atof(argv[1]);
//        chprintf(outputStream, "Adding distance %.2fmm \r\n", dist);
//
//        mainAsserv.resetToNormalMode();
//        commandManager.addStraightLine(dist);
//    } else if (!strcmp(argv[0], "anglecontrol")) {
//        float Kp = atof(argv[1]);
//        chprintf(outputStream, "setting angle Kp to %.2f \r\n", Kp);
//
//        angleRegulator.setGain(Kp);
//    } else if (!strcmp(argv[0], "distcontrol")) {
//        float Kp = atof(argv[1]);
//        chprintf(outputStream, "setting dist Kp to %.2f \r\n", Kp);
//
//        distanceRegulator.setGain(Kp);
//    } else if (!strcmp(argv[0], "enablemotor")) {
//        bool enable = !(atoi(argv[1]) == 0);
//        chprintf(outputStream, "%s motor output\r\n", (enable ? "enabling" : "disabling"));
//
//        mainAsserv.enableMotors(enable);
//    } else if (!strcmp(argv[0], "startPanel")) {
//        chBSemSignal(&controlPanelSemaphore);
//    } else if (!strcmp(argv[0], "enablepolar")) {
//        bool enable = !(atoi(argv[1]) == 0);
//        chprintf(outputStream, "%s polar control\r\n", (enable ? "enabling" : "disabling"));
//
//        mainAsserv.enablePolar(enable);
//    } else if (!strcmp(argv[0], "addgoto")) {
//        float X = atof(argv[1]);
//        float Y = atof(argv[2]);
//        chprintf(outputStream, "Adding goto(%.2f,%.2f) consign\r\n", X, Y);
//
//        mainAsserv.resetToNormalMode();
//        commandManager.addGoTo(X, Y);
//    } else if (!strcmp(argv[0], "gototest")) {
////        commandManager.addGoToEnchainement(450, -200);
////        commandManager.addGoToEnchainement(450, -600);
////        commandManager.addGoToEnchainement(300, -400);
////        commandManager.addGoTo(150,0);
//
//        mainAsserv.resetToNormalMode();
////        commandManager.addGoToEnchainement(365, -270);
////        commandManager.addGoToEnchainement(550, -385);
////        commandManager.addGoToEnchainement(490, -590);
////        commandManager.addGoToEnchainement(295, -720);
////        commandManager.addGoToEnchainement(180, -1000);
////        commandManager.addGoToEnchainement(390, -1100);
////        commandManager.addGoToEnchainement(550, -900);
////        commandManager.addGoToEnchainement(395, -630);
//        commandManager.addGoToNoStop(300, -440);
//        commandManager.addGoTo(300, -250);
////        commandManager.addGoToAngle(1000, -250);
////        commandManager.addStraightLine(-200);
//
//    } else if (!strcmp(argv[0], "md22speedlr")) {
//        int speedGoalL = atoi(argv[1]);
//        int speedGoalR = atoi(argv[2]);
//        mainAsserv.enableMotors(false);
//        md22MotorController.setMotorLeftSpeed(speedGoalL);
//        md22MotorController.setMotorRightSpeed(speedGoalR);
//        chprintf(outputStream, "Motors at %d %d\r\n", speedGoalL, speedGoalR);
//
//    } else if (!strcmp(argv[0], "intencodersval")) {
//        bool enable = !(atoi(argv[1]) == 0);
//        int16_t encoderDeltaRight_tmp = 0;
//        int16_t encoderDeltaLeft_tmp = 0;
//        int32_t encoderLSum = 0;
//        int32_t encoderRSum = 0;
//        do {
//            encoders_int.getValues(&encoderDeltaRight_tmp, &encoderDeltaLeft_tmp);
//            encoders_int.getEncodersTotalCount(&encoderRSum, &encoderLSum);
//            chprintf(outputStream, "INT TOTL %d \t%d\t;\tTOTR %d \t%d\r\n", encoderLSum, encoderDeltaLeft_tmp, encoderRSum,
//                    encoderDeltaRight_tmp);
//        } while (enable);
//    } else if (!strcmp(argv[0], "extencodersval")) {
//        bool enable = !(atoi(argv[1]) == 0);
//        mainAsserv.enableMotors(false);
//        int16_t encoderDeltaRight_tmp = 0;
//        int16_t encoderDeltaLeft_tmp = 0;
//        int32_t encoderLSum = 0;
//        int32_t encoderRSum = 0;
//        //encoders.start();
//        do {
//            encoders.getValues(&encoderDeltaRight_tmp, &encoderDeltaLeft_tmp);
//            encoders.getEncodersTotalCount(&encoderRSum, &encoderLSum);
//            chprintf(outputStream, "EXT TOTL %d \t%d \t;\tTOTR %d \t%d \r\n", encoderLSum, encoderDeltaLeft_tmp,
//                    encoderRSum, encoderDeltaRight_tmp);
//            chThdSleepMilliseconds(2);
//        } while (enable);
//
//    } else if (!strcmp(argv[0], "magvalues")) {
//        bool enable = !(atoi(argv[1]) == 0);
//
//        uint8_t agcR = 0;
//        uint8_t agcL = 0;
//        uint8_t diagR = 0;
//        uint8_t diagL = 0;
//        uint16_t magR = 0;
//        uint16_t magL = 0;
//        uint16_t rawR = 0;
//        uint16_t rawL = 0;
//
//        do {
//            encoders.getValuesStatus(&rawR, &rawL, &agcR, &agcL, &diagR, &diagL, &magR, &magL);
//
//            chprintf(outputStream,
//                    "MAG rawL=%d\t AGC=%d\t DIAG=%d\t MAG=%d\t  ;  rawR=%d\t AGC=%d\t DIAG=%d\t MAG=%d\t\r\n", rawL,
//                    agcL, diagL, magL, rawR, agcR, diagR, magR);
//            chThdSleepMilliseconds(200);
//        } while (enable);
//    } else if (!strcmp(argv[0], "test")) {
//
//        USBStream::instance()->setSpeedEstimatedRight(1);
//        USBStream::instance()->setSpeedEstimatedLeft(2);
//        USBStream::instance()->setSpeedGoalRight(3);
//        USBStream::instance()->setSpeedGoalLeft(4);
//        USBStream::instance()->setSpeedOutputRight(5);
//        USBStream::instance()->setSpeedOutputLeft(6);
//        USBStream::instance()->setSpeedIntegratedOutputRight(7);
//        USBStream::instance()->setSpeedIntegratedOutputLeft(8);
//        USBStream::instance()->setAngleGoal(9);
//        USBStream::instance()->setAngleAccumulator(0);
//        USBStream::instance()->setAngleOutput(1);
//        USBStream::instance()->setAngleOutputLimited(2);
//
//        USBStream::instance()->setDistGoal(3);
//        USBStream::instance()->setDistAccumulator(4);
//        USBStream::instance()->setDistOutput(5);
//        USBStream::instance()->setDistOutputLimited(6);
//        USBStream::instance()->setOdoX(10.0);
//        USBStream::instance()->setOdoY(100.0);
//        USBStream::instance()->setOdoTheta(9);
//
//        USBStream::instance()->setRawEncoderDeltaLeft(100.0);
//        USBStream::instance()->setRawEncoderDeltaRight(200.0);
//
//        USBStream::instance()->sendCurrentStream();
//    } else {
//        printUsage();
//    }
//}
//
//THD_FUNCTION(ControlPanelThread, p)
//{
//    (void) p;
//    void *ptr = nullptr;
//    uint32_t size = 0;
//    char *firstArg = nullptr;
//    char *argv[7];
//    chBSemWait(&controlPanelSemaphore);
//    while (!chThdShouldTerminateX()) {
//        USBStream::instance()->getFullBuffer(&ptr, &size);
//        if (size > 0) {
//            char *buffer = (char*) ptr;
//
//            //On transforme la commande recu dans une version argv/argc
//            //de manière a utiliser les commandes shell déjà définie...
//
//            bool prevWasSpace = false;
//            firstArg = buffer;
//            int nb_arg = 0;
//            for (uint32_t i = 0; i < size; i++) {
//                if (prevWasSpace && buffer[i] != ' ') {
//                    argv[nb_arg++] = &buffer[i];
//                }
//
//                if (buffer[i] == ' ' || buffer[i] == '\r' || buffer[i] == '\n') {
//                    prevWasSpace = true;
//                    buffer[i] = '\0';
//                } else {
//                    prevWasSpace = false;
//                }
//            }
//
//            // On évite de faire appel au shell si le nombre d'arg est mauvais ou si la 1ière commande est mauvaise...
//            if (nb_arg > 0 && !strcmp(firstArg, "asserv")) {
//                asservCommand(nullptr, nb_arg, argv);
//            }
//            USBStream::instance()->releaseBuffer();
//        }
//    }
//}
//*/
