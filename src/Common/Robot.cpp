/*
 * Robot.cpp
 *
 *
 */

#include "Robot.hpp"

#include <stdio.h>
#include <stdlib.h>

#include "../Log/SvgWriter.hpp"
#include "Asserv/MotorControl.hpp"
#include "Asserv/MovingBase.hpp"
#include "Utils/ConsoleKeyInput.hpp"

#ifdef SIMU
#include <sys/ipc.h>
#include <sys/msg.h>
#endif
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "../Log/Logger.hpp"
#include "Action/Actions.hpp"
#include "Asserv/Asserv.hpp"

using namespace std;

Robot::Robot() :
        chrono_("Robot"), myColor_(PMXNOCOLOR), cArgs_("", "(c) PM-ROBOTIX 2022", "/") // use character "/" instead of "-" for arguments
{
    points = 0;
    tabletest = false;

    actions_default_ = NULL;
    asserv_default_ = NULL;

    empty_ = 0;
    useExternalEncoder_ = 0;
    skipSetup_ = 0;
    end90s_ = 0;

    configureDefaultConsoleArgs();

}

Robot::~Robot() {
    svgPrintEndOfFile();
    stopMotionTimerAndActionManager();
    //Stop le log s'il existe (core dump sinon)
    logs::LoggerFactory::instance().stopLog();
}

void Robot::svgPrintPosition(int color) {

    if (asserv_default_ != NULL) {
        this->svgw().writePosition_Bot(this->passerv()->pos_getX_mm(), this->passerv()->pos_getY_mm(),
                this->passerv()->pos_getTheta(), color);
    }
    else logger().error() << "asserv_default is NULL !" << logs::end;
}

void Robot::svgPrintEndOfFile() {
    //end SVG file
    svg_->endHeader();
}

void Robot::configureDefaultConsoleArgs() {
#ifdef SIMU
    cArgs_.addOption('z', "Simulate button in a separate linux console, please execute this separately");
#endif
    // Add option "-h" with explanation...
    cArgs_.addOption('h', "Display usage help");

    cArgs_.addOption('k', "skip setup");

    cArgs_.addOption('b', "color BLEU"); //(without PMXGREEN!!)

    cArgs_.addArgument("type", "Type of match (t)est/(m)atch/(p)ause", "m");
    {
        Arguments::Option cOpt('n', ""); //TODO delete the /n, the t is enough
        cOpt.addArgument("num", "number of the functional test");
        cArgs_.addOption(cOpt);
    }

    //deprecated, replaced by /v
//    {
//        Arguments::Option cOpt('c', "");
//        cOpt.addArgument("color", "color of robot [y]ellow/[v]iolet", "y");
//        cArgs_.addOption(cOpt);
//    }

    {
        Arguments::Option cOpt('s', "");
        cOpt.addArgument("strategy", "name of the strategy", "all");
        cArgs_.addOption(cOpt);
    }
}

void Robot::parseConsoleArgs(int argc, char** argv, bool stopWithErrors) {
    if (stopWithErrors) {
        // request option "h" and print out manual if set...
        if (cArgs_['h']) {
            std::cout << "Available functional tests: " << std::endl;
            cmanager_.displayAvailableTests("", -1);
            cArgs_.usage();
            exit(0);
        }
    }

    if (!cArgs_.parse(argc, argv, stopWithErrors)) {
        logger().debug() << "Error parsing DEFAULT" << logs::end;
        sleep(1);
        exit(-1);
    }
}



void Robot::begin(int argc, char** argv) {
    int num = -1;
    string select = "-";
    string color = "-";
    string strat = "-";

#ifdef SIMU //cas de la simulation sous linux
    //http://jean-luc.massat.perso.luminy.univ-amu.fr/ens/docs/IPC.html
    //only for SIMU to simulate a non blocking getch() in a separate window console with /z
    if (cArgs_['z']) {
        int res;
        int frequete;
        printf("Send button from keyboard : BACK ENTER UP DOWN LEFT RIGHT\n");
        frequete = msgget(CLEF_REQUETES, 0700 | IPC_CREAT);
        if (frequete == -1) {
            perror("msgget");
            sleep(1);
            exit(0);
        }
        while (1) {
            char cInput;
            cInput = ConsoleKeyInput::mygetch(); //wait a user action
            //printf("button= %d<\n", cInput);
            if (cInput == 27)            // if ch is the escape sequence with num code 27, k turns 1 to signal the next
                    {
                cInput = ConsoleKeyInput::mygetch();
                if (cInput == 91) // if the previous char was 27, and the current 91, k turns 2 for further use
                        {
                    cInput = ConsoleKeyInput::mygetch();
                }
            }

            printf("final button= %d \n", cInput);

            switch (cInput) {
                case 10:
                    strcpy(msg_ipc.mtext, "enter");
                    break;
                case 127:
                    strcpy(msg_ipc.mtext, "back");
                    break;
                case 65:
                    strcpy(msg_ipc.mtext, "up");
                    break;
                case 66:
                    strcpy(msg_ipc.mtext, "down");
                    break;
                case 67:
                    strcpy(msg_ipc.mtext, "right");
                    break;
                case 68:
                    strcpy(msg_ipc.mtext, "left");
                    break;

                default:

                    break;
                    std::this_thread::sleep_for(std::chrono::microseconds(1000));

            }

            msg_ipc.mtype = getpid();

            res = msgsnd(frequete, &msg_ipc, strlen(msg_ipc.mtext) + 1, 0);
            if (res == -1) {
                perror("msgsnd");
                exit(0);
            }
            std::this_thread::yield();
        }
    }
#endif

    if (cArgs_["type"] == "p" || cArgs_["type"] == "P") {
        //display all functional tests
        cmanager_.displayAvailableTests("", -1);

        if (!cArgs_['k']) {
            //------------- Pour debug
            //pause s'il n'y a pas tous les elements pour visualiser le log d'erreur
            char cInput;
            Robot::logger().info() << "Press Enter key to continue ..." << logs::end;

            do {
                cInput = ConsoleKeyInput::mygetch();
                switch (cInput) {

                    case 10:
                        //printf("Enter key!\n");
                        break;
                    case 127:
                        //printf("Back key!\n");
                        cout << "Exit !\n" << endl;
                        //cout << default_console << endl;
                        sleep(1);
                        exit(0);
                        break;
                }
                utils::sleep_for_micros(1000);
                std::this_thread::yield();
            } while (cInput != 10);
            //---------------fin Pour debug
        }
    }
    //logger().debug() << "" << logs::end;

    //logger().debug() << "type = " << cArgs_["type"] << logs::end;

    //logger().debug() << "Option c set " << (int) cArgs_['c'] << ", color = " << " " << cArgs_['c']["color"] << logs::end;

//    if (cArgs_['c']) {
//        color = cArgs_['c']["color"];
//        if (color == "violet" || color == "v") this->setMyColor(PMXBLUE);
//        else if (color == "yellow" || color == "jaune" || color == "j" || color == "y") this->setMyColor(PMXGREEN);
//        else {
//            this->setMyColor(PMXNOCOLOR);
//            logger().error() << "setMyColor(NOCOLOR)" << logs::end;
//            exit(-1);
//        }
//        logger().debug() << "setMyColor DONE : " << this->getMyColor() << logs::end;
//    }
//    else {
//        //defaut si aucune couleur n'est specifiée
//        this->setMyColor(PMXGREEN);
//    }

    if (cArgs_['b']) {
        logger().debug() << "b = " << (int) cArgs_['b'] << logs::end;
        this->setMyColor(PMXBLUE);
    }
    else this->setMyColor(PMXGREEN);//defaut si aucune couleur n'est specifiée
    logger().debug() << "setMyColor done; getMyColor() = " << getMyColor() << logs::end;




    if (cArgs_['s']) {
        strat = cArgs_['s']["strategy"];
        logger().info() << "strategy selected = " << strat << logs::end;
        this->strategy(strat);
    }
    else {
        this->strategy("all");
    }

    //test number
    if (cArgs_['n']) {
        num = atoi(cArgs_['n']["num"].c_str());
        logger().debug() << "Option n set " << (int) cArgs_['n'] << ", num = " << num << logs::end;
    }

    //skip state
    if (cArgs_['k']) {
        logger().debug() << "skip = " << (int) cArgs_['k'] << logs::end;
        this->skipSetup(true);
    }
    else this->skipSetup(false);

    if (cArgs_["type"] != "m" && cArgs_["type"] != "t" && cArgs_["type"] != "T" && cArgs_["type"] != "M") {
        select = cmanager_.displayMenuFirstArgu();
        if (select == "-") {
            logger().error() << "displayMenuFirstArgu bad return " << logs::end;
            sleep(1);
            exit(-1);
        }
        cArgs_["type"] = select;
    }

    //functional test cases
    if (cArgs_["type"] == "t" or cArgs_["type"] == "T") {
        if (num > 0) {
            //execute defined test
            cmanager_.run(num, argc, argv);
        }
        else {
            //ask param
            cmanager_.displayMenuFunctionalTestsAndRun(argc, argv);
        }
    }
}

void Robot::stopMotionTimerAndActionManager() {

    if (passerv() != NULL) {
        this->passerv()->stopMotionTimerAndOdo();
    }
    else logger().error() << "asserv_default is NULL ! " << logs::end;

    if (actions_default_ != NULL) {

        this->actions_default_->clearAll(); //clear actions and timers
        this->actions_default_->waitAndStopManagers();
    }
    else logger().error() << "actions_default is NULL ! " << logs::end;
}

void Robot::freeMotion() {
    this->passerv()->freeMotion();
    this->passerv()->stopMotors();
}

void Robot::resetDisplayTS() {
    logger().error() << "resetDisplayTS ! (To be surcharged) " << logs::end;
}

void Robot::displayTS(TRAJ_STATE ts) {
    logger().error() << "displayTS ! (To be surcharged) " << logs::end;
}
void Robot::resetDisplayObstacle() {
    logger().error() << "resetDisplayObstacle ! (To be surcharged) " << logs::end;
}

void Robot::displayObstacle(int level) {
    logger().error() << "displayObstacle ! (To be surcharged) " << logs::end;
}
