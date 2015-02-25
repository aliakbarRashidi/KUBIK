#include "IPhotoboothItem.h"

using namespace kubik;
using namespace kubik::config;

IPhotoboothItem::IPhotoboothItem(PhotoboothSettingsRef phbSettings, const PhtTextID& id, const ci::Color& color, int index)
	:Sprite(),
	settings(phbSettings),
	state(CLOSE),
	index(index),
	itemWidth(1080 - 166),
	closeHeightMax(384),
	closeHeightMin(199),
	openHeight(1124),
	color(color),
	closeMinY(50),
	closeMaxY(130),
	openY(135),
	offsetBetweenTitles(60),
	id(id)
{
	mainTitleY = closeMaxY;
	subTitleY  = closeMaxY + offsetBetweenTitles;
	animHeight = closeHeightMax;
	animatePosition = Vec2f(0.0f, closeHeightMax * index);

	setPosition(animatePosition.value());

	mainTitleButton = MainTitleButtonRef(new MainTitleButton(ci::Rectf(ci::Vec2f::zero(), ci::Vec2f(itemWidth, closeHeightMax)), index));				
	addChild(mainTitleButton);				

	setMainTextTitle(textTools().getTextField(phbSettings->getMainTitle(id)));
	setSubTextTitle(textTools().getTextField(phbSettings->getSubTitleClose(id), true));				
}

void IPhotoboothItem::setSubTextTitle(const ci::gl::Texture& tex)
{
	subText			= tex;
	subTextTexPosX  = 0.5f * (itemWidth - subText.getWidth());
}

void IPhotoboothItem::setMainTextTitle(const ci::gl::Texture& tex)
{
	mainTextTex		= tex;
	mainTextTexPosX	= 0.5f * (itemWidth - mainTextTex.getWidth());
}

void IPhotoboothItem::activateListeners()
{
	mainTitleButton->connectEventHandler(&IPhotoboothItem::mainTitleClicked, this);	
	Sprite::activateListeners();
}

void IPhotoboothItem::mainTitleClicked(EventGUIRef& event)
{

}

void IPhotoboothItem::unActivateListeners()
{
	mainTitleButton->disconnectEventHandler();
	Sprite::unActivateListeners();
}

void IPhotoboothItem::draw()
{	
	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	gl::color(color);
	gl::drawSolidRect(ci::Rectf( ci::Vec2f::zero(), ci::Vec2f(itemWidth, animHeight)));
	gl::color(ci::Color::white());
	gl::draw(mainTextTex, Vec2f(mainTextTexPosX, mainTitleY));
	gl::draw(subText, Vec2f(subTextTexPosX, subTitleY));			
	gl::popMatrices();	

	if (state == OPEN)	
		Sprite::draw();
}

void IPhotoboothItem::drawContent()
{
}

int IPhotoboothItem::getIndex() const
{
	return index;
}

void IPhotoboothItem::setOpenLayoutIndex(int openLayoutIndex)
{
	this->openLayoutIndex = openLayoutIndex;

	float time = 0.7f, height = 0.0f;
	ci::EaseFn eFunc = ci::EaseOutExpo();
	ci::Vec2f animateTo;			

	if(openLayoutIndex == index)
	{						
		openingLayout(eFunc, time);
		animateTo = ci::Vec2f(0.0f, openLayoutIndex * closeHeightMin);						
	}
	else if (openLayoutIndex == -1)
	{
		closingLayoutMaxState(eFunc, time);						
		animateTo = ci::Vec2f(0.0f, index * closeHeightMax);								
	}	
	else
	{
		closingLayoutMinState(eFunc, time);		
		if (openLayoutIndex > index)					
			animateTo =  ci::Vec2f(0.0f, index * closeHeightMin);					
		else
			animateTo =  ci::Vec2f(0.0f, (index - 1) * (closeHeightMin) + openHeight);														
	}

	timeline().apply( &animatePosition, animateTo, time, eFunc)
		.updateFn(bind( &IPhotoboothItem::posAnimationUpdate, this))
		.finishFn( bind( &IPhotoboothItem::animationFinish2, this));
}

void IPhotoboothItem::openingLayout(const ci::EaseFn& eFunc, float time)
{
	state = OPEN;	
	setSubTextTitle(textTools().getTextField(settings->getSubTitleOpen(id), true));		
	onOpenResetParams();
	timeline().apply( &mainTitleY, openY, time, eFunc);
	timeline().apply( &subTitleY,  openY + offsetBetweenTitles, time, eFunc);
	timeline().apply( &animHeight, openHeight + 2, time, eFunc);	
}

void IPhotoboothItem::closingLayoutMaxState(const ci::EaseFn& eFunc, float time)
{
	state = CLOSE;	
	setSubTextTitle(textTools().getTextField(settings->getSubTitleClose(id), true));		
	mainTitleButton->setButtonArea( ci::Rectf( ci::Vec2f::zero(), ci::Vec2f(itemWidth, closeHeightMax)));
	timeline().apply( &mainTitleY, closeMaxY, time, eFunc);
	timeline().apply( &subTitleY,  closeMaxY + offsetBetweenTitles, time, eFunc);
	timeline().apply( &animHeight, closeHeightMax + 2, time, eFunc);			
}

void IPhotoboothItem::closingLayoutMinState(const ci::EaseFn& eFunc, float time)
{
	state = CLOSE_MIN;	
	setSubTextTitle(textTools().getTextField(settings->getSubTitleClose(id), true));		
	mainTitleButton->setButtonArea(ci::Rectf( ci::Vec2f::zero(), ci::Vec2f(itemWidth, closeHeightMin)));
	timeline().apply( &mainTitleY, closeMinY, time, eFunc);
	timeline().apply( &subTitleY,  closeMinY + offsetBetweenTitles, time, eFunc);
	timeline().apply( &animHeight, closeHeightMin + 2, time, eFunc);
}

void IPhotoboothItem::posAnimationUpdate()
{
	setPosition(animatePosition.value());
}

void IPhotoboothItem::animationFinish2()
{				
	if(state == OPEN)
		mainTitleButton->disconnectEventHandler();
	else
		mainTitleButton->connectEventHandler(&IPhotoboothItem::mainTitleClicked, this);	
}

void IPhotoboothItem::onOpenResetParams()
{

}