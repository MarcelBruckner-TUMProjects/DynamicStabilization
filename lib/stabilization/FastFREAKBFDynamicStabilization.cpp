//
// Created by brucknem on 11.04.21.
//

#include "FastFREAKBFDynamicStabilization.hpp"
#include "matching/BruteForceFeatureMatching.hpp"
#include "detection/FastFREAKFeatureDetection.hpp"

namespace providentia {
	namespace stabilization {
		FastFREAKBFDynamicStabilization::FastFREAKBFDynamicStabilization
			(providentia::stabilization::detection::FastFREAKFeatureDetection::Options options) {
			auto detector = providentia::stabilization::detection::FastFREAKFeatureDetection(options);
			frameFeatureDetector = std::make_shared<providentia::stabilization::detection::FastFREAKFeatureDetection>(
				detector);
			referenceFeatureDetector = std::make_shared<providentia::stabilization::detection::FastFREAKFeatureDetection>(
				detector);
			matcher = std::make_shared<providentia::stabilization::matching::BruteForceFeatureMatching>(
				cv::NORM_HAMMING);
			setName(typeid(*this).name());
		}
	}
}