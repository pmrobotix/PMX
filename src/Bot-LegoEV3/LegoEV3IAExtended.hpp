#ifndef LegoEV3_IAEXTENDED_HPP_
#define LegoEV3_IAEXTENDED_HPP_

#include <src/pmr_playground.h>
#include <string>

#include "../Common/IA/IAbyPath.hpp"
#include "../Common/IA/IAbyZone.hpp"

struct FoundPath;
struct Point;

class LegoEV3IAExtended //: public IA
{
private:

    IAbyZone iaz_;
    IAbyPath iap_;

    Playground *p_;

public:

    //Configuration per robot and strategy
    PlaygroundObjectID area_alea_violet;
    PlaygroundObjectID area_alea_yellow;

    PlaygroundObjectID area_palet_start_violet;
    PlaygroundObjectID area_palet_start_yellow;




    LegoEV3IAExtended(std::string botId, Robot *robot);

    ~LegoEV3IAExtended()
    {
    }

    IAbyZone & iAbyZone()
    {
        return iaz_;
    }

    IAbyPath & iAbyPath()
    {
        return iap_;
    }

    void initPlayground();

};

#endif
