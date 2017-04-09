#ifndef OPOS6UL_SWITCHDRIVER_HPP_
#define OPOS6UL_SWITCHDRIVER_HPP_

#include "../Common/Action.Driver/ASwitchDriver.hpp"
#include "../Log/LoggerFactory.hpp"


using namespace std;


class SwitchDriver: public ASwitchDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SwitchDriver(OPOS6UL).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SwitchDriver.OPO");
		return instance;
	}

public:

	/*!
	 * \brief Constructor.
	 */
	SwitchDriver();

	/*!
	 * \brief Destructor.
	 */
	virtual ~SwitchDriver();

	virtual int pressed(); //tirette
	virtual int pressed(unsigned char pin);

};

#endif
