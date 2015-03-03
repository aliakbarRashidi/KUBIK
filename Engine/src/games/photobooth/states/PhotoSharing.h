#pragma once

#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "model/PhotoStorage.h"

using namespace std;
using namespace ci::signals;
using namespace ci;
using namespace ci::app;

namespace kubik
{
	class PhotoSharing:public IPhotoboothLocation
	{
		ci::gl::Texture fon;
		ci::Font font;
		PhotoStorageRef  photoStorage;

		enum shareID
		{
			FACEBOOK,
			VKONTAKTE,
			TWITTER,
			EMAIL
		};

	//	vector<ShareButtonRef> shareBtns;
		
		public:

		PhotoSharing(PhotoboothSettingsRef settings, PhotoStorageRef  photoStorage):photoStorage(photoStorage)
		{			
			reset(settings);		
		};

		~PhotoSharing()
		{

		};

		void reset(PhotoboothSettingsRef _settings) override
		{
			settings = _settings;
			fon  = settings->getTexture("fon1");
			font = settings->getFont("helvetica40");

			//auto data = settings->getData();
			
		/*	shareBtns.clear();
			int i = 0;
			typedef PhotoboothSettings::SocialID SocialID;
			if (data.sharing.getSocialState(SocialID::FB))
				createShareButton(FACEBOOK, "FACEBOOK", i++);

			if (data.sharing.getSocialState(SocialID::VK))
				createShareButton(VKONTAKTE, "VKONTAKTE", i++);

			if (data.sharing.getSocialState(SocialID::TW))
				createShareButton(TWITTER, "TWITTER", i++);

			if (data.sharing.getSocialState(SocialID::EML))
				createShareButton(EMAIL, "EMAIL", i++);*/
		}

		void createShareButton(shareID id, const std::string& text, int i)
		{
			//ShareButtonRef button = ShareButtonRef(new ShareButton(id, getButtonArea(i), text, font));	
			//connect_once(button->mouseUpSignal, bind(&PhotoSharing::mouseUpListener, this, placeholders::_1));
			//shareBtns.push_back(button);
		}

		Rectf getButtonArea(int i) 
		{
			float btnSize = 300.0f;
			Vec2f pos  = Vec2f(btnSize * i, btnSize);		
			Vec2f size = Vec2f(300, 300); 	

			return Rectf(pos, pos + size);
		}

		//void mouseUpListener(ShareButton& button)
		//{
		//	nextLocationSignal();
		//}

		void start() override
		{
			console()<<"start PhotoSharing"<<endl;
		}

		void update() override
		{

		}

		void draw() override
		{
			gl::draw(fon, getWindowBounds());
			gl::color(Color::white());
			textTools().textFieldDraw("SHARING", &font, Color::white(), Vec2f(100.0f, 100.0f));

			//for (auto btn: shareBtns)
			//	btn->draw();
		}

		/*void mouseUpHandler(Vec2i vec) override
		{
			for (auto btn: shareBtns)		
				btn->mouseUpHandler(vec);
		}*/
	};

	typedef	shared_ptr<PhotoSharing> PhotoSharingRef;
}