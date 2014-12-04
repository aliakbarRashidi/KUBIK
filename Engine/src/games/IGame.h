#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "IDrawable.h"

using namespace std;
using namespace ci::signals;

class IGame:public IDrawable
{

public:	
	~IGame(){};
	virtual void draw() = 0;
	virtual void init() = 0;
	virtual void create() = 0;
	virtual void addMouseUpListener() = 0;
	virtual void removeMouseUpListener() = 0;
	signal<void(void)> closeGameSignal;
};