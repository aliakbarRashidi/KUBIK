#pragma once

#include "ISettings.h"
#include "KubikException.h"
#include "FileTools.h"
#include "ConfigTexts.h"
#include "JsonTools.h"
#include "ApplicationModel.h"

namespace kubik
{
	namespace config
	{		
		typedef std::shared_ptr<class PhotoboothSettings> PhotoboothSettingsRef;
	
		class PhotoboothSettings: public ISettings
		{
		public:	
			enum PhtTextID
			{
				PHOTO_OVER,	PHOTO_OVER_SUB,
				CARD_STYLE,	CARD_STYLE_SUB,				
				FILTERS, FILTERS_SUB,
				PUBLISHING,	PUBLISHING_SUB,
				SAVE_TEXT, YOUR_DESIGN_TEXT,
				VKONTAKTE, FACEBOOK, TWITTER,
				QRCODE,	PRINTER,EMAIL, FILTER_TEXT1, FILTER_TEXT2,
				TIMER_TEXT1, TIMER_TEXT2,
				TEMPLATE_TEXT1, TEMPLATE_TEXT2, TEMPLATE_PRINT,
				PRELOAD_TEXT1, PRELOAD_TEXT2,
				CHOOSE_TEXT1, CHOOSE_TEXT2, PHOTO_FILTERS
			};

			class PhotoCountItem
			{
				gl::Texture texture;
				bool isActive;

			public:
				bool getActive()
				{
					return isActive;
				}
			};

			PhotoboothSettings(ApplicationModelRef model);

			void buildData()    override;
			void load()			override;
			void setTextures()  override;			

			TextItem getMainTitle(PhtTextID id);
			TextItem getSubTitleClose(PhtTextID id);
			TextItem getSubTitleOpen(PhtTextID id);
			TextItem getTextItem(PhtTextID id);
			bool	 getSocialState(PhtTextID id);
			void	 setSocialState(PhtTextID id, bool value);

			ci::gl::Texture getIcon(PhtTextID id);
			ci::gl::Texture getEmptyIcon();

			std::vector<ci::gl::Texture> getPhotoCardStylesActiveTemplate();
			std::vector<ci::gl::Texture> getPhotoOverActiveTemplate();

			ci::gl::Texture getActivePrintBgTex();
			std::vector<int> getOnFilters();
			void swapFilter(int id);
			
			//bool findFilterId(int id, std::vector<int> filters);			

			DesignData getPhotoOverDesignData()
			{
				return photoOverDesignData;
			}
			DesignData getPhotoCardStyles()
			{
				return photoCardStyles;
			}

			DesignData getPhotoFiltersPreview()
			{
				return photoFiltersPreview;
			}	
			
			void setActiveOverDesignID(int id);
			int getActiveOverDesignID();

			int getUserOverDesignID();

			int getActivePhotoCardStyleDesignID();
			void setActivePhotoCardStyleDesignID(int id);

			int getUserPhotoCardStyleDesignID();
			int getCurrentPhotoCount();

			std::string getUserPhotoOverDesignPath();
			std::string getUserPhotoCardStylePath();

			void createMemento();
			void writeConfig();

			int getBeReadySeconds(){return seconds;};

			bool wasChanged(){ return false;};
			bool settingsChanged();	
			changeSetting::id getChangeID(){ return changeSetting::id::PHOTOBOOTH;};	

		private:
			class Filter
			{
				int id;
				bool isOn;

			public:
				friend PhotoboothSettings;

				bool isActive()
				{
					return isOn;
				}

				int getID()
				{
					return id;
				}
			};

		public:
			std::vector<Filter> getFilters()
			{
				return filters;
			}
		private:

			class ImageElement
			{
				std::string path;
				ci::gl::Texture tex;
				int id;

			public:
				friend PhotoboothSettings;
			};	

			typedef ImageElement Sticker;
			typedef ImageElement BackgroundPrint;

			class ConfigPath
			{
				std::string staticPartDesignPath;
				std::string kubikTemplatePartDesignPath;
				std::string userTemplatePartDesignPath;
				std::string finalPath;
				std::string userStickerPath;		
				std::string userCardStylePath;
				std::string photoOverDesignDataPath;
				std::string photoCardsStylesDesignDataPath;
				std::string photoFiltersPreviewDesignDataPath;

			public:
				friend PhotoboothSettings;
			};		

			class Sharing
			{
			public:
				void setSocialState(PhtTextID id, bool state);
				void setIcon(ci::gl::Texture icon, PhtTextID id);
				void setEmptyIcon(ci::gl::Texture icon);

				bool getSocialState(PhtTextID id);
				ci::gl::Texture getEmptyIcon();
				ci::gl::Texture getIcon(PhtTextID id);

				friend PhotoboothSettings;

			private:
				std::map <PhtTextID, bool> states;
				std::map <PhtTextID, ci::gl::Texture> icons;
				ci::gl::Texture emptyIcon;
			};

			ConfigObject mainConfigObj;

			int seconds;
			int secondsBetweenShots;			
			int templateId;			
			int minPhotosShots;
			int maxPhotosShots;		
			int minSecBetweenShots;
			int maxSecBetweenShots;			
			int minCountTimer;
			int maxCountTimer;
			bool isCustomDesign;
			bool isSticker;

			int activeOverDesignID, activeOverDesignIDMemento;
			int userOverDesignID;

			int activePhotoCardStyleDesignID, activePhotoCardStyleDesignIDMemento;
			int userPhotoCardStyleDesignID;

			vector<ci::RectT<int>> photoCardStylesCoordRects, photoOverCoordRects;
	
			ConfigPath					 configPaths;
			Sharing						 sharing, sharingMemento;

			bool						 memento;

			std::vector<Filter>			 filters, filtersMemento;		
			std::vector<Sticker>		 stickers;
			std::vector<BackgroundPrint> bgPrint;
			ConfigTexts<PhtTextID>		 configTexts;
			BackgroundPrint				 activeBgPrint;
			Sticker						 activeSticker;

			void loadPaths();
			void loadParams();
			void loadLabels();			
			void loadConsts();			
			void loadDesignPath();
			
			void parsePhotoOverDesigns();
			void parsePhotoCardStyles();
			void parsePhotoFiltersPreview();

			void loadPhotoParams(JsonTree config);
			void loadSocialParams(JsonTree config);
			void loadPhotoFilterParams(JsonTree config);
			void loadGameDesignParams(JsonTree config);
			void loadGameStickerParams(JsonTree config);
			//void loadGameBgPrintParams(JsonTree config);
			void loadConfigTexts(JsonTree config);
			void loadSharingIcons(JsonTree config);
			void saveConfig();		
			void findAllImagePrints(std::string path, std::vector<ImageElement> &prints, bool isCustom);	

			/////////////////////////////////

			DesignData photoOverDesignData;		
			DesignData photoCardStyles;
			DesignData photoFiltersPreview;

			std::string getActiveOverDesignText();
			std::string getActiveCardStyleText();		
			std::string getActiveFiltersTexts();		
			std::string getActivePublishingTexts();		
					
			bool sharingNotEqual(Sharing sharing1, Sharing sharing2);
			bool filtersNotEqual(const std::vector<Filter>& filter1, const std::vector<Filter>& filter2);
		};	

		typedef PhotoboothSettings::PhtTextID  PhtTextID;
	}
 }