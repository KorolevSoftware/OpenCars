#include "MyCallbackClass.h"

rp3d::decimal MyCallbackClass::notifyRaycastHit(const rp3d::RaycastInfo& info)
{
	std::cout << "Hit point : " << info.worldPoint.x <<" "<< info.worldPoint.y <<" " << info.worldPoint.z << std::endl;
	// Return a fraction of 1.0 to gather all hits return decimal(1.0);

	dist = std::min(dist, info.hitFraction);
	return 1.0;
}