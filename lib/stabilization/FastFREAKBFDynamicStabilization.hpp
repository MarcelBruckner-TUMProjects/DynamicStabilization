//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_FASTFREAKBFDYNAMICSTABILIZATION_HPP
#define CAMERASTABILIZATION_FASTFREAKBFDYNAMICSTABILIZATION_HPP

#include "DynamicStabilizationBase.hpp"
#include "detection/FastFREAKFeatureDetection.hpp"

namespace providentia {
	namespace stabilization {
/**
 * Dynamic stabilization with Fast feature detectors, FREAK feature descriptors and Brute Force matching.
 */
		class FastFREAKBFDynamicStabilization : public providentia::stabilization::DynamicStabilizationBase {
		public:
			/**
			 * @constructor
			 */
			explicit FastFREAKBFDynamicStabilization(
				providentia::stabilization::detection::FastFREAKFeatureDetection::Options options = {});

			/**
			 * @destructor
			 */
			~FastFREAKBFDynamicStabilization() override = default;
		};
	}
}

#endif //CAMERASTABILIZATION_FASTFREAKBFDYNAMICSTABILIZATION_HPP
