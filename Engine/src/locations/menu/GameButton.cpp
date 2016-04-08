#include "GameButton.h"

using namespace kubik::menu;
using namespace kubik::config;
using namespace ci;

GameButton::GameButton(const GameData& data, AdditionalGameData adata)
	:SimpleSpriteButton(adata.getBackground().getWidth(), adata.getBackground().getHeight(), data.getPosition(), GameChoosedEventRef(new GameChoosedEvent(data.getID()))),
	iconTexture(data.getTexture()),
	iconPosition(adata.getIconPosition()),
	backgroundTexture(adata.getBackground()),
	backgroundPosition(adata.getBackgroundPosition()),	
	titleTexture(adata.getTitleByID(data.getID())),
	titlePosition(adata.getTitlePosition())
{	
	app::console() << "create button game id:::::::::::::::::::  " << data.getID() << std::endl;
}

void GameButton::drawLayout()
{	
	gl::pushMatrices();
	gl::translate(backgroundPosition);
	gl::draw(backgroundTexture);
	gl::popMatrices();

	gl::pushMatrices();
	gl::translate(iconPosition);
	gl::draw(iconTexture);
	gl::popMatrices();

	gl::pushMatrices();
	gl::translate((getWidth() - titleTexture.getWidth())*0.5f, titlePosition.y);
	gl::draw(titleTexture);
	gl::popMatrices();
}
