#if CONFIG_CODEUR_DIRECTS

#ifndef CODEUR
#define CODEUR


#include "../config/config.h"

//Un codeur branché directement sur la Mbed

class Codeur
{

public:

    Codeur(PinName pinChanA, PinName pinChanB);
    ~Codeur();
    int32_t getCount();
    void reset();

private:

    void handleInterruptA();
    void handleInterruptB();

    volatile int32_t count;
    InterruptIn pinChannelA;
    InterruptIn pinChannelB;

};

#endif /* CODEUR */

#endif /* CONFIG_CODEUR_DIRECTS */
