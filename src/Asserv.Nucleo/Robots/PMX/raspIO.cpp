//#include "ch.h"
//#include "hal.h"
//#include "shell.h"
//#include <chprintf.h>
//#include <cstdlib>
//#include <cstring>
//#include <cstdio>
//#include <cfloat>
//
//
//
///*
// * All this part is absolutely terrible !
// * A (way!) best design must be found in order to share code between robot's instance !
// */
//#include "Odometry.h"
//#include "AsservMain.h"
//#include "commandManager/CommandManager.h"
//#include "motorController/Md22.h"
//#include "util/asservMath.h"
//
//extern Odometry *odometry;
//extern AsservMain *mainAsserv;
//extern Md22 *md22MotorController;
//extern CommandManager *commandManager;
//
//extern BaseSequentialStream *outputStream;
//extern BaseSequentialStream *outputStreamSd4;
//
//static void serialReadLine(char *buffer, unsigned int buffer_size)
//{
//    unsigned int i;
//    for(i=0; i<buffer_size; i++)
//    {
//        buffer[i] = streamGet(&SD4);
//        if ( buffer[i] == '\n' || buffer[i] == '\r')
//            break;
//    }
//    buffer[i] = '\0';
//}
//
//THD_FUNCTION(asservCommandSerial, p)
//{
//    (void) p;
//    /*
//     * Commande / Caractères à envoyer sur la série / Paramètres / Effets obtenus
//
//     g%x#%y\n / Goto / x, y : entiers, en mm /Le robot se déplace au point de coordonnée (x, y). Il tourne vers le point, puis avance en ligne droite. L'angle est sans cesse corrigé pour bien viser le point voulu.
//     e%x#%y\n / goto Enchaîné / x, y : entiers, en mm / Idem que le Goto, sauf que lorsque le robot est proche du point d'arrivée (x, y), on s'autorise à enchaîner directement la consigne suivante si c'est un Goto ou un Goto enchaîné, sans marquer d'arrêt.
//     v%d\n / aVancer / d : entier, en mm / Fait avancer le robot de d mm, tout droit
//     t%a\n / Tourner / a : entier, en degrées / Fait tourner le robot de a degrées. Le robot tournera dans le sens trigonométrique : si a est positif, il tourne à gauche, et vice-versa.
//     f%x#%y\n / faire Face / x, y : entiers, en mm / Fait tourner le robot pour être en face du point de coordonnées (x, y). En gros, ça réalise la première partie d'un Goto : on se tourne vers le point cible, mais on avance pas.
//     h / Halte ! / Arrêt d'urgence ! Le robot est ensuite systématiquement asservi à sa position actuelle. Cela devrait suffire à arrêter le robot correctement. La seule commande acceptée par la suite sera un Reset de l'arrêt d'urgence : toute autre commande sera ignorée.
//     r / Reset de l'arrêt d'urgence / Remet le robot dans son fonctionnement normal après un arrêt d'urgence. Les commandes en cours au moment de l'arrêt d'urgence NE sont PAS reprises. Si le robot n'est pas en arrêt d'urgence, cette commande n'a aucun effet.
//
//     p / get Position / Récupère la position et le cap du robot, sous la forme de 3 types float (3 * 4 bytes), avec x, y, et a les coordonnées et l'angle du robot.
//     S%x#%y#%a\n / set Position / applique la nouvelle position du robot
//
//     z / avance de 20 cm
//     s / recule de 20 cm
//     q / tourne de 45° (gauche)
//     d / tourne de -45° (droite)
//
//     M / modifie la valeur d'un paramètre / name, value
//     R / réinitialiser l'asserv
//     D / dump la config du robot
//     G / lire la valeur d'un paramètre / name
//     L / recharge la config config.txt
//     W / sauvegarde la config courante  config~1.txt = config.default.txt
//
//     I / Active les actions dans la boucle d'asservissement (odo + managers)
//     ! / Stoppe actions dans la boucle d'asservissement
//     K / desactive le consignController et le commandManager
//     J / reactive le consignController et le commandManager
//
//     + / applique une valeur +1 sur les moteurs LEFT
//     - / applique une valeur -1 sur les moteurs LEFT
//     */
//
//    float consigneValue1 = 0;
//    float consigneValue2 = 0;
//    float consigneValue3 = 0;
//    char buffer[64];
//    //chprintf(outputStream, "Started\r\n");
//    chprintf(outputStreamSd4, "Started\r\n");
//
//
//    while(true)
//    {
//        char readChar = streamGet(&SD4);
//
//        switch (readChar) {
//
//        case 'h': //Arrêt d'urgence
//            mainAsserv->setEmergencyStop();
//            serialReadLine(buffer, sizeof(buffer));
//            chprintf(outputStreamSd4, "Arrêt d'urgence ! \r\n");
//            break;
//
//        case 'r': //Reset de l'arrêt d'urgence
//            mainAsserv->resetEmergencyStop();
//            serialReadLine(buffer, sizeof(buffer));
//            break;
//
//        case 'z':
//            // Go 20cm
//            chprintf(outputStreamSd4, "consigne avant : 200mm\n");
//            commandManager->addStraightLine(200);
//            break;
//
//        case 's':
//            chprintf(outputStreamSd4, "consigne arrière : 200mm\n");
//            commandManager->addStraightLine(-200);
//            break;
//
//        case 'q':
//            chprintf(outputStreamSd4, "consigne gauche : 45°\n");
//            commandManager->addTurn(degToRad(45));
//            break;
//
//        case 'd':
//            chprintf(outputStreamSd4, "consigne gauche : 45°\n");
//             commandManager->addTurn(degToRad(-45));
//             break;
//
//        case 'v': //aVance d'un certain nombre de mm
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f", &consigneValue1);
//            commandManager->addStraightLine(consigneValue1);
//            break;
//
//        case 't': //Tourne d'un certain angle en degrés
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f", &consigneValue1);
//            commandManager->addTurn(degToRad(consigneValue1));
//            break;
//
//        case 'f': //faire Face à un point précis, mais ne pas y aller, juste se tourner
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f#%f", &consigneValue1, &consigneValue2);
//            commandManager->addGoToAngle(consigneValue1, consigneValue2);
//            break;
//
//        case 'g': //Go : va à un point précis
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f#%f", &consigneValue1, &consigneValue2);
//            commandManager->addGoTo(consigneValue1, consigneValue2);
//            break;
//
//        case 'b': //Go : va à un point précis
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f#%f", &consigneValue1, &consigneValue2);
//            commandManager->addGoToBack(consigneValue1, consigneValue2);
//            break;
//
//        case 'e': // goto, mais on s'autorise à Enchainer la consigne suivante sans s'arrêter
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f#%f", &consigneValue1, &consigneValue2);
//            commandManager->addGoToNoStop(consigneValue1, consigneValue2);
//            break;
//
//        case 'p': //retourne la Position et l'angle courants du robot
//            chprintf(outputStreamSd4, "x%fy%fa%fs%d\r\n",
//                    odometry->getX(), odometry->getY(), odometry->getTheta(),
//                    commandManager->getCommandStatus());
//            chprintf(outputStream, "x%fy%fa%fs%d\r\n",
//                                odometry->getX(), odometry->getY(), odometry->getTheta(),
//                                commandManager->getCommandStatus());
//            break;
//
//        case 'P': // set la position et l'angle du robot
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f#%f#%f", &consigneValue1, &consigneValue2, &consigneValue3);
//            mainAsserv->setPosition(consigneValue1, consigneValue2, consigneValue3);
//            break;
//
//        case 'M': // M0 = coupe les moteurs / M1 = remet les moteurs
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f", &consigneValue1);
//            mainAsserv->enableMotors(consigneValue1 == 1);
//            break;
//
//        case 'S': // Vitesse maximum en %
//            serialReadLine(buffer, sizeof(buffer));
//            sscanf(buffer, "%f", &consigneValue1);
//            mainAsserv->limitMotorControllerConsignToPercentage(consigneValue1);
//            break;
//
//        case 'I':
//            break;
//
//        case '!':
//            break;
//
//        case 'R':
//            mainAsserv->reset();
//            break;
//
//        default:
//            chprintf(outputStreamSd4, " - unexpected character\r\n");
//            break;
//        }
//    }
//}
//
//THD_FUNCTION(asservPositionSerial, p)
//{
//    (void) p;
//    const time_conv_t loopPeriod_ms = 100;
//    systime_t time = chVTGetSystemTime();
//    time += TIME_MS2I(loopPeriod_ms);
//    unsigned int debg = 0;
//    while(true)
//    {
//        chprintf(outputStreamSd4, "#%d;%d;%f;%d;%d;%d;%d;%d\r\n",
//            (int32_t)odometry->getX(), (int32_t)odometry->getY(), odometry->getTheta(),
//            commandManager->getCommandStatus(), commandManager->getPendingCmdCount(),
//            md22MotorController->getLeftSpeed(), md22MotorController->getRightSpeed(), debg);
////        chprintf(outputStream,    "#%d;%d;%f;%d;%d;%d;%d;%d\r\n",
////                    (int32_t)odometry->getX(), (int32_t)odometry->getY(), odometry->getTheta(),
////                    commandManager->getCommandStatus(), commandManager->getPendingCmdCount(),
////                    md22MotorController->getLeftSpeed(), md22MotorController->getRightSpeed(), debg);
//
//        chThdSleepUntil(time);
//        time += TIME_MS2I(loopPeriod_ms);
//        debg++;
//    }
//}
