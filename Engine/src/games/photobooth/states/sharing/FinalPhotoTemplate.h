#pragma once
#include "shaders/ShaderTool.h"
#include "model/PhotoStorage.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			class FinalPhotoTemplate
			{
				static const int MAX_PHOTOS = 3;				
				float photoTemplateScale, photoScale, templateWidth;
				float animTime;
				int index;

				ci::gl::Texture photoTemplate, photo;
				ci::Anim<float> _time;				
				shaders::imagefilters::BaseShaderRef shader;
				std::vector<PhotoTemplates> templates;

				void renderTexture();
				void chnagePhoto();

			public:	
				FinalPhotoTemplate();
				void setData(PhotoStorageRef photoStorage);

				void startAnimate();
				void stopAnimate();
				void setTemplate(const ci::gl::Texture& texture);
				void draw();
			};
		}
	}
}