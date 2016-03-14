
#ifndef BUTTONBAR_HPP_
#define BUTTONBAR_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "AActionsElement.hpp"

#include "../Action.Driver/AButtonDriver.hpp"

#include "../Utils/Macro.hpp"



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


public:
	AButtonDriver* buttondriver;

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

	bool waitPressed(ButtonTouch button);

};



#endif
