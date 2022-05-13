#ifndef ASSERV_E_DUMMYMOTORSCONTROLLER
#define ASSERV_E_DUMMYMOTORSCONTROLLER

#include "MotorsController.h"

class DummyMotorsController: public MotorsController
{
private:
    int vitMoteurG = 0;
    int vitMoteurD = 0;

public :
    void setVitesseG(int vitMoteurG) { this->vitMoteurG = vitMoteurG; }
    void setVitesseD(int vitMoteurD) { this->vitMoteurD = vitMoteurD; }
    int getVitesseG(void) { return this->vitMoteurG; }
    int getVitesseD(void) { return this->vitMoteurD; }
    ~DummyMotorsController() { }
};

#endif
