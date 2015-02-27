#pragma once
#include "ApplicationModel.h"
#include "IScreen.h"
#include "ConfigSettings.h"
#include "MenuSettings.h"
#include "GameSettings.h"
#include "ScreenSaverSettings.h"
#include "Types.h"
#include "CloseConfigEvent.h"
#include "main/MainConfig.h"
#include "photobooth/PhotoboothConfig.h"
#include "InstakubConfig.h"
#include "gui/Sprite.h"
#include "GameSettingsSprite.h"

namespace kubik
{
	namespace config
	{
		class ConfigScreen:public IScreen, public Sprite
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
			void startUpParams();
			void savePhtbtn();

			SignalVoid closeSettingsSignal;
			signal<void(vector<Changes>)> appSettingsChangedSignal;

			connection mouseUpListener;
			connection closeBtnListener;
			connection appSettingsChgListener;

			void setScreenSaverSettings(ScreenSaverSettingsRef screenSaverSettings);
			void setMenuSettings(MenuSettingsRef menuSettings);
			void setGameSettings(GameSettingsRef gameSettings);

		private:
			ConfigSettingsRef		configSettings;
			ScreenSaverSettingsRef  screenSaverSettings;
			MenuSettingsRef			menuSettings;
			GameSettingsRef			gameSettings;		
			GameSettingsSpriteRef	gameSettingsScreen;

			std::vector<Changes> changes;

			//PhotoboothSettings::PhotoboothDataStruct phData, initPhData;
			MenuSettings::MenuDataStruct menuData, initialMenuData;		
			GameSettings::GamesDataStruct gamesData, initialGamesData;	
			ScreenSaverSettings::ScreenSaverDataStruct screensaverData, initialScreensaverData;	

			MainConfigRef mainConfig; 
			PhotoboothConfigRef photoboothConfig;
			InstakubConfigRef instakubConfig;

			void update();
			void closeLocationHandler(EventGUIRef& event);
			void gamesBlockHandler(EventGUIRef& event);

			//void appSettingsChgHandler(ButtonText& button);		

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

			void showingMainConfAnimationComplete();		
		};

		typedef shared_ptr<ConfigScreen> ConfigScreenRef;	
	}
}