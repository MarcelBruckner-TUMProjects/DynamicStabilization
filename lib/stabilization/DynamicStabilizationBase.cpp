#include "opencv2/imgproc/imgproc.hpp"

#include "DynamicStabilizationBase.hpp"
#include <opencv2/cudaarithm.hpp>

namespace providentia {
	namespace stabilization {

		void DynamicStabilizationBase::stabilize(const cv::cuda::GpuMat &frame) {
			clear();
			updateKeyframe();

			frameFeatureDetector->detect(frame, segmentor->getBackgroundMask(frame.size()));
			if (referenceFeatureDetector->isEmpty()) {
				// TODO copy/clone frameFeatureDetector
				referenceFeatureDetector->detect(frame, segmentor->getBackgroundMask(frame.size()));
			}

			matcher->match(frameFeatureDetector, referenceFeatureDetector);
			warper->warp(frame, matcher);
			segmentor->segment(getStabilizedFrame());
			addTimestamp("Stabilization finished", 0);
		}

		void DynamicStabilizationBase::updateKeyframe() {
			if (!shouldUpdateKeyframe || currentIteration++ < warmUp) {
				return;
			}

			int newCount = cv::cuda::countNonZero(segmentor->getBackgroundMask(getStabilizedFrame().size()));
			int oldCount = cv::cuda::countNonZero(referenceFeatureDetector->getCurrentMask());
			if (oldCount == getStabilizedFrame().size().width * getStabilizedFrame().size().height ||
				newCount > oldCount) {
				referenceFeatureDetector->detect(getStabilizedFrame(),
												 segmentor->getBackgroundMask(
														 getStabilizedFrame().size())
													 .clone());
			}
		}

		cv::Mat DynamicStabilizationBase::getHomography() const {
			return warper->getHomography();
		}

		const cv::cuda::GpuMat &DynamicStabilizationBase::getStabilizedFrame() const {
			return warper->getWarpedFrame();
		}

		const cv::cuda::GpuMat &DynamicStabilizationBase::getOriginalFrame() const {
			return frameFeatureDetector->getOriginalFrame();
		}

		DynamicStabilizationBase::DynamicStabilizationBase() : providentia::utils::TimeMeasurable(
			"DynamicStabilizationBase", 1) {
			warper = std::make_shared<FrameWarping>();
			segmentor = std::make_shared<providentia::stabilization::segmentation::MOG2BackgroundSegmentation>(
				cv::Size(1920, 1200) /
				10);
		}

		cv::Mat DynamicStabilizationBase::draw() {
			cv::Mat result;
			cv::hconcat(std::vector<cv::Mat>{cv::Mat(frameFeatureDetector->getOriginalFrame()),
											 cv::Mat(getStabilizedFrame())},
						result);
			return result;
		}

		const cv::cuda::GpuMat &DynamicStabilizationBase::getReferenceframe() const {
			return referenceFeatureDetector->getOriginalFrame();
		}

		const cv::cuda::GpuMat &DynamicStabilizationBase::getReferenceframeMask() const {
			return referenceFeatureDetector->getCurrentMask();
		}

		const std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> &
		DynamicStabilizationBase::getFrameFeatureDetector() const {
			return frameFeatureDetector;
		}

		const std::shared_ptr<providentia::stabilization::matching::FeatureMatchingBase> &
		DynamicStabilizationBase::getMatcher() const {
			return matcher;
		}

		const std::shared_ptr<providentia::stabilization::segmentation::BackgroundSegmentionBase> &
		DynamicStabilizationBase::getSegmentor() const {
			return segmentor;
		}

		cv::cuda::GpuMat DynamicStabilizationBase::getBackgroundMask(const cv::Size &size) const {
			return segmentor->getBackgroundMask(size);
		}

		const std::shared_ptr<FrameWarping> &
		DynamicStabilizationBase::getWarper() const {
			return warper;
		}

		bool DynamicStabilizationBase::isShouldUpdateKeyframe() const {
			return shouldUpdateKeyframe;
		}

		void DynamicStabilizationBase::setShouldUpdateKeyframe(bool value) {
			shouldUpdateKeyframe = value;
		}

		void DynamicStabilizationBase::setShouldUseFundamentalMatrix(bool shouldUseFundamentalMatrix) {
			matcher->setShouldUseFundamentalMatrix(shouldUseFundamentalMatrix);
		}

		void DynamicStabilizationBase::setSkewThreshold(double skewThreshold) {
			warper->setSkewThreshold(skewThreshold);
		}

	}// namespace stabilization
}// namespace providentia
