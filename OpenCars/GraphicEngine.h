#pragma once
#include <Windows.h>
class GraphicEngine
{
public:
	virtual void createWindows(bool fullScreen, int width, int height) = 0;
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void initObjects() = 0;
};