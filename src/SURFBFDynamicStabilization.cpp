//
// Created by brucknem on 11.04.21.
//

#include "DynamicStabilization/SURFBFDynamicStabilization.hpp"
#include "DynamicStabilization/matching/BruteForceFeatureMatching.hpp"
#include "DynamicStabilization/detection/SURFFeatureDetection.hpp"

namespace dynamic_stabilization {
	namespace stabilization {
		SURFBFDynamicStabilization::SURFBFDynamicStabilization(
			dynamic_stabilization::stabilization::detection::SURFFeatureDetection::Options options) {
			auto detector = dynamic_stabilization::stabilization::detection::SURFFeatureDetection(options);
			dynamic_stabilization::stabilization::DynamicStabilizationBase::frameFeatureDetector = std::make_shared<dynamic_stabilization::stabilization::detection::SURFFeatureDetection>(
				detector);
			dynamic_stabilization::stabilization::DynamicStabilizationBase::referenceFeatureDetector = std::make_shared<dynamic_stabilization::stabilization::detection::SURFFeatureDetection>(
				detector);
			dynamic_stabilization::stabilization::DynamicStabilizationBase::matcher = std::make_shared<dynamic_stabilization::stabilization::matching::BruteForceFeatureMatching>(
				cv::NORM_L2);
			dynamic_stabilization::utils::TimeMeasurable::setName(typeid(*this).name());
		}
	}
}