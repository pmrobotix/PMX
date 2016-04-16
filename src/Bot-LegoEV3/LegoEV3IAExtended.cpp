#include "LegoEV3IAExtended.hpp"


LegoEV3IAExtended::LegoEV3IAExtended(std::string botId, Asserv *asserv)
{
	iaz_.ia_setAsserv(asserv);

}

LegoEV3IAExtended::~LegoEV3IAExtended()
{
}
