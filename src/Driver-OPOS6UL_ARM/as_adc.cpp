/*
 **    THE ARMadeus Systems
 **
 **    Copyright (C) 2011  The armadeus systems team
 **    Jérémie Scheer <jeremie.scheeer@armadeus.com>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 **
 */

#include "as_adc.hpp"

#include <as_devices/as_adc.h>
#include <stddef.h>
#include <iostream>

AsAdc::AsAdc()
{
	const char *aAdcType = AS_MAX1027_NAME;
	int aDeviceNum = 0; //for APF9328 hwmon[aDeviceNum] => /sys/class/hwmon/hwmon0/device/
	int aVRef = 0;
	mDev = as_adc_open(aAdcType, aDeviceNum, aVRef);

	//std::cout << "AsAdc mDev=" << mDev << std::endl;
}

AsAdc::~AsAdc()
{
	int ret;

	if (mDev != NULL)
	{
		ret = as_adc_close(mDev);
		if (ret < 0)
		{
			std::cout << "AsAdc destruction error" << std::endl;
		}
	}
	else
	{
		std::cerr << "~AsAdc device structure not allocated" << std::endl;
	}
}

long AsAdc::getValueInMillivolts(int aChannel) const
{
	//std::cout << "getValueInMillivolts mDev=" << mDev << std::endl;
	if (mDev != NULL)
	{
		return as_adc_get_value_in_millivolts(mDev, aChannel);
	}
	else
	{
		std::cerr << "AsAdc getValueInMillivolts device structure not allocated" << std::endl;
		return -1;
	}
}
