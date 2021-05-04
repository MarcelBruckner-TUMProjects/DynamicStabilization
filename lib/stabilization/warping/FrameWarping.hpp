//
// Created by brucknem on 13.01.21.
//

#ifndef CAMERASTABILIZATION_FRAMEWARPING_HPP
#define CAMERASTABILIZATION_FRAMEWARPING_HPP

#include "opencv2/opencv.hpp"
#include "../matching/FeatureMatchingBase.hpp"
#include "../TimeMeasurable.hpp"

namespace providentia {
	namespace stabilization {
		/**
		 * Base class for the frame warpers.
		 */
		class FrameWarping : public providentia::utils::TimeMeasurable {
		protected:

			/**
			 * The algorithm used to find the homography.
			 */
			int homographyCalculationAlgorithm = cv::LMEDS;

			/**
			 * The interpolation algorithm used to warp the frame.
			 */
			int perspectiveWarpFlags = cv::INTER_LINEAR;

			/**
			 * The found homography between the matched keypoints.
			 * Minimizer for the reprojection error between the writeFrames.
			 */
			cv::Mat homography;

			/**
			 * The current frame warpedFrame by the found homography and with minimal reprojection error to the reference frame.
			 */
			cv::cuda::GpuMat warpedFrame;

			/**
			 * The maximal value for the skew of the homographic transformation in the frame warping.
			 */
			double skewThreshold = 2e-4;

		public:

			/**
			 * @constructor
			 */
			explicit FrameWarping();

			/**
			 * @destructor
			 */
			~FrameWarping() override = default;

			/**
			 * @get The final frame warped by the found homography.
			 */
			const cv::cuda::GpuMat &getWarpedFrame() const;

			/**
			 * @get
			 */
			double getSkewThreshold() const;

			/**
			 * @get
			 */
			void setSkewThreshold(double skewThreshold);

			/**
			 * @get The homography that minimizes the reprojection error.
			 * 		If the skew of the homography exceeds the skew threshold the identity is returned.
			 */
			cv::Mat getHomography() const;

			/**
			 * Finds the homography that minimizes the reprojection error between the feature matches and warpes the frame.
			 * @param frame The frame to warp.
			 * @param matcher The matched features.
			 */
			void
			warp(const cv::cuda::GpuMat &frame,
				 const std::shared_ptr<providentia::stabilization::matching::FeatureMatchingBase> &matcher);

			/**
			 * Warps the given frame with the given homography.
			 *
			 * @param frame The frame to warp
			 * @param homography The homography to use.
			 * @param perspectiveWarpFlags The flags for the warp algorithm.
			 *
			 * @return The warped frame.
			 */
			static cv::cuda::GpuMat warp(const cv::cuda::GpuMat &frame, const cv::Mat &homography, int
			perspectiveWarpFlags = cv::INTER_LINEAR);
		};
	}
}

#endif //CAMERASTABILIZATION_FRAMEWARPING_HPP
