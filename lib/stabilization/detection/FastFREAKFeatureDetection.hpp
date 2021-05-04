//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_FASTFREAKFEATUREDETECTION_HPP
#define CAMERASTABILIZATION_FASTFREAKFEATUREDETECTION_HPP

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
			class FastFREAKFeatureDetection : public providentia::stabilization::detection::FeatureDetectionBase {
			private:
				/**
				 * The CUDA FastFeature detector used to detect keypoints and descriptors.
				 */
				cv::Ptr<cv::cuda::FastFeatureDetector> detector;
				cv::Ptr<cv::xfeatures2d::FREAK> descriptor;

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
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					int threshold = 40;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					bool nonmaxSuppression = true;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					cv::FastFeatureDetector::DetectorType type = cv::FastFeatureDetector::TYPE_9_16;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					int maxNPoints = 500000;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					bool orientationNormalized = true;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					bool scaleNormalized = true;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					float patternScale = 22.0f;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					int nOctaves = 4;

					/**
					 * @ref opencv2/cudafeatures2d.hpp -> cv::cuda::FastFeatures::create
					 */
					std::vector<int> selectedPairs = std::vector<int>();

					/**
					 * @constructor
					 */
					Options() {
						// Necessary for default initialization
					};
				};

				/**
				 * @constructor
				 *
				 * @param options The feature detector options
				 */
				explicit FastFREAKFeatureDetection(Options options = Options());

				/**
				 * @destructor
				 */
				~FastFREAKFeatureDetection() override = default;
			};

		}
	}
}
#endif //CAMERASTABILIZATION_FASTFREAKFEATUREDETECTION_HPP
