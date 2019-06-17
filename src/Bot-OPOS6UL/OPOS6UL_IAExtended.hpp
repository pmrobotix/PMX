#ifndef OPOS6UL_IAEXTENDED_HPP_
#define OPOS6UL_IAEXTENDED_HPP_

#include <src/pmr_symmetrical_pg.h>
#include <string>

#include "../Common/IA/IAbyPath.hpp"
#include "../Common/IA/IAbyZone.hpp"

//Definition de ce qui existe pour ce robot, quelle ia il utilise
class OPOS6UL_IAExtended
{
private:

    IAbyZone iaz_; //old IA
    IAbyPath iap_; //new IA

    //Playground *p_;
    SymmetricalPlayground * p_;

public:

    PlaygroundObjectID opponent_1;
    //PlaygroundObjectID opponent_2;
    PlaygroundObjectID area_alea_violet;
    PlaygroundObjectID area_alea_yellow;
    PlaygroundObjectID area_palet_start_violet;
    PlaygroundObjectID area_palet_start_yellow;
    PlaygroundObjectID area_palet_start2_violet;
    PlaygroundObjectID area_palet_start2_yellow;

    OPOS6UL_IAExtended(std::string botId, Robot *robot);

    ~OPOS6UL_IAExtended()
    {

    }

    //old IA
    IAbyZone & iAbyZone()
    {
        return iaz_;
    }

    //New IA using pathfinding
    IAbyPath & iAbyPath()
    {
        return iap_;
    }

    void initPlayground();

    void move_adv(float x_mm, float y_mm);

};

#endif
