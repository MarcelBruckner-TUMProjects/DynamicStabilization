//
// Created by brucknem on 11.04.21.
//

#include "opencv2/cudaarithm.hpp"
#include <utility>
#include <opencv2/cudawarping.hpp>
#include "BackgroundSegmentionBase.hpp"

namespace providentia {
	namespace stabilization {
		namespace segmentation {
			void BackgroundSegmentionBase::segment(const cv::cuda::GpuMat &frame) {
				clear();

				if (calculationSize.empty()) {
					calculationSize = frame.size();
				}

				if (calculationSize != frame.size()) {
					cv::cuda::resize(frame, calculationFrame, calculationSize);
				} else {
					calculationFrame = frame.clone();
				}

				specificApply();
				if (all255Mask.empty() || all255Mask.size() != foregroundMask.size()) {
					all255Mask.upload(cv::Mat::ones(foregroundMask.size(), CV_8UC1) * 255);
				}

				postProcess();
				cv::cuda::absdiff(all255Mask, foregroundMask, backgroundMask, stream);
				stream.waitForCompletion();
				addTimestamp("Background segmentation finished", 0);
			}

			void BackgroundSegmentionBase::postProcess() {
				for (const auto &filter : filters) {
					filter->apply(foregroundMask, foregroundMask, stream);
				}
			}

			const cv::cuda::GpuMat &
			BackgroundSegmentionBase::getForegroundMask(const cv::Size &size) {
				if (foregroundMask.empty() ||
					cv::cuda::countNonZero(foregroundMask) ==
					foregroundMask.size().width * foregroundMask.size().height) {
					clearMasks(size);
				}
				return foregroundMask;
			}

			const cv::cuda::GpuMat &
			BackgroundSegmentionBase::getBackgroundMask(const cv::Size &size) {
				if (backgroundMask.empty() ||
					cv::cuda::countNonZero(backgroundMask) == 0) {
					clearMasks(size);
				}
				if (!size.empty() && backgroundMask.size() != size) {
					cv::cuda::resize(backgroundMask, backgroundMask, size);
				}
				return backgroundMask;
			}

			cv::Mat BackgroundSegmentionBase::draw() const {
				cv::Mat result;
				cv::hconcat(std::vector<cv::Mat>{cv::Mat(backgroundMask), cv::Mat(foregroundMask)}, result);
				cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
				return result;
			}

			void BackgroundSegmentionBase::clearMasks(const cv::Size &size) {
				foregroundMask.upload(cv::Mat::ones(size, CV_8UC1) * 0);
				backgroundMask.upload(cv::Mat::ones(size, CV_8UC1) * 255);
			}

			BackgroundSegmentionBase::BackgroundSegmentionBase(cv::Size size)
				: TimeMeasurable(
				"BackgroundSegmentionBase", 1), calculationSize(std::move(size)) {
			}

			void BackgroundSegmentionBase::addFilters() {
				// Optional filters can be added by overriding this function.
			}
		}
	}
}