#include "CrowdRenderingConfig.h"

using namespace hiveCrowdRendering;

CCrowdRenderingConfig::CCrowdRenderingConfig()
{
	__defineAttributes();
}


CCrowdRenderingConfig::~CCrowdRenderingConfig()
{
}

void CCrowdRenderingConfig::__defineAttributes()
{
	defineAttribute(KEY_WORDS::RADIUS_OF_AGENT, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(KEY_WORDS::WIDTH_OF_SCENE, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(KEY_WORDS::HEIGHT_OF_SCENE, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(KEY_WORDS::COLOR_OF_AGENT, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(KEY_WORDS::COLOR_OF_OBSTACLE, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(KEY_WORDS::BG_COLOR_OF_SCENE, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(KEY_WORDS::SCALE, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(KEY_WORDS::OUTPUT_DIR, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(KEY_WORDS::EXIT, hiveConfig::ATTRIBUTE_STRING);
	defineAttribute(KEY_WORDS::EXIT_NUM, hiveConfig::ATTRIBUTE_INT);
	defineAttribute(KEY_WORDS::DISPLAY, hiveConfig::ATTRIBUTE_BOOL);
	defineAttribute(KEY_WORDS::SAVE, hiveConfig::ATTRIBUTE_BOOL);
}
