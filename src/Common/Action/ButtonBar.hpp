#ifndef BUTTONBAR_HPP_
#define BUTTONBAR_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "AActionsElement.hpp"

#include "../Action.Driver/AButtonDriver.hpp"

class ButtonBar: public AActionsElement
{
private:

    /*!
     * \brief Retourne le \ref Logger associé à la classe \ref ButtonBar.
     */
    static inline const logs::Logger & logger()
    {
        static const logs::Logger & instance = logs::LoggerFactory::logger("ButtonBar");
        return instance;
    }

    AButtonDriver* buttondriver_;

public:


    /*!
     * \brief Constructor.
     *
     */
    ButtonBar(Actions & actions);

    /*!
     * \brief Destructor.
     */
    ~ButtonBar();

    bool pressed(ButtonTouch button);

    //bool process(ButtonTouch button);

    bool waitPressed(ButtonTouch button);

    ButtonTouch waitOneOfAllPressed();
    ButtonTouch checkOneOfAllPressed();

};

#endif
