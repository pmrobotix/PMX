#if CONFIG_CODEUR_DIRECTS

#include "CodeursDirects.h"

CodeursDirects::CodeursDirects(PinName pinChanA_G, PinName pinChanB_G, PinName pinChanA_D, PinName pinChanB_D) :
        codeurG(pinChanA_G, pinChanB_G), codeurD(pinChanA_D, pinChanB_D)
{
    // On change la priorité de l'interruption timer pour qu'elle soit plus basse que celle des ticks codeurs
    NVIC_SetPriority(TIMER3_IRQn, 1);
    lastCountD = 0;
    lastCountG = 0;
}

CodeursDirects::~CodeursDirects()
{
}

void CodeursDirects::getCounts(int32_t* countG, int32_t* countD)
{
    /*
     * On récupère le compte des codeurs depuis le dernier refresh.
     * Pour ne pas avoir à resetter les compteurs à chaque refresh (et
     * donc risquer de perdre un ou deux ticks codeurs), on compare à la
     * valeur précédente des compteurs. Il *pourrait* y avoir un
     * overflow si l'un des codeurs génère plus de 2^31 ticks par
     * refresh, càd si le robot dépasse le mur du son...
     */
    int32_t tempD = codeurD.getCount();
    *countD = tempD - lastCountD;
    lastCountD = tempD;

    int32_t tempG = codeurG.getCount();
    *countG = tempG - lastCountG;
    lastCountG = tempG;

    // On ajuste le sens des codeurs en cas de problème de cablage
    if (Config::swapCodeurs) { //inversion des deux codeurs
        int32_t temp = *countG;
        *countG = *countD;
        *countD = temp;
    }

    if (Config::inverseCodeurD) { //Changement de sens de rotation du codeur droit
        *countD = -(*countD);
    }

    if (Config::inverseCodeurG) { //Changement de sens de rotation du codeur gauche
        *countG = -(*countG);
    }

}

#endif /* CONFIG_CODEUR_DIRECTS */
