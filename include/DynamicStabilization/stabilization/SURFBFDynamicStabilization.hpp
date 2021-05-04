//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_SURFBFDYNAMICSTABILIZATION_HPP
#define CAMERASTABILIZATION_SURFBFDYNAMICSTABILIZATION_HPP

#include "DynamicStabilizationBase.hpp"
#include "detection/SURFFeatureDetection.hpp"

namespace dynamic_stabilization {
	namespace stabilization {
/**
 * SURF feature detection and Brute Force feature matching stabilization algorithm.
 */
		class SURFBFDynamicStabilization : public dynamic_stabilization::stabilization::DynamicStabilizationBase {
		public:
			/**
			 * @constructor
			 */
			explicit SURFBFDynamicStabilization(
				dynamic_stabilization::stabilization::detection::SURFFeatureDetection::Options = {});

			/**
			 * @destructor
			 */
			~SURFBFDynamicStabilization() override = default;
		};

	}
}
#endif //CAMERASTABILIZATION_SURFBFDYNAMICSTABILIZATION_HPP
