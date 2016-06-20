#include "StrategyConfigJsonFactory.h"

#include "ListSizeStrategyConfig.h"
#include "TimeStrategyConfig.h"
#include "SimpleBufferStrategyConfig.h"
#include "SizeBufferStrategyConfig.h"



StrategyConfigJsonFactory::StrategyConfigJsonFactory()
{
}


StrategyConfigJsonFactory::~StrategyConfigJsonFactory()
{
}

std::shared_ptr<StrategyConfig> StrategyConfigJsonFactory::createFromJson(json::object jsonObject)
{
	utility::string_t className = jsonObject.at(L"className").as_string();

	if (className.compare(ListSizeStrategyConfig::CLASS_NAME) == 0) {
		std::shared_ptr<ListSizeStrategyConfig> config = std::make_shared<ListSizeStrategyConfig>();
		JAVA_INT listSize = jsonObject.at(L"listSize").as_number().to_int32();
		config->setListSize(listSize);
		return config;
	}
	else if (className.compare(TimeStrategyConfig::CLASS_NAME) == 0) {
		std::shared_ptr<TimeStrategyConfig> config = std::make_shared<TimeStrategyConfig>();
		JAVA_LONG time = jsonObject.at(L"time").as_number().to_int64();
		config->setTime(time);
		return config;
	}
	else if (className.compare(SimpleBufferStrategyConfig::CLASS_NAME) == 0) {
		std::shared_ptr<SimpleBufferStrategyConfig> config = std::make_shared<SimpleBufferStrategyConfig>();
		return config;
	}
	else if (className.compare(SizeBufferStrategyConfig::CLASS_NAME) == 0) {
		std::shared_ptr<SizeBufferStrategyConfig> config = std::make_shared<SizeBufferStrategyConfig>();
		JAVA_INT size = jsonObject.at(L"size").as_number().to_int32();
		config->setSize(size);
		return config;
	}
	else {
		return nullptr;
	}
}
