#ifndef SIMU_ROBOTPOSITIONSHARED_HPP_
#define SIMU_ROBOTPOSITIONSHARED_HPP_

#include "../Common/Interface.Driver/ARobotPositionShared.hpp"
#include "../Log/LoggerFactory.hpp"


class RobotPositionShared: public ARobotPositionShared, utils::Mutex
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref RobotPositionShared(SIMU).
     */
    static inline const logs::Logger& logger()
    {
        static const logs::Logger &instance = logs::LoggerFactory::logger("RobotPositionShared.SIMU");
        return instance;
    }

    ROBOTPOSITION p_;

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
