#pragma once

#include "SensorData.h"

#include <memory>

namespace inspectit {
	namespace sending {
		namespace buffer {

			class BufferStrategy
			{
			public:
				BufferStrategy() {}
				virtual ~BufferStrategy() {}

				virtual void addMeasurements(std::vector<std::shared_ptr<inspectit::data::SensorData>> data) = 0;
				virtual bool hasNext() = 0;
				virtual std::vector<std::shared_ptr<inspectit::data::SensorData>> next() = 0;
			};

		}
	}
}