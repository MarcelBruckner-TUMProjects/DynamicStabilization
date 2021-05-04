//
// Created by brucknem on 11.04.21.
//

#include "FlannFeatureMatching.hpp"

namespace providentia {
	namespace stabilization {
		namespace matching {
			FlannFeatureMatching::FlannFeatureMatching(cv::flann::IndexParams *params,
													   float goodMatchRatioThreshold)
				: FeatureMatchingBase(
				goodMatchRatioThreshold) {
				matcher = std::make_shared<cv::FlannBasedMatcher>(params);
				setName(typeid(*this).name());
			}

			void FlannFeatureMatching::specificMatch() {
				if (frameDetector->getDescriptorsCPU().empty() || referenceFrameDetector->getDescriptorsCPU().empty()) {
					throw std::invalid_argument("Possibly match with wrong descriptor format called.");
				}
				matcher->knnMatch(frameDetector->getDescriptorsCPU(), referenceFrameDetector->getDescriptorsCPU(),
								  knnMatchesCPU,
								  2);
			}

			FlannFeatureMatching::FlannFeatureMatching(bool binaryDescriptors,
													   float goodMatchRatioThreshold) : FeatureMatchingBase(
				goodMatchRatioThreshold) {
				if (binaryDescriptors) {
					matcher = std::make_shared<cv::FlannBasedMatcher>(new cv::flann::LshIndexParams(12, 20, 2));
				} else {
					matcher = std::make_shared<cv::FlannBasedMatcher>();
				}
			}
		}
	}
}