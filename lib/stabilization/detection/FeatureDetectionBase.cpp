//
// Created by brucknem on 12.01.21.
//
#include <opencv2/cudaimgproc.hpp>
#include "opencv2/features2d.hpp"
#include <utility>
#include "FeatureDetectionBase.hpp"

namespace providentia {
	namespace stabilization {
		namespace detection {

			void FeatureDetectionBase::setCurrentMask(cv::Size size) {
				cv::Size sizeIntern = std::move(size);
				if (sizeIntern.empty()) {
					sizeIntern = processedFrame.size();
				}
				if (useLatestMaskFlag) {
					if (latestMask.empty()) {
						latestMask.upload(cv::Mat::ones(sizeIntern, CV_8UC1) * 255);
					}
					currentMask = latestMask;
				} else {
					if (noMask.empty() || noMask.size() != sizeIntern) {
						noMask.upload(cv::Mat::ones(sizeIntern, CV_8UC1) * 255);
					}
					currentMask = noMask;
				}
			}

			void FeatureDetectionBase::detect(const cv::cuda::GpuMat &frame) {
				clear();
				originalFrame = frame.clone();
				cv::cuda::cvtColor(frame, processedFrame, cv::COLOR_BGR2GRAY);
				processedFrame.download(frameCPU);
				setCurrentMask();
				specificDetect();
				addTimestamp("Detection finished", 0);
			}

			void FeatureDetectionBase::detect(const cv::cuda::GpuMat &frame, bool useLatestMask) {
				useLatestMaskFlag = useLatestMask;
				detect(frame);
			}

			void FeatureDetectionBase::detect(const cv::cuda::GpuMat &frame,
											  const cv::cuda::GpuMat &mask) {
				latestMask = mask;
				detect(frame, true);
			}

			const cv::cuda::GpuMat &FeatureDetectionBase::getOriginalFrame() const {
				return originalFrame;
			}

			FeatureDetectionBase::FeatureDetectionBase() : providentia::utils::TimeMeasurable(
				"FeatureDetectionBase", 1) {}

			const cv::cuda::GpuMat &FeatureDetectionBase::getCurrentMask(cv::Size size) {
				setCurrentMask(std::move(size));
				return currentMask;
			}

			const std::vector<cv::KeyPoint> &FeatureDetectionBase::getKeypoints() const {
				return keypointsCPU;
			}

			bool FeatureDetectionBase::isEmpty() {
				return keypointsCPU.empty();
			}

			cv::Mat FeatureDetectionBase::draw() {
				cv::drawKeypoints(cv::Mat(originalFrame), keypointsCPU, drawFrame);
				return drawFrame;
			}

			const cv::cuda::GpuMat &FeatureDetectionBase::getDescriptorsGPU() const {
				return descriptorsGPU;
			}

			const cv::Mat &FeatureDetectionBase::getDescriptorsCPU() const {
				return descriptorsCPU;
			}

		}
	}
}