//
// Created by brucknem on 11.04.21.
//

#include "DynamicStabilization/detection/SIFTFeatureDetection.hpp"

namespace dynamic_stabilization {
	namespace stabilization {
		namespace detection {

			SIFTFeatureDetection::SIFTFeatureDetection(Options options) {
				detector = cv::SIFT::create(options.nfeatures, options.nOctaveLayers, options.contrastThreshold,
											options.edgeThreshold, options.sigma);
				setName(typeid(*this).name());
			}

			void SIFTFeatureDetection::specificDetect() {
				detector->detectAndCompute(frameCPU, cv::Mat(getCurrentMask()), keypointsCPU, descriptorsCPU);
				descriptorsGPU.upload(descriptorsCPU);
			}
		}
	}
}