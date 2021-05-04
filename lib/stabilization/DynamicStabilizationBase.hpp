#ifndef CAMERASTABILIZATION_DYNAMICSTABILIZATIONBASE_HPP
#define CAMERASTABILIZATION_DYNAMICSTABILIZATIONBASE_HPP

#include "segmentation/BackgroundSegmentionBase.hpp"
#include "segmentation/MOG2BackgroundSegmentation.hpp"
#include "detection/FeatureDetectionBase.hpp"
#include "matching/FeatureMatchingBase.hpp"
#include "warping/FrameWarping.hpp"
#include "TimeMeasurable.hpp"
#include "opencv2/opencv.hpp"

namespace providentia {
	namespace stabilization {

		/**
		 * Base class for the dynamic calibration algorithms.
		 */
		class DynamicStabilizationBase : public providentia::utils::TimeMeasurable {
		private:
			/**
			 * The warmup iterations before the keyframe may be updated.
			 */
			int warmUp = 50;

			/**
			 * The current iteration
			 */
			int currentIteration = 0;

			/**
			 * Flag if the keyframe should be updated.
			 */
			bool shouldUpdateKeyframe = false;

		protected:
			/**
			 * Feature detectors for the current frame and reference frame.
			 */
			std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> frameFeatureDetector,
				referenceFeatureDetector;

			/**
			 * Feature matcher to match the frame and reference frame stabilization::features.
			 */
			std::shared_ptr<providentia::stabilization::matching::FeatureMatchingBase> matcher;

			/**
			 * Warps the frame based on the given matches.
			 */
			std::shared_ptr<providentia::stabilization::FrameWarping> warper;

			/**
			 * Generates the foreground background masks.
			 */
			std::shared_ptr<providentia::stabilization::segmentation::BackgroundSegmentionBase> segmentor;

			/**
			 * @constructor
			 */
			DynamicStabilizationBase();

		public:
			/**
			 * @destructor
			 */
			~DynamicStabilizationBase() override = default;

			/**
			 * @get The warper used to align the frame with the reference frame.
			 */
			const std::shared_ptr<providentia::stabilization::FrameWarping> &getWarper() const;

			/**
			 * @get The background segmentor used to mask the writeFrames.
			 */
			const std::shared_ptr<providentia::stabilization::segmentation::BackgroundSegmentionBase> &
			getSegmentor() const;

			/**
			 * @get The feature detector for the current frame.
			 */
			const std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> &
			getFrameFeatureDetector() const;

			/**
			 * @get The matcher used for matching the frame and reference frame.
			 */
			const std::shared_ptr<providentia::stabilization::matching::FeatureMatchingBase> &getMatcher() const;

			/**
			 * @get The current frame.
			 */
			const cv::cuda::GpuMat &getOriginalFrame() const;

			/**
			 * @get The current frame stabilized with the found homography.
			 */
			const cv::cuda::GpuMat &getStabilizedFrame() const;

			/**
			 * @get The reference frame.
			 */
			const cv::cuda::GpuMat &getReferenceframe() const;

			/**
			 * @get The reference frame mask.
			 */
			const cv::cuda::GpuMat &getReferenceframeMask() const;

			/**
			 * @get The found homography minimizing the reprojection error between the frame and reference frame.
			 */
			cv::Mat getHomography() const;

			/**
			 * @get
			 */
			bool isShouldUpdateKeyframe() const;

			/**
			 * @set
			 */
			void setShouldUseFundamentalMatrix(bool shouldUseFundamentalMatrix);

			/**
			 * @set
			 */
			void setShouldUpdateKeyframe(bool shouldUpdateKeyframe);

			/**
			 * Main algorithm. <br>
			 * 1. Detects stabilization::features in the current frame and reference frame. <br>
			 * 2. Matches stabilization::features of the writeFrames. <br>
			 * 3. Finds homography by minimizing the reprojection error. <br>
			 * 4. Warps the frame using the found homography. <br>
			 *
			 * @param frame The frame to stabilize.
			 */
			void stabilize(const cv::cuda::GpuMat &frame);

			/**
			 * Draws the original and the stabilized frame aside.
			 */
			cv::Mat draw();

			/**
			 * Updates the keyframe.
			 */
			void updateKeyframe();

			/**
			 * Gets the current calculated background mask in the given size.
			 *
			 * @param size The size of the requested background mask.
			 * 		Necessary if the background estimation was not run previously and the mask is empty.
			 *
			 * @return The binary mask.
			 */
			cv::cuda::GpuMat getBackgroundMask(const cv::Size &size) const;

			/**
			 * @set the maximal value for the skew of the homographic transformation in the frame warping.
			 */
			void setSkewThreshold(double value);
		};

	}// namespace stabilization
}// namespace providentia
#endif//CAMERASTABILIZATION_DYNAMICSTABILIZATIONBASE_HPP
