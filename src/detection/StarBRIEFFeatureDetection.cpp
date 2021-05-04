//
// Created by brucknem on 11.04.21.
//

#include "DynamicStabilization/detection/StarBRIEFFeatureDetection.hpp"

namespace dynamic_stabilization {
	namespace stabilization {
		namespace detection {

			StarBRIEFFeatureDetection::StarBRIEFFeatureDetection(Options options) {
				detector = cv::xfeatures2d::StarDetector::create(options.maxSize, options.responseThreshold,
																 options.lineThresholdProjected,
																 options.lineThresholdBinarized,
																 options.suppressNonmaxSize);
				descriptor = cv::xfeatures2d::BriefDescriptorExtractor::create(options.bytes, options.useOrientation);
				dynamic_stabilization::utils::TimeMeasurable::setName(typeid(*this).name());
			}

			void StarBRIEFFeatureDetection::specificDetect() {
				detector->detect(FeatureDetectionBase::frameCPU, FeatureDetectionBase::keypointsCPU, cv::Mat(
					FeatureDetectionBase::getCurrentMask(FeatureDetectionBase::processedFrame.size())));
				descriptor->compute(FeatureDetectionBase::frameCPU, FeatureDetectionBase::keypointsCPU,
									FeatureDetectionBase::descriptorsCPU);
				FeatureDetectionBase::descriptorsGPU.upload(FeatureDetectionBase::descriptorsCPU);
			}
		}
	}
}