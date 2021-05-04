//
// Created by brucknem on 11.04.21.
//

#include "ORBFeatureDetection.hpp"

namespace providentia {
	namespace stabilization {
		namespace detection {

			ORBFeatureDetection::ORBFeatureDetection(Options options) {
				detector = cv::cuda::ORB::create(options.nfeatures, options.scaleFactor, options.nlevels,
												 options.edgeThreshold, options.firstLevel, options.wtaK,
												 options.scoreType, options.patchSize, options.fastThreshold,
												 options.blurForDescriptor);
				providentia::utils::TimeMeasurable::setName(typeid(*this).name());
			}

			void ORBFeatureDetection::specificDetect() {
				detector->detectAndComputeAsync(processedFrame, getCurrentMask(processedFrame.size()), keypointsGPU,
												descriptorsGPU, false);
				detector->convert(keypointsGPU, keypointsCPU);
				descriptorsGPU.download(descriptorsCPU);
			}
		}
	}
}