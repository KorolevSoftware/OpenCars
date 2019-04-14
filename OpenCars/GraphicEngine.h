#pragma once
#include <Windows.h>
class GraphicEngine
{
public:
	virtual void createWindows(bool fullScreen, int width, int height) = 0;
	virtual void init() = 0;
	virtual void hellowTriangle() = 0;
	virtual void render() = 0;
};