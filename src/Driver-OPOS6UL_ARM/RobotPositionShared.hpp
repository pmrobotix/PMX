#ifndef OPOS6UL_ROBOTPOSITIONSHARED_HPP_
#define OPOS6UL_ROBOTPOSITIONSHARED_HPP_

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
        static const logs::Logger &instance = logs::LoggerFactory::logger("RobotPositionShared.OPO");
        return instance;
    }

    ROBOTPOSITION p_;

    unsigned long long t_set_us_;

public:

    /*!
     * \brief Constructor.
     */
    RobotPositionShared();

    /*!
     * \brief Destructor.
     */
    ~RobotPositionShared();

    ROBOTPOSITION getRobotPosition(int debug=0);

    void setRobotPosition(ROBOTPOSITION p);


};

#endif
