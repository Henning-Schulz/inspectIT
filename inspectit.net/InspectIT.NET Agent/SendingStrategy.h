#pragma once

namespace inspectit {
	namespace sending {
		namespace strategy {

			class SendingStrategy
			{

			public:
				SendingStrategy() {}
				virtual ~SendingStrategy() {}

				virtual void start() = 0;
				virtual void stop() = 0;
			};

		}
	}
}