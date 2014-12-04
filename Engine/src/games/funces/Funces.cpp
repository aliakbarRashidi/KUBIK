#include "Funces.h"

Funces::Funces()
{	
	setTextures();	
}
Funces::~Funces()
{	
	console()<<"Funces destructor"<<endl;
	mouseUpListener.disconnect();
	closeBtnListener.disconnect();

	delete closeBtn;
	designTexures.clear();
}

void Funces::addMouseUpListener()
{
	mouseUpListener = getWindow()->connectMouseUp(&Funces::mouseUp, this);
}

void Funces::removeMouseUpListener()
{
	mouseUpListener.disconnect();
}

void Funces::setTextures()
{
	addToDictionary("img1", "gamesDesign\\funces\\1.jpg");
	addToDictionary("closeImg", "gamesDesign\\funces\\close.png");
	addToDictionary("img3", "gamesDesign\\funces\\3.jpg");
}

void Funces::create()
{
	closeImg = designTexures["closeImg"]->tex;
	console()<<"::funces createTextures::  "<<closeImg<<endl;
	closeBtn = new Button(closeImg, Vec2f(getWindowWidth() - 100, 100));		
	closeBtnListener = closeBtn->mouseUpSignal.connect(bind(&Funces::mouseUpHandler, this, std::placeholders::_1));
}

void Funces::init()
{

}

void Funces::mouseUp( MouseEvent &event)
{	
	closeBtn->mouseUpHandler(event.getPos());
}

void Funces::mouseUpHandler(Button& button )
{	
	console()<<"close event::"<<endl;
	closeGameSignal();
}

void Funces::draw()
{	
	closeBtn->draw();
}