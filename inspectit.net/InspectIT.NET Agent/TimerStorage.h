#pragma once
#include "MeasurementStorage.h"
#include "TimerStorageEntry.h"

#include <chrono>

namespace inspectit {
	namespace sensor {
		namespace timer {

			class TimerStorage :
				public inspectit::storage::MeasurementStorage
			{
			private:
				ThreadID threadId;

				int32_t started = false;

				std::vector<TimerStorageEntry> entries;
				std::vector<TimerStorageEntry> finishedEntries;

			public:
				TimerStorage(JAVA_LONG platformId, JAVA_LONG methodSensorId, ThreadID threadId, std::chrono::duration<long long, std::nano> timestamp);
				~TimerStorage();

				void newEntry(JAVA_LONG methodId, std::chrono::duration<long long, std::nano> startNanos);
				void finishCurrentEntry(std::chrono::duration<long long, std::nano> endNanos);

				bool finished();
				std::vector<std::shared_ptr<inspectit::data::SensorData>> finalizeData();
			};

		}
	}
}