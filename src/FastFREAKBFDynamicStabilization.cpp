//
// Created by brucknem on 11.04.21.
//

#include "DynamicStabilization/FastFREAKBFDynamicStabilization.hpp"
#include "DynamicStabilization/matching/BruteForceFeatureMatching.hpp"
#include "DynamicStabilization/detection/FastFREAKFeatureDetection.hpp"

namespace dynamic_stabilization {
	namespace stabilization {
		FastFREAKBFDynamicStabilization::FastFREAKBFDynamicStabilization
			(dynamic_stabilization::stabilization::detection::FastFREAKFeatureDetection::Options options) {
			auto detector = dynamic_stabilization::stabilization::detection::FastFREAKFeatureDetection(options);
			frameFeatureDetector = std::make_shared<dynamic_stabilization::stabilization::detection::FastFREAKFeatureDetection>(
				detector);
			referenceFeatureDetector = std::make_shared<dynamic_stabilization::stabilization::detection::FastFREAKFeatureDetection>(
				detector);
			matcher = std::make_shared<dynamic_stabilization::stabilization::matching::BruteForceFeatureMatching>(
				cv::NORM_HAMMING);
			setName(typeid(*this).name());
		}
	}
}