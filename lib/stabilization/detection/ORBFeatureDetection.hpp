//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_ORBFEATUREDETECTION_HPP
#define CAMERASTABILIZATION_ORBFEATUREDETECTION_HPP

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
			class ORBFeatureDetection : public providentia::stabilization::detection::FeatureDetectionBase {
			private:
				/**
				 * The CUDA ORB detector used to detect keypoints and descriptors.
				 */
				cv::Ptr<cv::cuda::ORB> detector;

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
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					int nfeatures = 1e4;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					float scaleFactor = 1.2f;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					int nlevels = 8;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					int edgeThreshold = 31;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					int firstLevel = 0;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					int wtaK = 2;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					int scoreType = cv::ORB::FAST_SCORE;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					int patchSize = 31;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					int fastThreshold = 20;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::ORB::create
					 */
					bool blurForDescriptor = false;

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
				explicit ORBFeatureDetection(Options options = Options());

				/**
				 * @destructor
				 */
				~ORBFeatureDetection() override = default;
			};

		}
	}
}
#endif //CAMERASTABILIZATION_ORBFEATUREDETECTION_HPP
