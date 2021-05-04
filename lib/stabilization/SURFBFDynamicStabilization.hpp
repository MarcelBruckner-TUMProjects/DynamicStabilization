//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_SURFBFDYNAMICSTABILIZATION_HPP
#define CAMERASTABILIZATION_SURFBFDYNAMICSTABILIZATION_HPP

#include "DynamicStabilizationBase.hpp"
#include "detection/SURFFeatureDetection.hpp"

namespace providentia {
	namespace stabilization {
/**
 * SURF feature detection and Brute Force feature matching stabilization algorithm.
 */
		class SURFBFDynamicStabilization : public providentia::stabilization::DynamicStabilizationBase {
		public:
			/**
			 * @constructor
			 */
			explicit SURFBFDynamicStabilization(
				providentia::stabilization::detection::SURFFeatureDetection::Options = {});

			/**
			 * @destructor
			 */
			~SURFBFDynamicStabilization() override = default;
		};

	}
}
#endif //CAMERASTABILIZATION_SURFBFDYNAMICSTABILIZATION_HPP
