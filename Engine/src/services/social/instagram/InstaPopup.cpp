#include "instagram/InstaPopup.h"

using namespace instagram;

InstaPopup::InstaPopup(InstagramClientRef client, const gl::Texture& close, const gl::Texture& print, const gl::Texture& _template)
	:client(client),
	bgColor(Color::hex(0x060a0e)),
	alpha(0),
	showing(false),
	templateImage(_template)
{
	closeBtn = ImageButtonSpriteRef(new ImageButtonSprite(close, Vec2f(getWindowWidth(), 0.0f) + Vec2f(-93.0f, 93.0f) - close.getSize() * 0.5f));
	printBtn = ImageButtonSpriteRef(new ImageButtonSprite(print, Vec2f(0.5f * (getWindowWidth() - print.getWidth()), 10.0f)));
}

void InstaPopup::draw()
{
	using namespace ci;

	if (!showing)
		return;

	gl::pushMatrices();
	gl::translate(getGlobalPosition());
	gl::color(ColorA(bgColor.r, bgColor.g, bgColor.b, alpha));
	gl::drawSolidRect(getWindowBounds());
	gl::color(Color::white());

	ci::gl::Texture tex = image.getStandartResImage();
	
	if (tex)
	{
		float scale = ((float)tex.getWidth()) / templateImage.getWidth();
		auto shift = Vec2f(0.5f * (getWindowWidth() - tex.getWidth()), 224.0f);
		
		gl::pushMatrices();
		gl::translate(shift);
		gl::scale(scale, scale);
		gl::draw(templateImage);
		gl::popMatrices();
		gl::draw(tex, shift);
		
	}		

	closeBtn->draw();
	printBtn->draw();
	gl::popMatrices();
}

void InstaPopup::show(const ImageGraphic& image, ci::EaseFn eFunc, float time)
{
	this->image = image;
	showing = true;
	alpha = 0.0f;
	timeline().apply(&alpha, 0.95f, time, eFunc)
		.updateFn(bind(&InstaPopup::alphAnimationUpdate, this))
		.finishFn(bind(&InstaPopup::showAnimationFinish, this));
}

void InstaPopup::hide(ci::EaseFn eFunc, float time)
{
	showing = false;
	timeline().apply(&alpha, 0.0f, time, eFunc)
		.updateFn(bind(&InstaPopup::alphAnimationUpdate, this))
		.finishFn(bind(&InstaPopup::hideAnimationFinish, this));
}

void InstaPopup::alphAnimationUpdate()
{
	setAlpha(alpha.value());
}

void InstaPopup::showAnimationFinish()
{

}

void InstaPopup::hideAnimationFinish()
{
	//HideCompleteSignal();
}

void InstaPopup::setAlpha(float  alpha)
{
	//bgColor = Utils::colorAlpha(bgColor, alpha);
	//closeBlock->setAlpha(alpha);
	//CompositeDispatcher::setAlpha(alpha);
}

void InstaPopup::activateListeners()
{
	closeBtn->connectEventHandler(&InstaPopup::hiding, this);
	printBtn->connectEventHandler(&InstaPopup::printing, this);
}

void InstaPopup::unActivateListeners()
{
	closeBtn->disconnectEventHandler();
	printBtn->disconnectEventHandler();
}

void InstaPopup::hiding(EventGUIRef& event)
{
	callback(CLOSE_POPUP);
}

void InstaPopup::printing(EventGUIRef& event)
{
	callback(PRINT);	
}