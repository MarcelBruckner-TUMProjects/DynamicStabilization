//
// Created by brucknem on 13.01.21.
//

#include <opencv2/cudawarping.hpp>
#include "DynamicStabilization/warping/FrameWarping.hpp"

namespace dynamic_stabilization {
	namespace stabilization {

		cv::cuda::GpuMat FrameWarping::warp(const cv::cuda::GpuMat &frame,
											const cv::Mat &homography, int perspectiveWarpFlags) {
			if (homography.empty()) {
				return frame;
			}
			cv::cuda::GpuMat result;
			cv::cuda::warpPerspective(frame, result, homography, frame.size(), perspectiveWarpFlags);
			return result;
		}

		void FrameWarping::warp(const cv::cuda::GpuMat &frame,
								const std::shared_ptr<dynamic_stabilization::stabilization::matching::FeatureMatchingBase> &matcher) {
			clear();
			if (matcher->getFrameMatchedPoints().size() < 4) {
				homography = cv::Mat::eye(3, 3, CV_64F);
			} else {
				homography = cv::findHomography(matcher->getFrameMatchedPoints(), matcher->getReferenceMatchedPoints(),
												homographyCalculationAlgorithm);
			}

			homography = getHomography();

			cv::Vec2d skew = {homography.at<double>(2, 0), homography.at<double>(2, 1)};
			if (cv::norm(skew) > skewThreshold) {
				homography = cv::Mat::eye(3, 3, CV_64F);
			}

			cv::cuda::warpPerspective(frame, warpedFrame, homography, frame.size(), perspectiveWarpFlags);
			addTimestamp("Frame warping finished", 0);
		}

		dynamic_stabilization::stabilization::FrameWarping::FrameWarping() : dynamic_stabilization::utils::TimeMeasurable("Frame Warper",
																									  1) {}

		const cv::cuda::GpuMat &dynamic_stabilization::stabilization::FrameWarping::getWarpedFrame() const {
			return warpedFrame;
		}

		cv::Mat dynamic_stabilization::stabilization::FrameWarping::getHomography() const {
			return homography;
		}

		double FrameWarping::getSkewThreshold() const {
			return skewThreshold;
		}

		void FrameWarping::setSkewThreshold(double value) {
			skewThreshold = value;
		}
	}
}
