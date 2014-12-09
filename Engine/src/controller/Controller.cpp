#include "Controller.h"

using namespace kubik;
using namespace std;

Controller::Controller(ApplicationView* view)
{
	this->view  = view;
	game		= NULL;
}

void Controller::initLoad()
{
	preloader		= new Preloader();
	servicePopup	= new ServicePopup();

	view->startLocation(preloader);	

	try
	{
		model = new ApplicationModel();
		model->load();

		loadAllLocationsConfigs();
	}
	catch(ExcConfigFileParsing exc)
	{
		servicePopupShow(exc);
	}	
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATING CONFIGS
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadAllLocationsConfigs()
{
	try
	{
		gameSettings		 = new GameSettings(model);
		menuSettings		 = new MenuSettings(model);	
		screenSaverSettings	 = new ScreenSaverSettings(model);
		tuneUpSettings		 = new TuneUpSettings(model);	

		// ������� ���������

		loadGraphics();
	}
	catch(ExcConfigFileParsing exc)
	{
		servicePopupShow(exc);
	}
	catch(ExcBigFileSizeOfScreenSaver exc)
	{
		servicePopupShow(exc);
	}
}

////////////////////////////////////////////////////////////////////////////
//
//				LOADING GRAPHICS
//
////////////////////////////////////////////////////////////////////////////

void Controller::loadGraphics()
{
	graphicsLoader = new Graphics();

	connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::allGraphicsLoadingCompleteHandler, this));
	connect_once(graphicsLoader->errorLoadingSignal, bind(&Controller::graphicsLoadErrorHandler, this, std::placeholders::_1));

	// ->allSettingsTexture()
	graphicsLoader->setLoadingTextures(menuSettings->getTextures());
	graphicsLoader->setLoadingTextures(screenSaverSettings->getTextures());
	graphicsLoader->setLoadingTextures(tuneUpSettings->getTextures());
	graphicsLoader->setLoadingTextures(gameSettings->getActiveGameTextures());	
	graphicsLoader->load();
}

////////////////////////////////////////////////////////////////////////////
//
//				CREATING LOCATIONS
//
////////////////////////////////////////////////////////////////////////////

void Controller::allGraphicsLoadingCompleteHandler()
{
	console()<<"Graphics all Loaded:: "<<endl;
	removeGraphicsLoadingSignals();

	screenSaver = new ScreenSaver(screenSaverSettings);	
	menu        = new MenuScreen(menuSettings);
	settings    = new TuneUpScreen(tuneUpSettings, menuSettings, gameSettings);	// all settings pass

	// ������� ���

	// ������ ������ � ����

	try
	{
		createGame(model->getDefaultGameID());
		game->init(gameSettings);// ��������

		firstStart();		
	}
	catch(ExcGameDoesNotExist exc)
	{
		servicePopupShow(exc);
	}	
}

void Controller::firstStart()
{
	if(screenSaverSettings->isExist())
	{
		startScreenSaver();	
	}
	else
	{
		startMenuScreen();
	}	
}

void Controller::removeGraphicsLoadingSignals()
{
	graphicsLoader->completeLoadingSignal.disconnect_all_slots();	
	graphicsLoader->errorLoadingSignal.disconnect_all_slots();			
}

void Controller::graphicsLoadErrorHandler(KubikException exc)
{
	removeGraphicsLoadingSignals();
	servicePopupShow(exc);
}

////////////////////////////////////////////////////////////////////////////
//
//				SCREEN SAVER
//
////////////////////////////////////////////////////////////////////////////

void Controller::startScreenSaver()
{	
	console()<<"startScreenSaver::  "<<endl;		

	screenSaver->start();
	screenSaver->addMouseUpListener();
	connect_once(screenSaver->closeLocationSignal, bind(&Controller::closeScreenSaverHandler, this));
	view->startLocation(screenSaver);
}

void Controller::closeScreenSaverHandler()
{
	screenSaver->closeLocationSignal.disconnect_all_slots();
	screenSaver->removeMouseUpListener();
	screenSaver->stop();	
	startMenuScreen();
}

void Controller::startScreenSaverHandler()
{
	if(screenSaverSettings->isExist())
	{
		menu->removeMouseUpListener();	
		startScreenSaver();
	}
}

////////////////////////////////////////////////////////////////////////////
//
//					MENU SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::startMenuScreen()
{
	if(model->onlyOneGameOn())
	{
		resetGame();
		startGame();
	}
	else
	{
		startMultiplyGameMode();
	}
}

void Controller::startMultiplyGameMode()
{
	connect_once(menu->startGameSignal,		bind(&Controller::startGameHandler, this, std::placeholders::_1));
	connect_once(menu->startSettingsSignal, bind(&Controller::startSettingsHandler, this));
	connect_once(menu->startVideoSignal,	bind(&Controller::startScreenSaverHandler, this));		

	menu->addMouseUpListener();	
	view->startLocation(menu);
}

////////////////////////////////////////////////////////////////////////////
//
//					GAME SCREEN
//
////////////////////////////////////////////////////////////////////////////	

void Controller::startGameHandler(int gameId)
{
	console()<<"game id:: "<<gameId<<endl;
	menu->removeMouseUpListener();

	if(gameSettings->isGameCurrent(gameId))
	{
		resetGame();
		startGame();
	}
	else
	{
		try
		{
			createGame(gameId);

			view->startLocation(preloader);	
			connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::gameGraphicsLoadingCompleteHandler, this));
			connect_once(graphicsLoader->errorLoadingSignal, bind(&Controller::graphicsLoadErrorHandler, this, std::placeholders::_1));
			graphicsLoader->setLoadingTextures(gameSettings->getActiveGameTextures());	

			graphicsLoader->load();	
		}
		catch(ExcGameDoesNotExist exc)
		{
			servicePopupShow(exc);
		}
	}	
}		

void Controller::createGame(int gameId)
{
	console()<<"::create new game::"<<endl;	

	if(model->checkIfGameIdPurchased(gameId))
	{
		clearGameByID(gameSettings->getCurrentGame());
		gameSettings->setCurrentGame(gameId);
		game = new GameScreen(gameId);	
	}
	else		
		throw ExcGameDoesNotExist();
}

void Controller::closeGameHandler()
{
	console()<<"::::closeGameHandler fired::::"<<endl;
	game->removeMouseUpListener();
	menu->addMouseUpListener();	
	view->startLocation(menu);	
}

void Controller::clearGameByID(int id)
{
	if(game)
	{
		delete game;
		game = NULL;
	}
}

void Controller::gameGraphicsLoadingCompleteHandler()
{
	graphicsLoader->completeLoadingSignal.disconnect_all_slots();	
	graphicsLoader->errorLoadingSignal.disconnect_all_slots();

	game->init(gameSettings);
	startGame();	
}	

void Controller::resetGame()
{
	game->reset();
}

void Controller::startGame()
{	
	console()<<"START GAME::"<<endl;
	game->addMouseUpListener();
	view->startLocation(game);
	connect_once(game->closeLocationSignal,	bind(&Controller::closeGameHandler, this));
}

////////////////////////////////////////////////////////////////////////////
//
//					SETTINGS SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::startSettingsHandler()
{		
	menu->removeMouseUpListener();	
	startSettingsScreen();
}

void Controller::startSettingsScreen()
{
	console()<<"Start Settings Screen :::::"<<endl;
	connect_once(settings->closeLocationSignal, bind(&Controller::closeSettingsHandler, this));			
	connect_once(settings->appSettingsChangedSignal, bind(&Controller::appSettingsChangedHandler, this, std::placeholders::_1));			
	settings->addMouseUpListener();	
	settings->startUpParams();
	view->startLocation(settings);
}

void Controller::closeSettingsHandler()
{
	console()<<"Close Settings Screen :::::"<<endl;
	settings->closeLocationSignal.disconnect_all_slots();
	settings->appSettingsChangedSignal.disconnect_all_slots();
	settings->removeMouseUpListener();

	startMenuScreen();
}

void Controller::appSettingsChangedHandler(vector<int> changes)
{
	console()<<"App Settings Changed :::::"<<endl;
	settings->closeLocationSignal.disconnect_all_slots();
	settings->appSettingsChangedSignal.disconnect_all_slots();
	settings->removeMouseUpListener();

	reloadScreens(changes);	
}

void Controller::reloadScreens(vector<int> changes)
{
	this->changes = changes;
	bool needToReload = false;

	for(auto changedID : changes)
	{
		if(changedID == SettingTypes::MENU)
		{
			menu->reload();
			menuSettings->reload();
			graphicsLoader->setLoadingTextures(menuSettings->getTextures());
			view->startLocation(preloader);
			needToReload = true;
		}
		else if(gameSettings->isGameID(changedID) && gameSettings->isGameCurrent(changedID))
		{
			game->reload();
			gameSettings->reload();
			graphicsLoader->setLoadingTextures(gameSettings->getActiveGameTextures());
			view->startLocation(preloader);
			needToReload = true;
		}
	}

	console()<<"NEED TO RELOAD?::  "<<needToReload<<endl;

	if(needToReload)
	{
		connect_once(graphicsLoader->completeLoadingSignal, bind(&Controller::allGraphicsReloadCompleteHandler, this));
		connect_once(graphicsLoader->errorLoadingSignal, bind(&Controller::graphicsLoadErrorHandler, this, std::placeholders::_1));	
		graphicsLoader->load();
	}
	else
	{
		startMenuScreen();
	}
}

void Controller::allGraphicsReloadCompleteHandler()
{
	removeGraphicsLoadingSignals();

	for(auto changedID : changes)
	{
		if(changedID == SettingTypes::MENU)
		{
			menu->init(menuSettings);
		}
		else if(gameSettings->isGameID(changedID) && gameSettings->isGameCurrent(changedID))
		{			
			game->init(gameSettings);
		}
	}

	startMenuScreen();
}

////////////////////////////////////////////////////////////////////////////
//
//					SERVICE POPUP SCREEN
//
////////////////////////////////////////////////////////////////////////////

void Controller::servicePopupShow(KubikException exc)
{
	servicePopup->setMessage(exc.what());
	view->startLocation(servicePopup);
}