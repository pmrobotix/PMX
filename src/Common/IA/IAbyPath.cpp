/*
 * IAbyPath.cpp
 *
 *  Created on: 4 mai 2017
 *      Author: pmx
 */

#include "IAbyPath.hpp"

#include <bits/basic_string.h>
#include <bits/stl_iterator.h>
#include <bits/stl_vector.h>
#include <bits/stringfwd.h>
#include <simple_svg_1.0.0.hpp>
#include <src/pmr_edge.h>
#include <src/pmr_node.h>
#include <src/pmr_path_result.h>
#include <src/pmr_pathfinding.h>
#include <src/pmr_point.h>
#include <src/pmr_zone.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <iosfwd>
#include <ostream>

#include "../../Log/Logger.hpp"
#include "../../Log/SvgWriter.hpp"
#include "../Asserv/Asserv.hpp"

IAbyPath::IAbyPath(Robot *robot)
{
    ia_clear();
    robot_ = robot;
    p_ = NULL;
}

void IAbyPath::addPlayground(Playground *p)
{
    p_ = p;
}

void IAbyPath::enable(PlaygroundObjectID id, bool enable)
{
    p_->enable(id, enable);

    //TODO afficher une croix sur le SVG si disable
}

void IAbyPath::toSVG()
{
    if (p_ == NULL) {
        logger().error() << __FUNCTION__ << " (line " << __LINE__ << ") : Playground is NULL !! !" << logs::end;
    } else {
        PathFinder *pf = p_->get_path_finder();
        std::vector<Zone*>::iterator zones_it;
        std::vector<Edge*>::iterator edges_it;
        std::vector<Node*>::iterator nodes_it;
        unsigned int i;

        svg::Dimensions dimensions(pf->field_x2, pf->field_y2);
        svg::Layout lay(dimensions, svg::Layout::TopLeft);

        svg::Document doc("ia", lay);

        doc << svg::elemStart("g") << svg::attribute("transform", "translate(200,3200) scale(1,-1)")
                << svg::emptyElemEnd(false);

        // Red image border.
        svg::Polygon border(svg::Fill(svg::Color::White), svg::Stroke(5, svg::Color::Red));
        border << svg::Point(pf->field_x1, pf->field_y1) << svg::Point(dimensions.width, pf->field_y1)
                << svg::Point(dimensions.width, dimensions.height) << svg::Point(pf->field_x1, dimensions.height);
        doc << border;

        // Display all zones
        for (zones_it = pf->zones.begin(); zones_it < pf->zones.end(); zones_it++) {
            Zone *zone = *zones_it;
            if (zone->nodes_count > 0) {
                svg::Polygon zone_poly(svg::Fill(svg::Color::Aqua), svg::Stroke(0, svg::Color::Aqua));
                for (i = 0; i < zone->nodes_count; i++) {
                    Node *node = zone->nodes[i];
                    zone_poly << svg::Point(node->x, node->y);
                }
                doc << zone_poly;
            }
        }

        // Display all enabled edges
        for (edges_it = pf->edges.begin(); edges_it < pf->edges.end(); edges_it++) {
            Edge *edge = *edges_it;
            if (edge->enabled) {
                svg::Point p1(edge->node1->x, edge->node1->y);
                svg::Point p2(edge->node2->x, edge->node2->y);
                svg::Line line(p1, p2, svg::Stroke(0.5, svg::Color(238, 238, 238)));
                doc << line;
            }
        }
        /*
         // Display found path
         if (found_path != NULL)
         {
         bool node_found = false;
         svg::Polyline path_polyline(svg::Stroke(5, svg::Color::Green));

         for (nodes_it = found_path->path.begin(); nodes_it < found_path->path.end(); nodes_it++)
         {
         Node* node = *nodes_it;
         if (!node_found)
         {
         node_found = true;
         }
         path_polyline << svg::Point(node->x, node->y);
         }
         if (node_found)
         {
         doc << path_polyline;
         }
         }*/

        doc << svg::elemEnd("g");

        //TODO move to svgWriter !

        //robot_->svgw().write...

        loggerSvg().info() << doc.toString() << logs::end;
        //doc.save();
    }
}

void IAbyPath::ia_start()
{
    ia_checkZones();
    if (_actions_count <= 0) {

        //printf("%s (line %d) : Error : no actions defined\n", __FUNCTION__, __LINE__);
        logger().error() << __FUNCTION__ << " (line " << __LINE__ << ") : no actions defined !" << logs::end;
        sleep(1);
        exit(2);
    }
    bool allDone = false;
    while (!allDone) {
        allDone = true;
        int i = 0;
        for (i = 0; i < _actions_count; i++) {
            ACTIONS *z = _actions[i];
            if (z->completed == false) {
                //printf("\n== ia is executing actions [%d/%d] : %s\n", i + 1, _actions_count, z->name);
                //printf("state before actions : %s : (%f,%f) %f\n", z->name, cc_getX(), cc_getY(), cc_getThetaInDegree());
                //printf("encoders: %ld,%ld\n",robot_getLeftExternalCounter(),robot_getRightExternalCounter());
                bool done = (*z->action)();
                if (!done) {
                    allDone = false;
                }
                z->completed = done;
                if (!done) {
                    if (robot_ != NULL)
//                        printf("%s state after actions : %s : (%f,%f) %f FAILED\n", __FUNCTION__, z->name,
//                                robot_->passerv()->pos_getX_mm(), robot_->passerv()->pos_getY_mm(),
//                                robot_->passerv()->pos_getThetaInDegree());

                        logger().error() << __FUNCTION__ << " state after actions : " << z->name << " : ("
                                << robot_->passerv()->pos_getX_mm() << "," << robot_->passerv()->pos_getY_mm() << ", "
                                << robot_->passerv()->pos_getThetaInDegree() << ") FAILED" << logs::end;
                    else {
                        logger().error() << __FUNCTION__ << " (line " << __LINE__ << ") : robot_ is NULL !"
                                << logs::end;
                        sleep(1);
                        exit(-1);
                    }

                }
                if (robot_ != NULL)
//                    printf("%s state after actions : %s : (%f,%f) %f\n", __FUNCTION__, z->name,
//                            robot_->passerv()->pos_getX_mm(), robot_->passerv()->pos_getY_mm(),
//                            robot_->passerv()->pos_getThetaInDegree());
                    logger().info() << __FUNCTION__ << " state after actions : " << z->name << " : ("
                            << robot_->passerv()->pos_getX_mm() << ", " << robot_->passerv()->pos_getY_mm() << ", "
                            << robot_->passerv()->pos_getThetaInDegree() << ")" << logs::end;
                else {
                    logger().error() << __FUNCTION__ << " (line " << __LINE__ << ") : robot_ is NULL !" << logs::end;
                    sleep(1);
                    exit(-1);
                }
            }

        }
        std::this_thread::yield();
    }
}

void IAbyPath::ia_clear()
{
    _zones_count = 0;
    //_zones_path_count = 0;
    _actions_count = 0;
}
void IAbyPath::ia_addAction(const char *name, RobotAction action)
{
    ACTIONS *a = (ACTIONS*) calloc(1, sizeof(ACTIONS));
    strcpy(a->name, name);
    a->action = action;
    a->completed = false;
    _actions[_actions_count] = a;
    _actions_count++;
}
void IAbyPath::ia_createZone(const char *name, float minX, float minY, float width, float height, float startX,
        float startY, float startAngleDeg)
{
    ZONE *z = (ZONE*) calloc(1, sizeof(ZONE));

    z->minX = minX;
    z->minY = minY;
    z->width = width;
    z->height = height;
    z->startX = startX;
    z->startY = startY;
    z->startAngle = startAngleDeg;
    if (robot_ != NULL) {
        z->startX = robot_->passerv()->getRelativeXMin(z->startX);
        z->minX = robot_->passerv()->getRelativeXMin(z->minX, z->width);
        //z->startAngle = robot_->passerv()->getRelativeAngle(z->startAngle);
        z->startAngle = radToDeg(robot_->passerv()->getRelativeAngleRad(degToRad(z->startAngle)));
    } else {
        logger().error() << __FUNCTION__ << " (line " << __LINE__ << ") : robot_ is NULL !" << logs::end;
        sleep(1);
        exit(-1);
    }
    strcpy(z->name, name);
    _zones[_zones_count] = z;
    _zones_count++;

    robot_->svgw().writeZone(z->name, z->minX, z->minY, z->width, z->height, z->startX, z->startY,
            z->startAngle * M_PI / 180.0);
    //log
    ia_printZone(z);
}

void IAbyPath::ia_printZone(ZONE *z)
{
    //printf("ZONE: %s (%f,%f) w:%f h:%f start:%f,%f %f degrees\n", z->name, z->minX, z->minY, z->width, z->height, z->startX, z->startY, z->startAngle);
    logger().debug() << "ZONE: " << z->name << "(" << z->minX << "," << z->minY << ")" << " w:" << z->width << " h:"
            << z->height << " start: " << z->startX << "," << z->startY << "," << z->startAngle << logs::end;
}
void IAbyPath::ia_checkZones()
{
    if (_zones_count <= 0) {
        printf("%s (line %d) : Error : no zones defined\n", __FUNCTION__,
        __LINE__);
        sleep(1);
        exit(2);
    }

    for (int i = 0; i < _zones_count; i++) {
        ZONE *z = _zones[i];
        if (z->width <= 0) {
            printf("%s (line %d) : Error : negative width for zone %s\n", __FUNCTION__,
            __LINE__, z->name);
            sleep(1);
            exit(2);
        }
        if (z->height <= 0) {
            printf("%s (line %d) : Error : negative height for zone %s\n", __FUNCTION__,
            __LINE__, z->name);
            sleep(1);
            exit(2);
        }
    }
}

ZONE* IAbyPath::ia_getZone(const char *zoneName)
{
    int i = 0;
    for (i = 0; i < _zones_count; i++) {
        ZONE *z = _zones[i];
        if (strcmp(zoneName, z->name) == 0) {
            return z;
        }
    }
    return NULL;
}
ZONE* IAbyPath::ia_getZoneAt(float x, float y)
{
    logger().info() << __FUNCTION__ << " (line " << __LINE__ << ") : " << x << "," << y << logs::end;
    //printf("ia_getZoneAt : (%f,%f) \n", x, y);
    int i = 0;
    for (i = 0; i < _zones_count; i++) {
        ZONE *z = _zones[i];
        if (z->minX < x && z->minY < y && (z->minX + z->width) > x && (z->minY + z->height) > y) {
            return z;
        }
    }
    return NULL;
}

ZONE* IAbyPath::ia_getNearestZoneFrom(float x, float y)
{
    ZONE *result = ia_getZoneAt(x, y);
    if (result != NULL) {
//        printf("ia_getNearestZoneFrom is current zone : %s : (%f,%f) \n", result->name, robot_->passerv()->pos_getX_mm(),
//                robot_->passerv()->pos_getY_mm());
        return result;
    }

    int i = 0;
    result = _zones[i];
    float minDist2 = 8888888888888.0f;

    for (i = 0; i < _zones_count; i++) {

        ZONE *z = _zones[i];
        float x1 = z->minX + z->width / 2.0f;
        float y1 = z->minY + z->height / 2.0f;
        float dx = x1 - x;
        float dy = y1 - y;
        float d = dx * dx + dy * dy;
        if (d < minDist2) {
            minDist2 = d;
            result = z;
        }
//printf("ia_getNearestZoneFrom for %f; %s (%f,%f) ? (%f,%f)\n", d, z->name, x, y, x1, y1);
    }
    //printf("end ia_getNearestZoneFrom\n");
    return result;
}

//A renommer retrievezone
void IAbyPath::goToZone(const char *zoneName, ROBOTPOSITION *zone_p)
{

    ZONE *z = ia_getZone(zoneName);

    //printf("%s (line %d) : goToZone %s\n", __FUNCTION__, __LINE__, zoneName);

    logger().info() << __FUNCTION__ << " (line " << __LINE__ << ") : to " << zoneName << logs::end;
    if (z == NULL) {
        //printf("ERROR: %s %d : unable to get zone %s\n", __FUNCTION__, __LINE__, zoneName);
        logger().error() << __FUNCTION__ << " (line " << __LINE__ << ") : unable to get zone " << zoneName << logs::end;
        sleep(1);
        exit(-1);
    }

    zone_p->x = robot_->passerv()->getRelativeX(z->startX);
    zone_p->y = z->startY;
    //zone_p->theta = robot_->passerv()->getRelativeAngle(z->startAngle);
    zone_p->theta = robot_->passerv()->getRelativeAngleRad(degToRad(z->startAngle));

}

void IAbyPath::playgroundFindPath(FoundPath *&path, Point &start, Point &end)
{
    p_->find_path(path, start, end);
}
//TODO rename doPathForwardTo!
TRAJ_STATE IAbyPath::doMoveForwardTo(float xMM, float yMM, bool rotate_ignored_detection)
{
    TRAJ_STATE ts = TRAJ_OK;
    logger().debug() << "position p = x " << robot_->passerv()->pos_getX_mm() << " y "
            << robot_->passerv()->pos_getY_mm() << " a " << robot_->passerv()->pos_getThetaInDegree() << logs::end;

    //mise a jour de la position de l'adversaire
    //enable(robot_->, 0);

    Point endPoint = { x : robot_->passerv()->getRelativeX(xMM), y : yMM };
    FoundPath *found_path = NULL;

    Point startPoint = { x : robot_->passerv()->pos_getX_mm(), y : robot_->passerv()->pos_getY_mm() };
    playgroundFindPath(found_path, startPoint, endPoint);

    std::ostringstream path_polyline;
    std::vector<Node*>::iterator nodes_it;
    if (found_path != NULL) {
        if (found_path->cost == 0) {
            logger().info() << "PATH NOT FOUND - IMPOSSIBLE " << ", " << found_path->cost << logs::end;
            delete found_path;
            return TRAJ_IMPOSSIBLE;
        }
        int count = 0;

        for (nodes_it = found_path->path.begin(); nodes_it < found_path->path.end(); nodes_it++) {

            Node *node = *nodes_it;
            ts = TRAJ_OK;
            path_polyline << node->x << "," << -node->y << " ";

            if (count != 0) {

                logger().info() << " ======> PATH GOTO " << node->x << "," << node->y << logs::end;

                //ts = robot_->asserv()->doMoveForwardTo(robot_->asserv()->getRelativeX(node->x), node->y,
                //        rotate_ignored_detection); //inversement de x car doMoveForwardTo va aussi le refaire.

                //TODO SIMU A coder par l'asserv driver

                //if (nodes_it < found_path->path.end() - 1) {
                //ts = robot_->asserv()->gotoChain(robot_->asserv()->getRelativeX(node->x), node->y);

                if (rotate_ignored_detection) {
//                    logger().info() << " ======> PATH doFaceTo rotate_ignored_detection=false ! " << node->x << ","
//                            << node->y << logs::end;
                    //on tourne pour se degager si robot adverse devant
                    ts = robot_->passerv()->doFaceTo(robot_->passerv()->getRelativeX(node->x), node->y);
                    //rotate_ignored_detection = false;
                    robot_->svgPrintPosition();
                }

                ts = robot_->passerv()->gotoXY(robot_->passerv()->getRelativeX(node->x), node->y);
//                logger().info() << " ======> PATH GOTO ts=" << ts << " nodex,y=" << node->x << "," << node->y
//                        << logs::end;
                robot_->svgPrintPosition();
//                if (ts != TRAJ_FINISHED) {
//                    break;
//                }
//                } else {
//                    //ts = robot_->asserv()->gotoChain(robot_->asserv()->getRelativeX(node->x), node->y);
//                    ts = robot_->asserv()->gotoXY(robot_->asserv()->getRelativeX(node->x), node->y);
//
//                }

//                logger().debug() << "222 p = x " << robot_->asserv()->pos_getX_mm() << " y "
//                        << robot_->asserv()->pos_getY_mm() << " a " << robot_->asserv()->pos_getThetaInDegree()
//                        << logs::end;

            }
            count++;

        }

//        logger().debug() << "333 p = x " << robot_->asserv()->pos_getX_mm() << " y " << robot_->asserv()->pos_getY_mm()
//                << " a " << robot_->asserv()->pos_getThetaInDegree() << logs::end;

        robot_->svgw().pathPolyline(path_polyline.str());

    } else {
        logger().error() << __FUNCTION__ << " (line " << __LINE__ << ") : robot_ is NULL !" << logs::end;
    }
    delete found_path;

    return ts;
}

//TODO rename doPathForwardAndFaceTo
TRAJ_STATE IAbyPath::doMoveForwardAndFaceTo(float xMM, float yMM, float f_x, float f_y)
{
    TRAJ_STATE ts = TRAJ_OK;
    ts = doMoveForwardTo(xMM, yMM);
    if (ts != TRAJ_FINISHED) {
        return ts;
    }

    ts = robot_->passerv()->doFaceTo(f_x, f_y);
    robot_->svgPrintPosition();
    if (ts != TRAJ_FINISHED) {
        return ts;
    }
    return ts;

}

//TODO deprecated ? //TODO rename doPathForwardAndRotateTo
TRAJ_STATE IAbyPath::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree)
{
    TRAJ_STATE ts = TRAJ_OK;
    ts = doMoveForwardTo(xMM, yMM);
    if (ts != TRAJ_FINISHED) {
        return ts;
    }

    ts = robot_->passerv()->doAbsoluteRotateTo(thetaInDegree);
    robot_->svgPrintPosition();
    if (ts != TRAJ_FINISHED) {
        return ts;
    }
    return ts;
}

//goto avec l'asserv
//TRAJ_STATE IAbyPath::whileAsservMoveForwardTo(float xMM, float yMM, bool rotate_ignored_detection, int wait_tempo_us,
//        int nb_near_obstacle, int nb_collision, bool byPathfinding)
//{
//
//}
//TODO rename whilePathForwardAndRotateTo
TRAJ_STATE IAbyPath::whileMoveForwardTo(float xMM, float yMM, bool rotate_ignored_detection, int wait_tempo_us,
        int nb_near_obstacle, int nb_collision, bool byPathfinding, int reculOnObstacleMm, int reculOnCollisionMm)
{
    TRAJ_STATE ts = TRAJ_OK;
    int f = 0;
    int c = 0;
//    int nn = 0;
    while (ts != TRAJ_FINISHED) {
//        nn++;
//        logger().info() << "____________while ... doMoveForwardTo nn=" << nn << " byPathfinding=" << byPathfinding
//                << logs::end;
        if (byPathfinding) {
            //Avance par PathFinding
            //attention a la rotation si adversaire devant, pour cela on fait une rotation (faceTo simple car le gotoXY ne sait pas ignorer l'adversaire

            ts = doMoveForwardTo(xMM, yMM, rotate_ignored_detection);
        } else {
            //Avance avec l'asserv en direct
            ts = robot_->passerv()->doMoveForwardTo(xMM, yMM, rotate_ignored_detection);
        }

//        robot_->logger().error() << "byPathfinding=" << byPathfinding << "  TS = " << ts
//                << " reculOnCollisionMm=" << reculOnCollisionMm
//                << " reculOnObstacleMm=" << reculOnObstacleMm << logs::end;
        robot_->svgPrintPosition();
        robot_->displayTS(ts);

        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot_->logger().info() << " ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
            f++;
            if (f < nb_near_obstacle)
                robot_->passerv()->resetEmergencyOnTraj("IAbyPath whileMoveForwardTo TRAJ_NEAR_OBSTACLE"); //pour autoriser le level de detection 3 puis 4
            if (reculOnObstacleMm > 0) {
                TRAJ_STATE tr = robot_->passerv()->doLineAbs(-reculOnObstacleMm);
                if (tr != TRAJ_OK) {
                    robot_->passerv()->resetEmergencyOnTraj(
                            " IAbyPathdoLineAbs(-reculOnObstacleMm); TRAJ_NEAR_OBSTACLE"); //pour autoriser le level de detection 1 puis 2
                }
            }
            if (f >= nb_near_obstacle) {
                break;
            }
        }
        if (ts == TRAJ_COLLISION) {
            robot_->logger().info() << "===== COLLISION essai n°" << c << logs::end;
            c++;
            robot_->passerv()->resetEmergencyOnTraj("IAbyPath whileMoveForwardTo TRAJ_COLLISION");
            if (reculOnCollisionMm > 0) {
                //robot_->logger().info() << "IAbyPath::whileMoveForwardTo RECUL de mm=" << reculOnCollisionMm << logs::end;
                TRAJ_STATE tr = robot_->passerv()->doLineAbs(-reculOnCollisionMm);
                if (tr != TRAJ_OK) {
                    robot_->passerv()->resetEmergencyOnTraj("IAbyPath doLineAbs(-reculOnCollisionMm); TRAJ_COLLISION");
                }
            }
            if (c >= nb_collision) {
                break;
            }
        }
        if (ts == TRAJ_IMPOSSIBLE) {
            robot_->logger().info() << "===== TRAJ IMPOSSIBLE " << logs::end;

            robot_->passerv()->resetEmergencyOnTraj("whileMoveForwardTo TRAJ_IMPOSSIBLE");

            break;

        }
        if (ts == TRAJ_COLLISION || ts == TRAJ_NEAR_OBSTACLE) {
            //temps d'attente avant de recommencer
            utils::sleep_for_micros(wait_tempo_us);

            logger().info() << "WAITms=" << wait_tempo_us / 1000.0 << " AGAIN GOTO+ x=" << xMM << " y=" << yMM
                    << logs::end;
        }
        robot_->resetDisplayTS();
        //std::this_thread::yield();
    }

    robot_->displayTS(ts);
    logger().info() << "time= " << robot_->chrono().getElapsedTimeInMilliSec() << "ms " << " x="
            << robot_->passerv()->pos_getX_mm() << " y=" << robot_->passerv()->pos_getY_mm() << " a="
            << robot_->passerv()->pos_getThetaInDegree() << logs::end;

    //robot_->svgPrintPosition();

    return ts;
}
//TODO rename whilePathBackwardTo
TRAJ_STATE IAbyPath::whileMoveBackwardTo(float xMM, float yMM, bool rotate_ignored_detection, int wait_tempo_us,
        int nb_near_obstacle, int nb_collision, bool byPathfinding, int reculOnObstacleMm, int reculOnCollisionMm)
{
    TRAJ_STATE ts = TRAJ_OK;
    int f = 0;
    int c = 0;

    while (ts != TRAJ_FINISHED) {
        if (byPathfinding) {

            //TODOdoMoveBackwardTo
            //ts = doMoveBackwardTo(xMM, yMM, rotate_ignored_detection);
        } else {
            ts = robot_->passerv()->doMoveBackwardTo(xMM, yMM, rotate_ignored_detection);
        }
        if (ts != TRAJ_FINISHED) {
            robot_->logger().debug() << " TS = " << ts << logs::end;
            robot_->svgPrintPosition();
            robot_->displayTS(ts);

            if (ts == TRAJ_NEAR_OBSTACLE) {
                robot_->logger().info() << " ===== TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;
                f++;
                if (f < 2)
                    robot_->passerv()->resetEmergencyOnTraj(); //pour autoriser le level de detection
                if (reculOnObstacleMm > 0) {
                    TRAJ_STATE tr = robot_->passerv()->doLineAbs(reculOnObstacleMm);
                    if (tr != TRAJ_OK) {
                        robot_->passerv()->resetEmergencyOnTraj("doLineAbs(reculOnObstacleMm); TRAJ_NEAR_OBSTACLE"); //pour autoriser le level de detection 1 puis 2
                    }
                }
                if (f >= nb_near_obstacle) {
                    break;
                }
            }
            if (ts == TRAJ_COLLISION) {
                robot_->logger().info() << "===== COLLISION essai n°" << c << logs::end;
                c++;
                robot_->passerv()->resetEmergencyOnTraj();
                if (reculOnCollisionMm > 0) {
                    //robot_->logger().info() << "IAbyPath::whileMoveForwardTo RECUL de mm=" << reculOnCollisionMm << logs::end;
                    TRAJ_STATE tr = robot_->passerv()->doLineAbs(reculOnCollisionMm);
                    if (tr != TRAJ_OK) {
                        robot_->passerv()->resetEmergencyOnTraj("doLineAbs(reculOnCollisionMm); TRAJ_COLLISION"); //pour autoriser le level de detection 1 puis 2
                    }
                }
                if (c >= nb_collision) {
                    break;
                }
            }
            if (ts == TRAJ_IMPOSSIBLE) {
                robot_->logger().info() << "===== TRAJ IMPOSSIBLE " << logs::end;

                robot_->passerv()->resetEmergencyOnTraj();

                break;

            }

            if (ts == TRAJ_COLLISION || ts == TRAJ_NEAR_OBSTACLE) {
                //temps d'attente avant de recommencer
                utils::sleep_for_micros(wait_tempo_us);

                logger().info() << "WAITms=" << wait_tempo_us / 1000.0 << " AGAIN BAKWARD+ x=" << xMM << " y=" << yMM
                        << logs::end;
            }
            robot_->resetDisplayTS();
        }
        std::this_thread::yield();
    }

    robot_->displayTS(ts);
    logger().info() << "time= " << robot_->chrono().getElapsedTimeInMilliSec() << "ms " << " x="
            << robot_->passerv()->pos_getX_mm() << " y=" << robot_->passerv()->pos_getY_mm() << " a="
            << robot_->passerv()->pos_getThetaInDegree() << logs::end;

    //robot_->svgPrintPosition();

    return ts;
}

TRAJ_STATE IAbyPath::whileMoveRotateTo(float AbsoluteThetaInDegree, int wait_tempo_us, int nb_collision)
{
    //robot_->svgPrintPosition(1);
    //robot_->logger().info() << " ===== whileMoveRotateTo" << logs::end;

    TRAJ_STATE ts = TRAJ_OK;
    int f = 0;
    int c = 0;

    while (ts != TRAJ_FINISHED) {

        ts = robot_->passerv()->doAbsoluteRotateTo(AbsoluteThetaInDegree);

        robot_->logger().info() << " TS = " << ts << logs::end;
        robot_->svgPrintPosition(1);
        robot_->displayTS(ts);

        if (ts == TRAJ_NEAR_OBSTACLE) {
            robot_->logger().info() << " ===== NE DOIT PAS ARRIVER !!! TRAJ_NEAR_OBSTACLE essai n°" << f << logs::end;

            utils::sleep_for_micros(wait_tempo_us);
            f++;
            robot_->passerv()->resetEmergencyOnTraj();

            break;

        }
        if (ts == TRAJ_COLLISION) {
            robot_->logger().info() << "===== COLLISION essai n°" << c << logs::end;

            utils::sleep_for_micros(wait_tempo_us);
            c++;
            robot_->passerv()->resetEmergencyOnTraj();
            if (c > nb_collision) {

                break;
            }
        }
        robot_->resetDisplayTS();
        logger().debug() << "AGAIN ROTATE AbsoluteThetaInDegree=" << AbsoluteThetaInDegree << logs::end;
        std::this_thread::yield();
    }

    logger().info() << "time= " << robot_->chrono().getElapsedTimeInMilliSec() << "ms " << " x="
            << robot_->passerv()->pos_getX_mm() << " y=" << robot_->passerv()->pos_getY_mm() << " a="
            << robot_->passerv()->pos_getThetaInDegree() << logs::end;

    //robot_->svgPrintPosition(1);
    return ts;
}

TRAJ_STATE IAbyPath::whileMoveForwardAndRotateTo(float xMM, float yMM, float absoluteThetaInDegree,
        bool rotate_ignored_detection, int wait_tempo_us, int nb_near_obstacle, int nb_collision, bool byPathfinding,
        int reculMm)
{
    TRAJ_STATE ts = TRAJ_OK;
    ts = whileMoveForwardTo(xMM, yMM, rotate_ignored_detection, wait_tempo_us, nb_near_obstacle, nb_collision,
            byPathfinding, reculMm, reculMm);
    if (ts != TRAJ_FINISHED) {
        return ts;
    }

    whileMoveRotateTo(absoluteThetaInDegree, wait_tempo_us, nb_collision); //TODO que faire de cette info ???
    return TRAJ_FINISHED;
}

TRAJ_STATE IAbyPath::whileMoveBackwardAndRotateTo(float xMM, float yMM, float absoluteThetaInDegree,
        bool rotate_ignored_detection, int wait_tempo_us, int nb_near_obstacle, int nb_collision, bool byPathfinding,
        int reculMm)
{
    TRAJ_STATE ts = TRAJ_OK;
    ts = whileMoveBackwardTo(xMM, yMM, rotate_ignored_detection, wait_tempo_us, nb_near_obstacle, nb_collision,
            byPathfinding, reculMm, reculMm);
    if (ts != TRAJ_FINISHED) {
        return ts;
    }

    whileMoveRotateTo(absoluteThetaInDegree, wait_tempo_us, nb_collision); //TODO que faire de cette info ???
    return TRAJ_FINISHED;
}
