
#ifndef AS_ADC__HPP
#define AS_ADC__HPP

#include <as_devices/as_adc.h>

struct as_adc_device;

/**
 * C++ wrapper for ADC device
 *
 * @ingroup cpp_wrappers
 */
class AsAdc
{
private:

public:

	static AsAdc & instance()
	{
		static AsAdc instance;
		return instance;
	}

	virtual ~AsAdc();

	/** @brief get value in millivolts
	 *
	 * @param aChannel	channel number
	 *
	 * @return value in millivolts ; error if negative millivolts
	 */
	long getValueInMillivolts(int aChannel) const;

protected:

	/** @brief Constructor: Initialize port access
	 *
	 * @param aDeviceNum device number
	 * @param aAdcType ADC type 0:max1027 1:as1531
	 * @param aVRef reference tension in millivolts
	 */
	AsAdc();
	/**
	 * ADC device C structure
	 */
	mutable struct as_adc_device *mDev; //
};

#endif // AS_ADC__HPP__
