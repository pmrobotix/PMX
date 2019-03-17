#if CONFIG_MOTORCTRL_BOTMOTORS

#ifndef ASSERV_ESIAL_CODEURS_BOTCODEURS_H_
#define ASSERV_ESIAL_CODEURS_BOTCODEURS_H_

#include "CodeursInterface.h"

class Robot;

#include <sys/types.h>

//Deux codeurs, utilisant les drivers robot
class BotCodeurs: public CodeursInterface
{

public:
    BotCodeurs(Robot * robot);
    ~BotCodeurs();
    virtual void getCounts(int32_t* countG, int32_t* countD);

private:
    /*!
     *\brief ref vers le robot lié.
     */
    Robot * robot_;

    int32_t lastCountD;
    int32_t lastCountG;
};

#endif /* ASSERV_ESIAL_CODEURS_BOTCODEURS_H_ */

#endif /* CONFIG_MOTORCTRL_BOTMOTORS */
