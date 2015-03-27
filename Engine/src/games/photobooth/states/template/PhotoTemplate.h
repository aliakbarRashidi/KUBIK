#pragma once
#include "states/IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "TextTools.h"
#include "TimerTools.h"
#include "cinder/gl/Texture.h"
#include "model/PhotoStorage.h"
#include "states/template/TemplateButtons.h"
#include "shaders/ShaderTool.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			typedef	shared_ptr<class PhotoTemplate> PhotoTemplateRef;

			class PhotoTemplate : public IPhotoboothLocation
			{
				enum locationState
				{
					ANIM_HIDE,
					TEMPLATE_CHOOSE
				}
				state;

				PhotoStorageRef photoStorage;

				std::vector<ci::gl::Texture> templates, stickers;
				std::vector<TemplateButtonRef> templatebtns;
				ci::Anim<float> alphaAnim;
				ci::Vec2f titlePos;

				void photoTemplateChoose(EventGUIRef& event);
				void setChoosingTemplate();
				void showAnimationComplete();
				void initShowAnim();
				void startHideAnimation();

				TemplateButtonRef selectedTemplate;

			public:
				PhotoTemplate(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);

				virtual void reset(PhotoboothSettingsRef settings) override;
				virtual void start() override;
				virtual void stop() override;
				virtual void update() override;
				virtual void draw() override;

				void resetTemplateButtons();
				virtual void stopAllTweens() override;
			};
		}
	}
}