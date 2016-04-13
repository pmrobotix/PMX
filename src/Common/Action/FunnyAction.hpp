#ifndef FUNNYACTION_HPP_
#define FUNNYACTION_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "AActionsElement.hpp"

class AServoUsingMotorDriver;


class FunnyAction: public AActionsElement
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref FunnyAction.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("FunnyAction");
		return instance;
	}

public:
	AServoUsingMotorDriver* servomotordriver;

	/*!
	 * \brief Constructor.
	 *
	 */
	FunnyAction(Actions & actions);

	/*!
	 * \brief Destructor.
	 */
	~FunnyAction();


	void reset();

	void activate(int pos);

	void release();


};

#endif
