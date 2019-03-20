#if CONFIG_MOTORCTRL_BOTMOTORS

#ifndef BOTMOTORS
#define BOTMOTORS

#include "MotorsController.h"

class Robot;

class BotMotors: public MotorsController
{
public:

    // Constructeur
    BotMotors(Robot * robot);
    // Destructeur
    virtual ~BotMotors();

    // Paramétrage des vitesses
    virtual void setVitesseG(int vitMoteurG);
    virtual void setVitesseD(int vitMoteurD);
    virtual int getVitesseG(void)
    {
        return vitMoteurG;
    }
    virtual int getVitesseD(void)
    {
        return vitMoteurD;
    }

private:
    /*!
     *\brief ref vers le robot lié.
     */
    Robot * robot_;

    int vitMoteurG, vitMoteurD;

};

#endif /* BOTMOTORS */

#endif /* CONFIG_MOTORCTRL_BOTMOTORS */
