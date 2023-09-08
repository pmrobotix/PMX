#ifndef OPS6UL_ROBOTPOSITIONSHARED_HPP_
#define OPS6UL_ROBOTPOSITIONSHARED_HPP_

#include "../Common/Interface.Driver/ARobotPositionShared.hpp"
#include "../Log/LoggerFactory.hpp"

class RobotPositionShared: public ARobotPositionShared
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref RobotPositionShared(OPO).
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("RobotPositionShared.OPO");
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
    inline ~RobotPositionShared()
    {
    }

    ROBOTPOSITION getRobotPosition();

    void setRobotPosition(ROBOTPOSITION p);

};

#endif
