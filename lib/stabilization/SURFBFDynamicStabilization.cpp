//
// Created by brucknem on 11.04.21.
//

#include "SURFBFDynamicStabilization.hpp"
#include "matching/BruteForceFeatureMatching.hpp"
#include "detection/SURFFeatureDetection.hpp"

namespace providentia {
	namespace stabilization {
		SURFBFDynamicStabilization::SURFBFDynamicStabilization(
			providentia::stabilization::detection::SURFFeatureDetection::Options options) {
			auto detector = providentia::stabilization::detection::SURFFeatureDetection(options);
			providentia::stabilization::DynamicStabilizationBase::frameFeatureDetector = std::make_shared<providentia::stabilization::detection::SURFFeatureDetection>(
				detector);
			providentia::stabilization::DynamicStabilizationBase::referenceFeatureDetector = std::make_shared<providentia::stabilization::detection::SURFFeatureDetection>(
				detector);
			providentia::stabilization::DynamicStabilizationBase::matcher = std::make_shared<providentia::stabilization::matching::BruteForceFeatureMatching>(
				cv::NORM_L2);
			providentia::utils::TimeMeasurable::setName(typeid(*this).name());
		}
	}
}