#ifndef OPOS6UL_IAEXTENDED_HPP_
#define OPOS6UL_IAEXTENDED_HPP_

#include <src/pmr_symmetrical_pg.h>
#include <string>

#include "../Common/IA/IAbyPath.hpp"
#include "../Common/IA/IAbyZone.hpp"

//Definition de ce qui existe pour ce robot, quelle ia il utilise
class OPOS6UL_IAExtended {
private:

    IAbyZone iaz_; //old IA
    IAbyPath iap_; //new IA

    //Playground *p_;
    SymmetricalPlayground * p_;

public:

    PlaygroundObjectID opponent_1;
    PlaygroundObjectID opponent_2;
    PlaygroundObjectID opponent_3;
    PlaygroundObjectID opponent_4;

    PlaygroundObjectID area_alea_violet;
    PlaygroundObjectID area_alea_yellow;

    PlaygroundObjectID area_3_start_violet;
    PlaygroundObjectID area_3_start_yellow;

    PlaygroundObjectID area_triangle_violet;
    PlaygroundObjectID area_triangle_yellow;

    PlaygroundObjectID area_distrib_violet;
    PlaygroundObjectID area_distrib_yellow;

    PlaygroundObjectID area_galerie_violet;
    PlaygroundObjectID area_galerie_yellow;

//    PlaygroundObjectID area_alea_yellow;
//    PlaygroundObjectID area_alea_blue;
//
//    PlaygroundObjectID area_v_vert_gauche_yellow;
//    PlaygroundObjectID area_v_rouge_droite_yellow;
//    PlaygroundObjectID area_v_vert_gauche_blue;
//    PlaygroundObjectID area_v_rouge_droite_blue;
//    PlaygroundObjectID area_v_vert_droite_zoneNS_yellow;
//    PlaygroundObjectID area_v_rouge_gauche_zoneSN_blue;
//    PlaygroundObjectID area_v_vert_droite_zoneNS_blue;
//    PlaygroundObjectID area_v_rouge_gauche_zoneSN_yellow;
//
//    PlaygroundObjectID area_v_vert_devant_yellow;
//    PlaygroundObjectID area_v_vert_devant_blue;
//    PlaygroundObjectID area_v_rouge_devant_droite_yellow;
//    PlaygroundObjectID area_v_rouge_devant_droite_blue;
//    PlaygroundObjectID area_v_rouge_devant_gauche_milieu_terrain_yellow;
//    PlaygroundObjectID area_v_rouge_devant_gouche_milieu_terrain_blue;

    OPOS6UL_IAExtended(std::string botId, Robot *robot);

    ~OPOS6UL_IAExtended() {

    }

    //old IA
    IAbyZone & iAbyZone() {
        return iaz_;
    }

    //New IA using pathfinding
    IAbyPath & iAbyPath() {
        return iap_;
    }

    void initPlayground();

    void move_adv(float x_mm, float y_mm);

};

#endif
