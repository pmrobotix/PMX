/*
 * IAbyPath.cpp
 *
 *  Created on: 4 mai 2017
 *      Author: pmx
 */

#include "IAbyPath.hpp"

#include <simple_svg_1.0.0.hpp>
#include <src/pmr_edge.h>
#include <src/pmr_node.h>
#include <src/pmr_path_result.h>
#include <src/pmr_pathfinding.h>
#include <src/pmr_point.h>
#include <src/pmr_zone.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <cstring>
#include <iterator>
#include <vector>

#include "../../Log/Logger.hpp"
#include "../../Log/SvgWriter.hpp"
#include "../Asserv/Asserv.hpp"

IAbyPath::IAbyPath(Robot *robot)
{
    ia_clear();
    robot_ = robot;
    p_ = NULL;
}

void IAbyPath::addPlayground(Playground * p)
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
        logger().error() << "!! Playground is NULL !! " << logs::end;
    } else {
        PathFinder * pf = p_->get_path_finder();
        std::vector<Zone*>::iterator zones_it;
        std::vector<Edge*>::iterator edges_it;
        std::vector<Node*>::iterator nodes_it;
        unsigned int i;

        svg::Dimensions dimensions(pf->field_x2, pf->field_y2);
        svg::Layout lay(dimensions, svg::Layout::TopLeft);

        svg::Document doc("ia", lay);

        doc << svg::elemStart("g") << svg::attribute("transform", "translate(200,2200) scale(1,-1)")
                << svg::emptyElemEnd(false);

        // Red image border.
        svg::Polygon border(svg::Fill(svg::Color::White), svg::Stroke(5, svg::Color::Red));
        border << svg::Point(pf->field_x1, pf->field_y1) << svg::Point(dimensions.width, pf->field_y1)
                << svg::Point(dimensions.width, dimensions.height) << svg::Point(pf->field_x1, dimensions.height);
        doc << border;

        // Display all zones
        for (zones_it = pf->zones.begin(); zones_it < pf->zones.end(); zones_it++) {
            Zone* zone = *zones_it;
            if (zone->nodes_count > 0) {
                svg::Polygon zone_poly(svg::Fill(svg::Color::Aqua), svg::Stroke(0, svg::Color::Aqua));
                for (i = 0; i < zone->nodes_count; i++) {
                    Node* node = zone->nodes[i];
                    zone_poly << svg::Point(node->x, node->y);
                }
                doc << zone_poly;
            }
        }

        // Display all enabled edges
        for (edges_it = pf->edges.begin(); edges_it < pf->edges.end(); edges_it++) {
            Edge* edge = *edges_it;
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
        printf("%s (line %d) : Error : no actions defined\n", __FUNCTION__, __LINE__);
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
                        printf("%s state after actions : %s : (%f,%f) %f FAILED\n", __FUNCTION__, z->name,
                                robot_->asserv_default->pos_getX_mm(), robot_->asserv_default->pos_getY_mm(),
                                robot_->asserv_default->pos_getThetaInDegree());
                    else {
                        logger().error() << "robot_ is NULL !" << logs::end;
                        exit(-1);
                    }

                }
                if (robot_ != NULL)
                    printf("%s state after actions : %s : (%f,%f) %f\n", __FUNCTION__, z->name,
                            robot_->asserv_default->pos_getX_mm(), robot_->asserv_default->pos_getY_mm(),
                            robot_->asserv_default->pos_getThetaInDegree());
                else {
                    logger().error() << "robot_ is NULL !" << logs::end;
                    exit(-1);
                }
            }

        }
        //sleep(1);
    }
}

void IAbyPath::ia_clear()
{
    _zones_count = 0;
    //_zones_path_count = 0;
    _actions_count = 0;
}
void IAbyPath::ia_addAction(const char* name, RobotAction action)
{
    ACTIONS *a = (ACTIONS*) calloc(1, sizeof(ACTIONS));
    strcpy(a->name, name);
    a->action = action;
    a->completed = false;
    _actions[_actions_count] = a;
    _actions_count++;
}
void IAbyPath::ia_createZone(const char* name, float minX, float minY, float width, float height, float startX,
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
        z->startX = robot_->asserv_default->getRelativeX(z->startX);
        z->minX = robot_->asserv_default->getRelativeX(z->minX, z->width);
        z->startAngle = robot_->asserv_default->getRelativeAngle(z->startAngle);
    } else {
        logger().error() << "robot_ is NULL !" << logs::end;
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
    logger().info() << "ZONE: " << z->name << "(" << z->minX << "," << z->minY << ")" << " w:" << z->width << " h:"
            << z->height << " start: " << z->startX << "," << z->startY << "," << z->startAngle << logs::end;
}
void IAbyPath::ia_checkZones()
{
    if (_zones_count <= 0) {
        printf("%s (line %d) : Error : no zones defined\n", __FUNCTION__,
        __LINE__);
        exit(2);
    }

    for (int i = 0; i < _zones_count; i++) {
        ZONE *z = _zones[i];
        if (z->width <= 0) {
            printf("%s (line %d) : Error : negative width for zone %s\n", __FUNCTION__,
            __LINE__, z->name);
            exit(2);
        }
        if (z->height <= 0) {
            printf("%s (line %d) : Error : negative height for zone %s\n", __FUNCTION__,
            __LINE__, z->name);
            exit(2);
        }
    }
}

ZONE* IAbyPath::ia_getZone(const char* zoneName)
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
    printf("ia_getZoneAt : (%f,%f) \n", x, y);
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
        printf("ia_getNearestZoneFrom is current zone : %s : (%f,%f) \n", result->name,
                robot_->asserv_default->pos_getX_mm(), robot_->asserv_default->pos_getY_mm());
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
    printf("end ia_getNearestZoneFrom\n");
    return result;
}

//A renommer retrievezone
void IAbyPath::goToZone(const char *zoneName, RobotPosition *zone_p)
{

    ZONE* z = ia_getZone(zoneName);

    printf("%s (line %d) : goToZone %s\n", __FUNCTION__, __LINE__, zoneName);

    if (z == NULL) {
        printf("ERROR: %s %d : unable to get zone %s\n", __FUNCTION__, __LINE__, zoneName);
        exit(-1);
    }

    zone_p->x = robot_->asserv_default->getRelativeX(z->startX);
    zone_p->y = z->startY;
    zone_p->theta = robot_->asserv_default->getRelativeAngle(z->startAngle);

}

TRAJ_STATE IAbyPath::doMoveForwardAndRotateTo(float xMM, float yMM, float thetaInDegree) //zone toujours donné du côté gauche (ORANGE), à transcrire
{
    TRAJ_STATE ts = TRAJ_OK;
    logger().debug() << "111 p = x " << robot_->asserv_default->pos_getX_mm() << " y "
            << robot_->asserv_default->pos_getY_mm() << " a " << robot_->asserv_default->pos_getThetaInDegree()
            << logs::end;

    Point endPoint = { x : robot_->asserv_default->getRelativeX(xMM), y : yMM };
    FoundPath * found_path = NULL;

    Point startPoint = { x : robot_->asserv_default->pos_getX_mm(), y : robot_->asserv_default->pos_getY_mm() };
    playgroundFindPath(found_path, startPoint, endPoint);


    std::ostringstream path_polyline;
    std::vector<Node*>::iterator nodes_it;
    if (found_path != NULL) {
        if (found_path->cost == 0) {
            logger().info() << "PATH NOT FOUND - CANCELLED " << ", " << found_path->cost << logs::end;
            delete found_path;
            return TRAJ_CANCELLED;
        }

        int count = 0;
        for (nodes_it = found_path->path.begin(); nodes_it < found_path->path.end(); nodes_it++) {

            Node* node = *nodes_it;

            path_polyline << node->x << "," << -node->y << " ";

            if (count != 0) {

                logger().info() << "GOTO - PATH to " << node->x << "," << node->y << logs::end;

                ts = robot_->asserv_default->doMoveForwardTo(robot_->asserv_default->getRelativeX(node->x), node->y); //inversement de x car doMoveForwardTo va aussi le refaire.
                if (ts != TRAJ_OK) {
                    return ts;
                }
                logger().debug() << "222 p = x " << robot_->asserv_default->pos_getX_mm() << " y "
                        << robot_->asserv_default->pos_getY_mm() << " a "
                        << robot_->asserv_default->pos_getThetaInDegree() << logs::end;
                robot_->svgPrintPosition();
            }
            count++;

        }

        logger().debug() << "333 p = x " << robot_->asserv_default->pos_getX_mm() << " y "
                << robot_->asserv_default->pos_getY_mm() << " a " << robot_->asserv_default->pos_getThetaInDegree()
                << logs::end;

        robot_->svgw().pathPolyline(path_polyline.str());

        ts = robot_->asserv_default->doRotateTo(thetaInDegree); //les angles du path sont bons
        robot_->svgPrintPosition();
        if (ts != TRAJ_OK) {
            return ts;
        }

    } else {
        logger().error() << "ERROR - FOUND PATH NULL !!!" << logs::end;
    }

    delete found_path;

    return ts;
}

void IAbyPath::playgroundFindPath(FoundPath * & path, Point& start, Point& end)
{
    p_->find_path(path, start, end);
}
