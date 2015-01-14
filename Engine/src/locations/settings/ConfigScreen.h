#pragma once
#include "ApplicationModel.h"
#include "MenuButton.h"
#include "IScreen.h"
#include "ConfigSettings.h"
#include "MenuSettings.h"
#include "Button.h"
#include "GameSettings.h"
#include "ScreenSaverSettings.h"
#include "Types.h"

#include "elements/Title.h"
#include "elements/StartNewActivity.h"
#include "elements/StatBlock.h"
#include "elements/MenuBlock.h"
#include "elements/ScreenSaverBlock.h"
#include "elements/GamesBlock.h"
#include "elements/PrinterBlock.h"
#include "elements/Logo.h"
#include "elements/CloseBlock.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::signals;
using namespace params;
using namespace kubik::config;

namespace kubik
{
	class ConfigScreen:public IScreen
	{
	public:
		ConfigScreen(ISettingsRef config);
		~ConfigScreen();

		void start();
		void stop();
		void draw();	

		void init();
		void init(ISettingsRef settings) override;

		void reset() override{};
		void addMouseUpListener();
		void removeMouseUpListener();
		void startUpParams();
		void savePhtbtn();
		
		signal<void(void)> closeSettingsSignal;
		signal<void(vector<Changes>)> appSettingsChangedSignal;

		connection mouseUpListener;
		connection closeBtnListener;
		connection appSettingsChgListener;

		void setScreenSaverSettings(ScreenSaverSettingsRef screenSaverSettings);
		void setMenuSettings(MenuSettingsRef menuSettings);
		void setGameSettings(GameSettingsRef gameSettings);

	private:
		InterfaceGlRef	photoBoothParams;
		InterfaceGlRef	funcesParams;
		InterfaceGlRef	menuParams;
		InterfaceGlRef	gamesParams;
		InterfaceGlRef	screensaverParams;		

		vector<InterfaceGlRef> params;

		ConfigSettingsRef				configSettings;
		shared_ptr<ScreenSaverSettings> screenSaverSettings;
		shared_ptr<MenuSettings>		menuSettings;
		shared_ptr<GameSettings>		gameSettings;

		shared_ptr<ButtonText>	saveChngBtn;
		shared_ptr<Button>		closeBtn;
		Font font;

		vector<Changes> changes;

		PhotoboothSettings::PhotoboothDataStruct phData, initPhData;
		MenuSettings::MenuDataStruct menuData, initialMenuData;		
		GameSettings::GamesDataStruct gamesData, initialGamesData;	
		ScreenSaverSettings::ScreenSaverDataStruct screensaverData, initialScreensaverData;	

		void update();
		void mouseUp(MouseEvent &event);	
		void closeLocationHandler(IButton& button);
		void appSettingsChgHandler(ButtonText& button);		
		
		void savePhotoboothParams();

		void createPhotoboothParams();
		void createMenuParams();	
		void createFuncesParams();	

		void checkPhotoBoothParamsForChanges();
		void checkFuncesParamsForChanges();
		void checkMenuParamsForChanges();
		void checkGamesParamsForChanges();
		void checkScreenSaverParamsForChanges();
		void createGamesParams();		
		void createScreensaverParams();

		void showInExplorer(string path);
		void showInExplorerMenuDesignPath();
		
		void setDefaultGameIdInSwitchOnGames();
		void setReloadGamePropertyIfNeedIt(Changes &chng);

		Texture tempBg;
		CloseBlockRef		closeBlock;
		TitleRef			title;
		StartNewActivityRef startNewActivity;
		StatBlockRef		statBlock;
		MenuBlockRef		menuBlock;
		ScreenSaverBlockRef screenSaverBlock;
		GamesBlockRef		gamesBlock;
		PrinterBlockRef		printerBlock;
		LogoRef				logo;
		vector<IDrawableRef> components;
	};

	typedef shared_ptr<ConfigScreen> ConfigScreenRef;	
}