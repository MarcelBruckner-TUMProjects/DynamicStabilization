//
// Created by brucknem on 11.04.21.
//

#include "SURFFeatureDetection.hpp"

namespace providentia {
	namespace stabilization {
		namespace detection {

			SURFFeatureDetection::SURFFeatureDetection(Options options) {
				detector = cv::cuda::SURF_CUDA::create(options.hessianThreshold, options.nOctaves,
													   options.nOctaveLayers, options.extended,
													   options.keypointsRatio, options.upright);
				providentia::utils::TimeMeasurable::setName(typeid(*this).name());
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