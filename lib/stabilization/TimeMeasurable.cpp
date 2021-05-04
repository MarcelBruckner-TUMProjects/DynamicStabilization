//
// Created by brucknem on 18.01.21.
//

#include "TimeMeasurable.hpp"

namespace providentia {
	namespace utils {

		TimeMeasurable::TimeMeasurable(std::string name, int verbosity) : name(std::move(name)),
																		  verbosity(verbosity) {}

		void TimeMeasurable::setName(std::string value) {
			name = std::move(value);
		}

		void TimeMeasurable::setVerbosity(int value) {
			verbosity = value;
		}

		void TimeMeasurable::setNameAndVerbosity(std::string nameValue, int verbosityValue) {
			setName(std::move(nameValue));
			setVerbosity(verbosityValue);
		}

		std::chrono::milliseconds TimeMeasurable::now() {
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch());
		}

		void TimeMeasurable::clear() {
			timestamps.clear();
			durations.clear();
			previous = now();
			addTimestamp("Start");
		}

		void TimeMeasurable::addTimestamp(const std::string &measurementName, int minVerbosity) {
			if (verbosity < minVerbosity) {
				return;
			}
			auto now = TimeMeasurable::now();
			timestamps.emplace_back(measurementName, now);
			durations.emplace_back(std::make_pair(measurementName, (now - previous).count()));
			previous = TimeMeasurable::now();
		}

		long TimeMeasurable::getTotalMilliseconds() {
			return (timestamps[timestamps.size() - 1].second - timestamps[0].second).count();
		}

		std::string TimeMeasurable::durationsToString() {
			std::stringstream ss;

			for (const auto &duration : durations) {
				ss << "[" << name << "] " << duration.second << "ms : " << duration.first << std::endl;
			}

			return ss.str();
		}

	}
}