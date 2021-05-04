//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_SURFFEATUREDETECTION_HPP
#define CAMERASTABILIZATION_SURFFEATUREDETECTION_HPP

#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/cuda.hpp"
#include <opencv2/cudafeatures2d.hpp>

#include "../TimeMeasurable.hpp"
#include "FeatureDetectionBase.hpp"

namespace providentia {
	namespace stabilization {
		namespace detection {

/**
 * Wrapper for the CUDA SURF feature detector.
 */
			class SURFFeatureDetection : public providentia::stabilization::detection::FeatureDetectionBase {
			private:
				/**
				 * The CUDA SURF detector used to detect keypoints and descriptors.
				 */
				cv::Ptr<cv::cuda::SURF_CUDA> detector;

			protected:
				/**
				 * @copydoc
				 */
				void specificDetect() override;

			public:

				/**
				 * The feature detector options.
				 */
				struct Options {
					/**
					 * @ref opencv2/xfeatures2d/cuda.hpp -> cv::cuda::SURF_CUDA::create
					 */
					double hessianThreshold = 500;

					/**
					 * @ref opencv2/xfeatures2d/cuda.hpp -> cv::cuda::SURF_CUDA::create
					 */
					int nOctaves = 4;

					/**
					 * @ref opencv2/xfeatures2d/cuda.hpp -> cv::cuda::SURF_CUDA::create
					 */
					int nOctaveLayers = 2;

					/**
					 * @ref opencv2/xfeatures2d/cuda.hpp -> cv::cuda::SURF_CUDA::create
					 */
					bool extended = false;

					/**
					 * @ref opencv2/xfeatures2d/cuda.hpp -> cv::cuda::SURF_CUDA::create
					 */
					float keypointsRatio = 0.01f;

					/**
					 * @ref opencv2/xfeatures2d/cuda.hpp -> cv::cuda::SURF_CUDA::create
					 */
					bool upright = false;

					/**
					 * @constructor
					 */
					Options() {
						// Necessary for default initialization
					};
				};

				/**
				 * @constructor
				 */
				explicit SURFFeatureDetection(Options options = Options());

				/**
				 * @destructor
				 */
				~SURFFeatureDetection() override = default;

			};
		}
	}
}

#endif //CAMERASTABILIZATION_SURFFEATUREDETECTION_HPP
