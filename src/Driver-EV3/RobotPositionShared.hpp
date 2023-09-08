#ifndef EV3_ROBOTPOSITIONSHARED_HPP_
#define EV3_ROBOTPOSITIONSHARED_HPP_


#include "../Common/Interface.Driver/ARobotPositionShared.hpp"
#include "../Log/LoggerFactory.hpp"

class RobotPositionShared: public ARobotPositionShared
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref RobotPositionShared(EV3).
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("RobotPositionShared.EV3");
        return instance;
    }


public:

    /*!
     * \brief Constructor.
     */
    RobotPositionShared();

    /*!
     * \brief Destructor.
     */
    ~RobotPositionShared();

    ROBOTPOSITION getRobotPosition();

    void setRobotPosition(ROBOTPOSITION p);

};

#endif
