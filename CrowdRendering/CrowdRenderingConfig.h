#pragma once
#include "common/HiveConfig.h"

namespace hiveCrowdRendering
{
	namespace KEY_WORDS
	{
		const std::string COLOR_OF_AGENT = "COLOR_OF_AGENT";
		const std::string RADIUS_OF_AGENT = "RADIUS_OF_AGENT";
		const std::string COLOR_OF_OBSTACLE = "COLOR_OF_OBSTACLE";
		const std::string BG_COLOR_OF_SCENE = "BG_COLOR_OF_SCENE";
		const std::string WIDTH_OF_SCENE = "WIDTH_OF_SCENE";
		const std::string HEIGHT_OF_SCENE = "HEIGHT_OF_SCENE";
	}

	class CCrowdRenderingConfig : public hiveConfig::CHiveConfig, public hiveOO::CSingleton<CCrowdRenderingConfig>
	{
	public:
		~CCrowdRenderingConfig();

	private:
		CCrowdRenderingConfig();

		void __defineAttributes();

		friend class hiveOO::CSingleton<CCrowdRenderingConfig>;
	};
}