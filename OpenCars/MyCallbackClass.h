#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <reactphysics3d.h>

class MyCallbackClass:public rp3d::RaycastCallback
{
public:
	float dist = 10000;
	virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info);
};

