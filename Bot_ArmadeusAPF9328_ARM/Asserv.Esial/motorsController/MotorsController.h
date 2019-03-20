#ifndef MOTORCONTROLLER
#define MOTORCONTROLLER


#include "../config/config.h"

// interface du contrôleur de moteurs

class MotorsController
{

public:
    virtual void setVitesseG(int vitMoteurG) = 0;
    virtual void setVitesseD(int vitMoteurD) = 0;
    virtual int getVitesseG(void) = 0;
    virtual int getVitesseD(void) = 0;
    virtual ~MotorsController() { }

    void inverseMoteurDroit()
    {
        Config::inverseMoteurG = !Config::inverseMoteurG;
    }
    void inverseMoteurGauche()
    {
        Config::inverseMoteurD = !Config::inverseMoteurD;
    }
    void echangeMoteurs()
    {
        Config::swapMoteurs = !Config::swapMoteurs;
    }
};

#endif
