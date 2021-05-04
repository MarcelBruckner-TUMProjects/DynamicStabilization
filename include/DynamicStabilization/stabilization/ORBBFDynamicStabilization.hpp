//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_ORBBFDYNAMICSTABILIZATION_HPP
#define CAMERASTABILIZATION_ORBBFDYNAMICSTABILIZATION_HPP

#include "DynamicStabilizationBase.hpp"
#include "detection/ORBFeatureDetection.hpp"

namespace dynamic_stabilization {
	namespace stabilization {
/**
 * ORB feature detection and Brute Force feature matching stabilization algorithm.
 */
		class ORBBFDynamicStabilization : public dynamic_stabilization::stabilization::DynamicStabilizationBase {
		public:
			/**
			 * @constructor
			 */
			explicit ORBBFDynamicStabilization(
				dynamic_stabilization::stabilization::detection::ORBFeatureDetection::Options options = {});

			/**
			 * @destructor
			 */
			~ORBBFDynamicStabilization() override = default;
		};
	}
}

#endif //CAMERASTABILIZATION_ORBBFDYNAMICSTABILIZATION_HPP
