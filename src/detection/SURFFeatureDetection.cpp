//
// Created by brucknem on 11.04.21.
//

#include "DynamicStabilization/detection/SURFFeatureDetection.hpp"

namespace dynamic_stabilization {
	namespace stabilization {
		namespace detection {

			SURFFeatureDetection::SURFFeatureDetection(Options options) {
				detector = cv::cuda::SURF_CUDA::create(options.hessianThreshold, options.nOctaves,
													   options.nOctaveLayers, options.extended,
													   options.keypointsRatio, options.upright);
				dynamic_stabilization::utils::TimeMeasurable::setName(typeid(*this).name());
			}

			void SURFFeatureDetection::specificDetect() {
				detector->detectWithDescriptors(processedFrame, getCurrentMask(processedFrame.size()), keypointsGPU,
												descriptorsGPU, false);
				detector->downloadKeypoints(keypointsGPU, keypointsCPU);
				descriptorsGPU.download(descriptorsCPU);
			}
		}
	}
}