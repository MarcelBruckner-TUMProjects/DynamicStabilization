//
// Created by brucknem on 11.04.21.
//

#include "DynamicStabilization/ORBBFDynamicStabilization.hpp"
#include "DynamicStabilization/matching/BruteForceFeatureMatching.hpp"
#include "DynamicStabilization/detection/ORBFeatureDetection.hpp"

namespace dynamic_stabilization {
	namespace stabilization {
		ORBBFDynamicStabilization::ORBBFDynamicStabilization(
			dynamic_stabilization::stabilization::detection::ORBFeatureDetection::Options options) {
			auto detector = dynamic_stabilization::stabilization::detection::ORBFeatureDetection(options);
			frameFeatureDetector = std::make_shared<dynamic_stabilization::stabilization::detection::ORBFeatureDetection>(
				detector);
			referenceFeatureDetector = std::make_shared<dynamic_stabilization::stabilization::detection::ORBFeatureDetection>(
				detector);
			matcher = std::make_shared<dynamic_stabilization::stabilization::matching::BruteForceFeatureMatching>(
				cv::NORM_HAMMING);
			setName(typeid(*this).name());
		}
	}
}