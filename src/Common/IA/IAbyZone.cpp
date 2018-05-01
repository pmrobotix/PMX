/*
 * IAbyZone.cpp
 *
 *  Created on: 14 avr. 2016
 *      Author: pmx
 */

#include "IAbyZone.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>

#include "../../Log/Logger.hpp"
#include "../../Log/SvgWriter.hpp"
#include "../Asserv/Asserv.hpp"
#include "../Robot.hpp"

IAbyZone::IAbyZone(Robot * robot)
{
    ia_clear();
    robot_ = robot;
}

void IAbyZone::ia_clear()
{
    _zones_count = 0;
    _zones_path_count = 0;
    _actions_count = 0;
}
void IAbyZone::ia_addAction(const char* name, RobotAction action)
{
    ACTIONS *a = (ACTIONS*) calloc(1, sizeof(ACTIONS));
    strcpy(a->name, name);
    a->action = action;
    a->completed = false;
    _actions[_actions_count] = a;
    _actions_count++;
}

void IAbyZone::ia_createZone(const char* name, float minX, float minY, float width, float height, float startX, float startY, float startAngleDeg)
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

    robot_->svgw().writeZone(z->name, z->minX, z->minY, z->width, z->height, z->startX, z->startY, z->startAngle * M_PI / 180.0);
    //ia_printZone(z);
}

void IAbyZone::ia_printZone(ZONE *z)
{
    printf("ZONE: %s (%f,%f) w:%f h:%f start:%f,%f %f degrees\n", z->name, z->minX, z->minY, z->width, z->height, z->startX, z->startY, z->startAngle);
}
void IAbyZone::ia_checkZones()
{
    if (_zones_count <= 0) {
        printf("%s (line %d) : Error : no zones defined\n", __FUNCTION__,
        __LINE__);
        exit(2);
    }

    int i, j;
    for (i = 0; i < _zones_count; i++) {
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
        // Check existing zone path
        for (j = 0; j < _zones_count; j++) {
            if (i != j) {
                ZONE *z2 = _zones[j];
                ZONE_PATH* p = ia_getZonePath(z, z2);
                if (p == NULL) {
                    printf("%s (line %d) : Error : no path from %s and %s\n", __FUNCTION__,
                    __LINE__, z->name, z2->name);
                    exit(2);
                }
            }
        }
    }
}
void IAbyZone::ia_setPath(const char* zone1Name, const char* zone2Name, float x, float y)
{
    ZONE_PATH *zp = (ZONE_PATH*) calloc(1, sizeof(ZONE_PATH));
    zp->z1 = ia_getZone(zone1Name);
    zp->z2 = ia_getZone(zone2Name);
    zp->x = x;
    zp->y = y;

    /*
     //printf("ia_setPath cc_getMatchColor=%d\n", cc_getMatchColor());
     if (cc_getMatchColor() != 0)
     {
     //yMM = -yMM;
     zp->x = 3000 - zp->x;
     }*/

    if (robot_ != NULL) {
        zp->x = robot_->asserv_default->getRelativeX(zp->x);
    } else {
        logger().error() << "robot_ is NULL !" << logs::end;
        exit(-1);
    }

    _zones_path[_zones_path_count] = zp;
    _zones_path_count++;

    robot_->svgw().writeIaPath(zone1Name, zone2Name, zp->x, zp->y);

}

ZONE_PATH* IAbyZone::ia_getZonePath(ZONE *z1, ZONE *z2)
{
    int i;
    printf("z1 z2 %s - %s\n", z1->name, z2->name);
    for (i = 0; i < _zones_path_count; i++) {
        ZONE_PATH *zp = _zones_path[i];

        printf("zp1 zp2 %s - %s\n", zp->z1->name, zp->z2->name);
        if ((zp->z1 == z1 && zp->z2 == z2) || (zp->z1 == z2 && zp->z2 == z1)) {
            return zp;
        }
    }
    printf("ia_getZonePath return NULL!!!");
    return NULL;
}

void IAbyZone::ia_start()
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
                        printf("%s state after actions : %s : (%f,%f) %f FAILED\n", __FUNCTION__, z->name, robot_->asserv_default->pos_getX_mm(),
                                robot_->asserv_default->pos_getY_mm(), robot_->asserv_default->pos_getThetaInDegree());
                    else {
                        logger().error() << "robot_ is NULL !" << logs::end;
                        exit(-1);
                    }

                }
                if (robot_ != NULL)
                    printf("%s state after actions : %s : (%f,%f) %f\n", __FUNCTION__, z->name, robot_->asserv_default->pos_getX_mm(), robot_->asserv_default->pos_getY_mm(),
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

ZONE* IAbyZone::ia_getZone(const char* zoneName)
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
ZONE* IAbyZone::ia_getZoneAt(float x, float y)
{
    printf("ia_getZoneAt : (%f,%f) \n", x, y);
    int i = 0;
    for (i = 0; i < _zones_count; i++) {
        ZONE *z = _zones[i];
        if (z->minX <= x && z->minY <= y && (z->minX + z->width) >= x && (z->minY + z->height) >= y) {
            return z;
        }
    }
    return NULL;
}

ZONE* IAbyZone::ia_getNearestZoneFrom(float x, float y)
{
    ZONE *result = ia_getZoneAt(x, y);
    if (result != NULL) {
        printf("ia_getNearestZoneFrom is current zone : %s : (%f,%f) \n", result->name, robot_->asserv_default->pos_getX_mm(), robot_->asserv_default->pos_getY_mm());
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
            //printf("0000005result %s \n", result->name);
        }
//printf("ia_getNearestZoneFrom for %f; %s (%f,%f) ? (%f,%f)\n", d, z->name, x, y, x1, y1);
    }
    printf("end ia_getNearestZoneFrom\n");
    return result;
}

void IAbyZone::goToZone(const char *zoneName, RobotPosition *path_p, RobotPosition *zone_p)
{
    ZONE* z = ia_getZone(zoneName);

    printf("%s (line %d) : goToZone %s\n", __FUNCTION__, __LINE__, zoneName);

    if (z == NULL) {
        printf("ERROR: %s %d : unable to get zone %s\n", __FUNCTION__, __LINE__, zoneName);
        exit(-1);
    }

    ZONE *zCurrent = ia_getNearestZoneFrom(robot_->asserv_default->pos_getX_mm(), robot_->asserv_default->pos_getY_mm());
    if (zCurrent == NULL) {
        printf("ERROR: cc_goToZone ia_getNearestZoneFrom return NULL !!");
        exit(-1);
    }
    //printf("01\n");
    ZONE_PATH *path = ia_getZonePath(zCurrent, z);
    //printf("02\n");
    /*
     if (path != NULL)
     {
     printf("%s (line %d) : goToZone FROM %s TO %s using path (%f,%f)\n", __FUNCTION__, __LINE__,
     zCurrent->name, z->name, path->x, path->y);
     ts = cc_moveForwardTo(cc_getRelativeX(path->x), path->y, cc_motion_GetDefaultSpeed(), cc_motion_GetDefaultAccel(), cc_motion_GetDefaultDecel());
     if (ts != TRAJ_OK)
     {
     return ts;
     }
     }
     else
     {
     printf("%s (line %d) : goToZone FROM %s TO %s with NO path \n", __FUNCTION__, __LINE__,
     zCurrent->name, z->name);
     }
     ts = cc_moveForwardAndRotateTo(cc_getRelativeX(z->startX), z->startY,
     cc_getRelativeAngle(z->startAngle), cc_motion_GetDefaultSpeed(), cc_motion_GetDefaultAccel(), cc_motion_GetDefaultDecel());
     return ts;
     */
    if (path != NULL) {
        printf("%s (line %d) : goToZone FROM %s TO %s using path (%f,%f)\n", __FUNCTION__, __LINE__, zCurrent->name, z->name, path->x, path->y);
        path_p->x = robot_->asserv_default->getRelativeX(path->x);
        path_p->y = path->y;
    }/*else
     {
     printf("%s (line %d) : goToZone FROM %s TO %s with NO path \n", __FUNCTION__, __LINE__,
     zCurrent->name, z->name);
     path_p->x = robot_->asserv_default->getRelativeX(z->startX);
     path_p->y = z->startY;
     path_p->theta = robot_->asserv_default->getRelativeAngle(z->startAngle);
     }*/
    zone_p->x = robot_->asserv_default->getRelativeX(z->startX);
    zone_p->y = z->startY;
    zone_p->theta = robot_->asserv_default->getRelativeAngle(z->startAngle);

    printf("----%s (line %d) : goToZone FROM %s TO %s using path (%f,%f)\n", __FUNCTION__, __LINE__, zCurrent->name, z->name, path->x, path->y);
    //printf("03\n");
    /*
     RobotPosition pos;

     //convert position from ticks to meter
     pos.x = path->x;
     pos.y = path->y;
     pos.theta = z->startAngle;

     return pos;*/
}

