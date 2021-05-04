//
// Created by brucknem on 12.01.21.
//

#ifndef CAMERASTABILIZATION_FEATUREMATCHINGBASE_HPP
#define CAMERASTABILIZATION_FEATUREMATCHINGBASE_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/cudafeatures2d.hpp"
#include "../detection/FeatureDetectionBase.hpp"
#include "../TimeMeasurable.hpp"

namespace providentia {
	namespace stabilization {
		namespace matching {

			/**
			 * Base class for all feature matchers.
			 */
			class FeatureMatchingBase : public providentia::utils::TimeMeasurable {
			private:

				/**
				 * Filters the good matches by using the fundamental matrix.
				 */
				void filterUsingFundamentalMatrix();

			protected:

				/**
				 * The current frame feature detector applied in the main loop.
				 */
				std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> frameDetector, referenceFrameDetector;

				/**
				 * Flag if the fundamental matrix should be used.
				 */
				bool shouldUseFundamentalMatrix = true;

				/**
				 * Fundamental matrix inlier mask.
				 */
				cv::Mat fundamentalMatrixInlierMask;

				/**
				 * The fundamental matrix between the images.
				 */
				cv::Mat fundamentalMatrix;

				/**
				 * The k nearest neighbors of features.
				 */
				cv::cuda::GpuMat knnMatchesGPU;
				std::vector<std::vector<cv::DMatch>> knnMatchesCPU;

				/**
				 * A vector of good matches of features.
				 */
				std::vector<cv::DMatch> goodMatches;

				/**
				 * A vector of matches of features filtered using the fundamental matrix.
				 */
				std::vector<cv::DMatch> fundamentalMatches;

				/**
				 * The matched points of the frame and reference frame.
				 */
				std::vector<cv::Point2f> frameMatchedPoints, referenceMatchedPoints;

				/**
				 * The ratio threshold of good matches for the Lowe's ratio toCameraSpace.
				 */
				float goodMatchRatioThreshold;

				/**
				 * The frame with the matches drawn into.
				 */
				cv::Mat drawFrame;

				/**
				 * Specific implementation of the matching algorithm.
				 */
				virtual void specificMatch() = 0;

			public:
				/**
				 * @destructor
				 */
				~FeatureMatchingBase() override = default;

				/**
				 * @constructor
				 *
				 * @param goodMatchRatioThreshold The ratio threshold of good matches for the Lowe's ratio toCameraSpace.
				 */
				explicit FeatureMatchingBase(float goodMatchRatioThreshold = 0.75f);

				/**
				 * @get
				 */
				const std::vector<cv::DMatch> &getGoodMatches() const;

				/**
				 * @get
				 */
				const std::vector<cv::Point2f> &getReferenceMatchedPoints();

				/**
				 * @get
				 */
				const std::vector<cv::Point2f> &getFrameMatchedPoints() const;

				/**
				 * @set Flag for the matcher to filter the matches using the fundamental matrix.
				 */
				void setShouldUseFundamentalMatrix(bool shouldUseFundamentalMatrix);

				/**
				 * Matches the detected features of two writeFrames.
				 *
				 * @param frameFeatureDetector The feature detector of the frame.
				 * @param referenceFeatureDetector  The feature detector of the reference frame.
				 */
				void match(
					const std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> &frameFeatureDetector,
					const std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> &referenceFeatureDetector);

				/**
				 * Horizontally stacks the writeFrames and draws the matched features.
				 */
				cv::Mat draw();

			};

		}
	}
}

#endif //CAMERASTABILIZATION_FEATUREMATCHINGBASE_HPP
