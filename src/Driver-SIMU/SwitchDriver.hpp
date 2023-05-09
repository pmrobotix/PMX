#ifndef SIMU_SWITCHDRIVER_HPP_
#define SIMU_SWITCHDRIVER_HPP_

#include "../Common/Action.Driver/ASwitchDriver.hpp"
#include "../Log/LoggerFactory.hpp"

using namespace std;

class SwitchDriver: public ASwitchDriver
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref SwitchDriver(SIMU).
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("SwitchDriver.SIMU");
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
	~SwitchDriver();

	bool is_connected();

	int tirettePressed();
	int backLeftPressed();
	int backRightPressed();

	int pressed(unsigned char pin);

	void setGPIO(int gpio, bool activate);

	int state_;

};

#endif
