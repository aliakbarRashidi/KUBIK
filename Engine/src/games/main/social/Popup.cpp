#include "main/social/Popup.h"

using namespace kubik::games;
using namespace kubik::config;
using namespace kubik;
using namespace std;
using namespace ci;
using namespace ci::app;

ci::gl::Texture Popup::backgroundImage;

Popup::Popup(ISettingsRef settings)
	:headColor(Color::hex(0x4b515a)),
	bgColor(Color::hex(0x34383f)), 
	closeIcon(settings->getTexture("closePopup")),
	inputFont(getFont("Intro-Book", 44))
{
	closeBtn = ImageButtonSpriteRef(new ImageButtonSprite(closeIcon, Vec2f(getWindowWidth(), 0.0f) + Vec2f(-93.0f, 93.0f) - closeIcon.getSize() * 0.5f));
}

void Popup::show()
{
	initVirtualKeyboard();

	timeline()
		.apply(&alphaAnim, 0.0f, 0.97f, 0.6f, EaseOutCubic())
		.finishFn(bind(&Popup::showAnimComplete, this));
}

void Popup::initVirtualKeyboard()
{
	touchKeyboard().activateSendMode();
	touchKeyboard().clearInputFieldText();
	touchKeyboard().setOriginPoint(Vec2f::zero());
	touchKeyboard().connectKeyboard();
	touchKeyboard().show(Vec2f(30.0f, 800.0f + 500.0f), Vec2f(30.0f, 800.0f), 0.7f);
}

void Popup::showAnimComplete()
{
	connectCloseBtn();
}

void Popup::kill()
{
	disconnectCloseBtn();
	disconnectAllFieldAsButton();
}

void Popup::connectCloseBtn()
{
	closeBtn->connectEventHandler(&Popup::hide, this);
}

void Popup::disconnectCloseBtn()
{
	closeBtn->disconnectEventHandler();
}

void Popup::connectAllFieldAsButton()
{
	connectEventHandler(&Popup::hideQuick, this);
}

void Popup::disconnectAllFieldAsButton()
{
	disconnectEventHandler();
}

void Popup::hideAnimComplete()
{
	callback(POPUP_CLOSED);
}

void Popup::hide(EventGUIRef& event)
{
	close();
}

void Popup::hideQuick(EventGUIRef& event)
{
	kill();

	touchKeyboard().disconnectKeyboard();
	touchKeyboard().hideQuick(Vec2f(30.0f, 1950.0f));
	callback(POPUP_CLOSED);
}

void Popup::close()
{
	kill();

	touchKeyboard().clearInputFieldText();
	touchKeyboard().disconnectKeyboard();
	touchKeyboard().hide(Vec2f(30.0f, 1950.0f), 0.3f);

	timeline()
		.apply(&alphaAnim, 0.0f, 0.6f, EaseOutCubic())
		.finishFn(bind(&Popup::hideAnimComplete, this));
}

void Popup::draw()
{
	closeBtn->setAlpha(alphaAnim);
	closeBtn->draw();

	touchKeyboard().draw();
	gl::color(Color::white());
}

void Popup::drawBackgrounds()
{
	auto color = Utils::colorAlpha(Color::white(), alphaAnim);
	gl::color(color);

	if (backgroundImage)
	{
		gl::draw(backgroundImage);
	}
}

void Popup::setBackground(const ci::gl::Texture& texture)
{
	backgroundImage = texture;
}
