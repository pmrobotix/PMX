#if CONFIG_CODEUR_DIRECTS

#ifndef ASSERV_E_CODEUR_DIRECT
#define ASSERV_E_CODEUR_DIRECT


#include "../config/config.h"
#include "CodeursInterface.h"
#include "Codeur.h"

//Deux codeurs, branchés directement sur la Mbed

class CodeursDirects: public CodeursInterface
{

public:
    CodeursDirects(PinName pinChanA_G, PinName pinChanB_G, PinName pinChanA_D, PinName pinChanB_D);
    ~CodeursDirects();
    virtual void getCounts(int32_t* countG, int32_t* countD);

private:
    Codeur codeurG;
    Codeur codeurD;

    int32_t lastCountD;
    int32_t lastCountG;
};

#endif /* CODEUR_DIRECT */

#endif /* CONFIG_CODEUR_DIRECTS */
