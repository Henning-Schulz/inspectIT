#pragma once

#include "SensorDataWrapper.h"
#include "IntegerIterable.h"
#include "IntegerIterator.h"

#include <vector>
#include <atomic>
#include <mutex>
#include <memory>

namespace inspectit {
	namespace sending {

		/*
		* Should be used as:
		* 
		* int slot = ringBuffer.tryNext();
		* if (slot > 0) {
		*     wrapper = ringBuffer.get(slot);
		*     wrapper.setData(...);
		*     ringBuffer.publish(slot);
		* }
		*
		* ...
		*
		* ringBuffer.free(finishedSlot);
		*/
		class RingBuffer : public IntegerIterable
		{
		private:
			size_t bufferSize;
			std::vector<std::shared_ptr<inspectit::sending::SensorDataWrapper>> buffer;
			std::vector<std::shared_ptr<inspectit::sending::SensorDataWrapper>> finished;
			std::vector<std::atomic_flag> flags;

			std::mutex mutexDataAvailable;
			std::condition_variable cvDataAvailable;

			std::atomic<size_t> next = 0;

		public:
			RingBuffer();
			~RingBuffer();

			void initialize(size_t bufferSize);

			/*
			* Returns an index to be used or -1, if no one can be accessed.
			*/
			unsigned int tryNext();
			std::shared_ptr<inspectit::sending::SensorDataWrapper> get(unsigned int slot);
			void publish(unsigned int slot);

			void waitForData();
			void free(unsigned int slot);

			int getNextInteger(int startPosition);

			IntegerIterator iterator();
		};

	}
}