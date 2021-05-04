//
// Created by brucknem on 11.04.21.
//

#include "FastFREAKFeatureDetection.hpp"

namespace providentia {
	namespace stabilization {
		namespace detection {
			FastFREAKFeatureDetection::FastFREAKFeatureDetection(Options options) {
				detector = cv::cuda::FastFeatureDetector::create(options.threshold, options.nonmaxSuppression,
																 options.type, options.maxNPoints);
				descriptor = cv::xfeatures2d::FREAK::create(options.orientationNormalized, options.scaleNormalized,
															options.patternScale, options.nOctaves,
															options.selectedPairs);
				providentia::utils::TimeMeasurable::setName(typeid(*this).name());
			}

			void FastFREAKFeatureDetection::specificDetect() {
				detector->detect(processedFrame, keypointsCPU, getCurrentMask(processedFrame.size()));
				descriptor->compute(frameCPU, keypointsCPU, descriptorsCPU);
				descriptorsGPU.upload(descriptorsCPU);
			}
		}
	}
}