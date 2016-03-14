//drivers...SIMULAUNCHER

#include "ContactDriver.hpp"


using namespace std;

AContactDriver * AContactDriver::create()
{
	static ContactDriver *instance = new ContactDriver();
	return instance;
}

ContactDriver::ContactDriver()
{

}

ContactDriver::~ContactDriver()
{
}

int ContactDriver::pressed()
{

 return 0;
}
