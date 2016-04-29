#ifndef BEACONDETECTORS_HPP_
#define BEACONDETECTORS_HPP_

#include "../../Log/LoggerFactory.hpp"
#include "AActionsElement.hpp"

class ASensorsDriver;


class BeaconDetector: public AActionsElement
{
private:

	/*!
	 * \brief Retourne le \ref Logger associé à la classe \ref BeaconDetector.
	 */
	static inline const logs::Logger & logger()
	{
		static const logs::Logger & instance = logs::LoggerFactory::logger("BeaconDetector");
		return instance;
	}

public:
	//ASensorsDriver* sensorsdriver;

	/*!
	 * \brief Constructor.
	 *
	 */
	BeaconDetector(Actions & actions);

	/*!
	 * \brief Destructor.
	 */
	~BeaconDetector();

	bool execute();

};

#endif
