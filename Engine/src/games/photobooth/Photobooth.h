#pragma once
#include "cinder/app/AppNative.h"
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "Graphics.h"
#include "IGame.h"
#include "Button.h"
#include "PhotoInstruction.h"
#include "PhotoFilter.h"
#include "PhotoTimer.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class Photobooth:public IGame
{
public:	
	
	Photobooth();
	~Photobooth();
	void draw();	
	void init();
	void create();

	void addMouseUpListener();
	void removeMouseUpListener();

private:	

	Texture closeImg;
	shared_ptr<Button> closeBtn;

	void setTextures();
	void mouseUp( MouseEvent &event);
	void mouseUpHandler(Button& button );

	connection mouseUpListener, closeBtnListener;	

	shared_ptr<PhotoInstruction> photoInstruction;
	shared_ptr<PhotoFilter>	photoFilter;
	shared_ptr<PhotoTimer>	photoTimer;

	list<shared_ptr<ILocation>>  locations;
//	ILocation* currentLocation;

	list<shared_ptr<ILocation>>::iterator currentLocation;

	void nextLocationHandler();
};