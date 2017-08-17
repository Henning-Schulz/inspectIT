#pragma once

#include "SensorData.h";

namespace inspectit {
	namespace sending {

		class SensorDataWrapper
		{
		private:
			std::shared_ptr<inspectit::data::SensorData> data;

			size_t slot;
			bool finished = false;

		public:
			SensorDataWrapper();
			~SensorDataWrapper();

			std::shared_ptr<inspectit::data::SensorData> getData();
			void setData(std::shared_ptr<inspectit::data::SensorData> data);

			void setSlot(size_t slot);
			size_t getSlot();

			void finish();
			void reset();
			int isFinished();
		};

	}
}