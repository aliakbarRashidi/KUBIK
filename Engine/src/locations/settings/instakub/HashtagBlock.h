#pragma once
#include "gui/Sprite.h"
#include "TextTools.h"
#include "ConfigSettings.h"
#include "SettingsFactory.h"
#include "LoadButton.h"
#include "InstakubSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class HashTagBlock> HashTagBlockRef;

		class HashTagBlock: public Sprite
		{
		public:		
			HashTagBlock(InstakubSettingsRef settings, const ci::Vec2i& position);
			virtual void drawLayout();

		private:
			InstakubSettingsRef settings;	
			ci::gl::Texture titleTextTex, subTitleTextTex, hashTex;	
			ci::Vec2f titleTextPos, subTitleTextPos, searchfieldPos, hashPos;
			ci::gl::Texture searchfield;
		};
	}
}