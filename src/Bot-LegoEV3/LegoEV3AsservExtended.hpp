#ifndef LegoEV3_ASSERVEXTENDED_HPP_
#define LegoEV3_ASSERVEXTENDED_HPP_

#include <string>

#include "../Common/Asserv/Asserv.hpp"
#include "../Log/LoggerFactory.hpp"

class LegoEV3AsservExtended: public Asserv
{
private:
    /*!
     * \brief Return \ref Logger linked to \ref LegoEV3AsservExtended.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("LegoEV3AsservExtended");
        return instance;
    }

public:
    LegoEV3AsservExtended(std::string botId, Robot * robot);

    ~LegoEV3AsservExtended()
    {
    }

    void startMotionTimerAndOdo(bool assistedHandlingEnabled);


};

#endif

