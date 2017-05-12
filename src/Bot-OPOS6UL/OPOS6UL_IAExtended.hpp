#ifndef OPOS6UL_IAEXTENDED_HPP_
#define OPOS6UL_IAEXTENDED_HPP_

#include <string>

#include "../Common/IA/IAbyPath.hpp"
#include "../Common/IA/IAbyZone.hpp"

struct FoundPath;
struct Point;

class OPOS6UL_IAExtended
{
private:

	IAbyZone iaz_;
	IAbyPath iap_;

	Playground *p_;

public:
	OPOS6UL_IAExtended(std::string botId, Robot *robot);

	~OPOS6UL_IAExtended();

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
