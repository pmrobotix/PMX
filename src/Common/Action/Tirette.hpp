#ifndef TIRETTE_HPP_
#define TIRETTE_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "AActionsElement.hpp"

#include "../Interface.Driver/ASwitchDriver.hpp"

class Tirette: public AActionsElement
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref Tirette.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("Tirette");
        return instance;
    }

    ASwitchDriver* switchdriver_;

public:


    /*!
     * \brief Constructor.
     *
     */
    Tirette(Actions & actions);

    /*!
     * \brief Destructor.
     */
    ~Tirette();

    bool is_connected();

    int pressed();

    void monitor(int nb);

    bool waitPressed();

    bool waitUnpressed();

    void setGPIO(int p, bool activate);

};

#endif
