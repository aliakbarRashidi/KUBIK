#pragma once
#include "ConfigSettings.h"
#include "LoadButton.h"
#include "ImageQuadroButton.h"
#include "ChangeDesignButton.h"
#include "main/DesignBlock/ScreenSaverChecker.h"
#include "DecorLoadButton.h"
#include "PhotoboothSettings.h"

using namespace ci;

namespace kubik
{
	namespace config
	{
		class SettingsFactory
		{
		public:

			static SettingsFactory& getInstance() { 
				static SettingsFactory tt; 
				return tt; 
			};

			LoadButtonRef createLoadButton(const ci::Vec2f& pos)
			{
				return LoadButtonRef(
					new LoadButton(" ", ci::Rectf(pos, pos + Vec2f(200.0f, 70.0f)),
					settings->getTextItem(ConfigTextID::LOAD),					
					settings->getTexture("loadIcon")					
					));
			}

			LoadButtonRef createDecorLoadButton(const std::string &path, const ci::Vec2f& pos)
			{
				return DecorLoadButtonRef(
					new DecorLoadButton(path,
					ci::Rectf(pos, pos + Vec2f(200.0f, 70.0f)),					
					settings->getTextItem(ConfigTextID::LOAD),					
					settings->getTexture("loadIcon")					
					));
			}

			ScreenSaverCheckerRef createScreenSaverChecker(const ci::Vec2f& pos)
			{
				IconPair icons(settings->getTexture("ssCheckerOn"), settings->getTexture("ssCheckerOff"));
				ScreenSaverCheckerRef ch = ScreenSaverCheckerRef(new ScreenSaverChecker(Rectf(pos, pos + Vec2f(135, 83)), icons));				
				ch->setActive(true);
				return ch;
			}

			ImageQuadroButtonRef createImageQuadroButton(OneDesignItem item, const ci::Vec2f& pos)
			{				
				ImageQuadroButtonRef iq = ImageQuadroButtonRef(new ImageQuadroButton(item, pos));	
				return iq;				
			}

			ChangeDesignButtonRef createChangeDesignButton(OneDesignItem item, const ci::Vec2f& pos)
			{				
				ChangeDesignButtonRef iq = ChangeDesignButtonRef(new ChangeDesignButton(item, pos));	
				return iq;				
			}		

			ImageQuadroButtonRef createPhotoOverButton(OneDesignItem item, const ci::Vec2f& pos)
			{				
				return PhotoOverButtonRef(new PhotoOverButton(item, pos));						
			}

			ImageQuadroButtonRef createCardStyleButton(OneDesignItem item, const ci::Vec2f& pos)
			{				
				return PhotoCardStyleButtonRef(new PhotoCardStyleButton(item, pos));							
			}

			PhotoFilterPreviewButtonRef createPhotoFilterPreviewButton(OneDesignItem item, const ci::Vec2f& pos)
			{
				PhotoFilterPreviewButtonRef iq = PhotoFilterPreviewButtonRef(new PhotoFilterPreviewButton(item, pos));	
				return iq;	
			}

			void inject(ConfigSettingsRef configSettings)
			{
				settings = configSettings;
			}

			void inject(PhotoboothSettingsRef configSettings)
			{
				phtSettings = configSettings;
			}

		private:
			ConfigSettingsRef settings;
			PhotoboothSettingsRef phtSettings;
		};
	
		inline SettingsFactory&	settingsFactory() { return SettingsFactory::getInstance();};
	}
}
