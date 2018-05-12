#ifndef LegoEV3_IAEXTENDED_HPP_
#define LegoEV3_IAEXTENDED_HPP_

#include <src/pmr_playground.h>
#include <string>

#include "../Common/IA/IAbyPath.hpp"
#include "../Common/IA/IAbyZone.hpp"

struct FoundPath;
struct Point;

class LegoEV3IAExtended
{
private:

    IAbyZone iaz_;
    IAbyPath iap_;

    Playground *p_;

public:

    PlaygroundObjectID oarea_cube1;
    PlaygroundObjectID oarea_cube2high;
    PlaygroundObjectID oarea_cube3left;
    PlaygroundObjectID oarea_distribadverse;
    PlaygroundObjectID oarea_buildzone;

    PlaygroundObjectID garea_cube1;
    PlaygroundObjectID garea_cube2high;
    PlaygroundObjectID garea_cube3left;
    PlaygroundObjectID garea_distribadverse;
    PlaygroundObjectID garea_buildzone;

    PlaygroundObjectID area_stations;

    LegoEV3IAExtended(std::string botId, Robot *robot);

    ~LegoEV3IAExtended();

    IAbyZone & iAbyZone()
    {
        return iaz_;
    }

    IAbyPath & iAbyPath()
    {
        return iap_;
    }
    /*
     void initPlayground();
     void playgroundComputeEdges();
     void playgroundFindPath(FoundPath * & path, Point& start, Point& end);
     */
};

#endif
