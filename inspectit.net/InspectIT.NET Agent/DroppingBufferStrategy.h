#pragma once
#include "BufferStrategy.h"

namespace inspectit {
	namespace sending {
		namespace buffer {

			class DroppingBufferStrategy :
				public BufferStrategy
			{
			public:
				DroppingBufferStrategy();
				~DroppingBufferStrategy();

				void addMeasurements(std::vector<std::shared_ptr<inspectit::data::SensorData>> data);
				bool hasNext();
				std::vector<std::shared_ptr<inspectit::data::SensorData>> next();
			};

		}
	}
}