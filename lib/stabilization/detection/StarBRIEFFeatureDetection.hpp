//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_STARBRIEFFEATUREDETECTION_HPP
#define CAMERASTABILIZATION_STARBRIEFFEATUREDETECTION_HPP

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
			class StarBRIEFFeatureDetection : public providentia::stabilization::detection::FeatureDetectionBase {
			private:
				/**
				 * The CUDA FastFeature detector used to detect keypoints and descriptors.
				 */
				cv::Ptr<cv::xfeatures2d::StarDetector> detector;
				cv::Ptr<cv::xfeatures2d::BriefDescriptorExtractor> descriptor;

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
					 * @ref opencv2/xfeatures2d.hpp -> cv::xfeatures2d::StarDetector::create
					 */
					int maxSize = 45;

					/**
					 * @ref opencv2/xfeatures2d.hpp -> cv::xfeatures2d::StarDetector::create
					 */
					int responseThreshold = 30;

					/**
					 * @ref opencv2/xfeatures2d.hpp -> cv::xfeatures2d::StarDetector::create
					 */
					int lineThresholdProjected = 10;

					/**
					 * @ref opencv2/xfeatures2d.hpp -> cv::xfeatures2d::StarDetector::create
					 */
					int lineThresholdBinarized = 8;

					/**
					 * @ref opencv2/xfeatures2d.hpp -> cv::xfeatures2d::StarDetector::create
					 */
					int suppressNonmaxSize = 5;

					/**
					 * @ref cv::xfeatures2d::BriefDescriptorExtractor::create
					 */
					int bytes = 64;

					/**
					 * @ref cv::xfeatures2d::BriefDescriptorExtractor::create
					 */
					bool useOrientation = false;

					/**
					 * @constructor
					 */
					Options() {
						// Necessary for default initialization
					}
				};

				/**
				 * @constructor
				 *
				 * @ref opencv2/xfeatures2d.hpp -> cv::cuda::FastFeatures::create
				 * @ref cv::xfeatures2d::BriefDescriptorExtractor::create
				 */
				explicit StarBRIEFFeatureDetection(Options options = {});

				/**
				 * @destructor
				 */
				~StarBRIEFFeatureDetection() override = default;

			};

		}
	}
}
#endif //CAMERASTABILIZATION_STARBRIEFFEATUREDETECTION_HPP
