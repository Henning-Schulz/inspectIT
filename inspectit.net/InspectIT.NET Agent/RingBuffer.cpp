#include "RingBuffer.h"

using namespace inspectit::sending;


RingBuffer::RingBuffer()
{
}


RingBuffer::~RingBuffer()
{
}

void inspectit::sending::RingBuffer::initialize(size_t bufferSize)
{
	this->bufferSize = bufferSize;

	buffer = std::vector<std::shared_ptr<SensorDataWrapper>>(bufferSize);
	finished = std::vector<std::shared_ptr<SensorDataWrapper>>(bufferSize);
	flags = std::vector<std::atomic_flag>(bufferSize);

	int slot = 0;
	for (auto it = buffer.begin(); it != buffer.end(); it++) {
		*it = std::make_shared<SensorDataWrapper>();
		(*it)->setSlot(slot);
		slot++;
	}
}

unsigned int RingBuffer::tryNext()
{
	unsigned int index = next++;
	if (!flags.at(index % bufferSize).test_and_set()) {
		return index;
	}
	else {
		// TODO: implement retry strategy
		return -1;
	}
}

std::shared_ptr<SensorDataWrapper> RingBuffer::get(unsigned int slot)
{
	return buffer.at(slot % bufferSize);
}

void RingBuffer::publish(unsigned int slot)
{
	get(slot)->finish();

	// notify waiting thread
	std::unique_lock<std::mutex> lock(mutexDataAvailable);
	cvDataAvailable.notify_all();
}

/*
* Must be called by one thread only!
*/
void inspectit::sending::RingBuffer::waitForData()
{
	std::unique_lock<std::mutex> lock(mutexDataAvailable);
	cvDataAvailable.wait(lock);
}

void inspectit::sending::RingBuffer::free(unsigned int slot)
{
	get(slot)->reset();
	flags.at(slot % bufferSize).clear();
}

int inspectit::sending::RingBuffer::getNextInteger(int startPosition)
{
	unsigned int i = startPosition;

	while (i % bufferSize != startPosition % bufferSize || i == startPosition) {
		if (get(i)->isFinished()) {
			return i;
		}

		i++;
	}

	return -1;
}

IntegerIterator inspectit::sending::RingBuffer::iterator()
{
	size_t start = next + bufferSize / 2;
	return IntegerIterator(this, start, start + bufferSize);
}

