//
// Created by brucknem on 18.01.21.
//

#ifndef CAMERASTABILIZATION_TIMEMEASURABLE_HPP
#define CAMERASTABILIZATION_TIMEMEASURABLE_HPP

#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <utility>
#include <map>
#include <chrono>
#include <cstdarg>
#include <fstream>

namespace providentia {
	namespace utils {
		/**
		 * Class to record timestamps and measure durations of algorithm steps.
		 */
		class TimeMeasurable {
		private:
			/**
			 * The print name of the measurable.
			 */
			std::string name;

			/**
			 * The named timestamps of the measurements.
			 */
			std::vector<std::pair<std::string, std::chrono::milliseconds>> timestamps;

			/**
			 * The durations of the measurements.
			 */
			std::vector<std::pair<std::string, long>> durations;

			/**
			 * Buffer for the previous timestamp.
			 */
			std::chrono::milliseconds previous{};

			/**
			 * The minimum verbosity needed for a measurement to be added to the list.
			 * This enables different logging verbosities for different algorithms.
			 */
			int verbosity;

		public:
			/**
			 *  @Constructor
			 *
			 * @param name The print name of the instance.
			 */
			explicit TimeMeasurable(std::string name = "Unnamed", int verbosity = 1);

			/**
			 * @destructor
			 */
			virtual ~TimeMeasurable() = default;

			/**
			 * @set The print name of the instance.
			 */
			void setName(std::string name);

			/**
			 * @set The minimal verbosity needed to add a measurement.
			 */
			void setVerbosity(int verbosity);

			/**
			 * @set The name and verbosity.
			 */
			void setNameAndVerbosity(std::string name, int verbosity);

			/**
			 * Gets the current unix timestamp in milliseconds since 01.01.1970.

			 * @return The total milliseconds since 01.01.1970.
			 */
			static std::chrono::milliseconds now();

			/**
			 * Clears the buffers and adds a start timestamp.
			 */
			void clear();

			/**
			 * Adds a timestamp.
			 *
			 * @param measurementName The name of the measurement.
			 */
			void addTimestamp(const std::string &measurementName = "Unnamed", int minVerbosity = 0);

			/**
			 * Gets the total milliseconds duration from the last clear to the latest added timestamp.
			 *
			 * @return The total milliseconds.
			 */
			long getTotalMilliseconds();

			/**
			 * Converts the durations between the steps into a readable format.
			 *
			 * @return The formatted durations.
			 */
			std::string durationsToString();
		};
	}
}

#endif //CAMERASTABILIZATION_TIMEMEASURABLE_HPP
