#pragma once

#include "basehdr.h"

namespace inspectit {
	namespace sensor {

		class ThreadHook
		{
		public:
			ThreadHook() {}
			virtual ~ThreadHook() {}

			/*
			 * Is called, when a thread is created. The delayed flag is used to notify a thread hook
			 * of threads that have been created before the thread hook was initialized.
			 */
			virtual void threadCreated(ThreadID threadId) = 0;

			/*
			* Is called, when a thread is destroyed. The delayed flag is used to notify a thread hook
			* of threads that have been destroyed before the thread hook was initialized.
			*/
			virtual void threadDestroyed(ThreadID threadId) = 0;
		};

	}
}