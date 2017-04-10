/*!
 * \file
 * \brief Implémentation de la classe LcdShieldDriverTest.
 */

#include "LcdShieldDriverTest.hpp"

#include <unistd.h>
#include <cstdint>

void test::LcdShieldDriverTest::suite()
{
	this->test();

}

void test::LcdShieldDriverTest::test()
{
	lcdshielddriver->setBacklightOn();
	lcdshielddriver->clear();
	lcdshielddriver->home();
	print("PMX");
	sleep(2);
	lcdshielddriver->clear();
	lcdshielddriver->setBacklightOff();

	this->assert(true, "OK");
}

size_t test::LcdShieldDriverTest::print(const String &s)
{
	size_t n = 0;
	for (uint16_t i = 0; i < s.length(); i++)
	{
		n += lcdshielddriver->write(s[i]);
	}
	return n;
}
