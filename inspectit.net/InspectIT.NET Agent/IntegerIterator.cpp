#include "IntegerIterator.h"

using namespace inspectit::sending;

IntegerIterator::IntegerIterator(IntegerIterable *iterated, int start, int max)
{
	this->iterated = iterated;
	this->max = max;
	this->current = -1;
	this->nextStart = start;
	this->hasNextCalled = false;
}


IntegerIterator::~IntegerIterator()
{
}

bool inspectit::sending::IntegerIterator::hasNext()
{
	hasNextCalled = true;

	if (nextStart > max) {
		current = -1;
		return false;
	}
	else {
		current = iterated->getNextInteger(nextStart);
		if (current > max) {
			return false;
		}
		else {
			return current >= 0;
		}
	}
}

int inspectit::sending::IntegerIterator::next()
{
	if (!hasNextCalled) {
		hasNext();
	}

	nextStart = current + 1;
	return current;
}
