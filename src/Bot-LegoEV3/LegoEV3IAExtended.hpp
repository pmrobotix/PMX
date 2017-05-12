#ifndef LegoEV3_IAEXTENDED_HPP_
#define LegoEV3_IAEXTENDED_HPP_

#include <string>

#include "../Common/IA/IAbyPath.hpp"
#include "../Common/IA/IAbyZone.hpp"

struct FoundPath;
struct Point;

class LegoEV3IAExtended
{
private:

	IAbyZone iaz_;
	IAbyPath iap_;

	Playground *p_;

public:
	LegoEV3IAExtended(std::string botId, Robot *robot);

	~LegoEV3IAExtended();

	IAbyZone & iAbyZone()
	{
		return iaz_;
	}

	IAbyPath & iAbyPath()
	{
		return iap_;
	}

	void initPlayground();
	void playgroundComputeEdges();
	void playgroundFindPath(FoundPath * & path, Point& start, Point& end);

};

#endif
