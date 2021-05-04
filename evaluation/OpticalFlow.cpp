//
// Created by brucknem on 18.01.21.
//

#include "OpticalFlow.hpp"
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include "opencv2/cudaoptflow.hpp"

namespace providentia {
	namespace opticalflow {

#pragma region DenseOpticalFlow

		void DenseOpticalFlow::initialize() {
			previousFrame = currentFrame.clone();
			hsv = cv::Mat(currentFrame.size(), CV_8UC3, cv::Scalar(0, 0, 0, 0));
			bgr = cv::Mat(currentFrame.size(), CV_8UC3, cv::Scalar(0, 0, 0, 0));
		}

		void DenseOpticalFlow::calculate(const cv::cuda::GpuMat &_frame, const cv::cuda::GpuMat &_mask) {
			cv::cuda::cvtColor(_frame, currentFrame, cv::COLOR_BGR2GRAY);
			mask = _mask;

			gaussianBlur->apply(currentFrame, currentFrame);

			if (previousFrame.empty()) {
				initialize();
				return;
			}

			specificCalculate();

			denseOpticalFlowGPU.download(denseOpticalFlowCPU);

			cv::split(denseOpticalFlowCPU, flowParts);

			cv::cartToPolar(flowParts[0], flowParts[1], flowParts[0], flowParts[1], true);
			magnitude = flowParts[0];
			cv::threshold(magnitude, magnitude, 2, 1e10, cv::ThresholdTypes::THRESH_TOZERO);

			angle = flowParts[1];
			normalize(flowParts[0], flowParts[2], 0.0f, 1.0f, cv::NORM_MINMAX);
			flowParts[1] *= ((1.f / 360.f) * (180.f / 255.f));

			//build hsv image
			_hsv[0] = flowParts[1];
			_hsv[1] = cv::Mat::ones(flowParts[1].size(), CV_32F);
			_hsv[2] = flowParts[2];
			merge(_hsv.data(), 3, hsv);

			hsv.convertTo(hsv8, CV_8U, 255.0);
			cvtColor(hsv8, bgr, cv::COLOR_HSV2BGR);

			previousFrame = currentFrame.clone();
		}

		double DenseOpticalFlow::getMagnitudeMean() {
			return cv::mean(magnitude)[0];
		}

		double DenseOpticalFlow::getAngleMean() {
			return cv::mean(angle)[0];
		}

		const cv::Mat &DenseOpticalFlow::draw() const {
			return bgr;
		}

		DenseOpticalFlow::DenseOpticalFlow() :
			gaussianBlur(cv::cuda::createGaussianFilter(CV_8UC1, CV_8UC1, cv::Size(5, 5), 75)) {}

#pragma endregion DenseOpticalFlow

#pragma region FarnebackDenseOpticalFlow

		void FarnebackDenseOpticalFlow::specificCalculate() {
			cv::cuda::GpuMat previousFrameMasked = previousFrame.clone();
			cv::cuda::GpuMat currentFrameMasked = currentFrame.clone();
			if (!mask.empty()) {
				cv::cuda::bitwise_and(previousFrameMasked, mask, previousFrameMasked);
				cv::cuda::bitwise_and(currentFrameMasked, mask, currentFrameMasked);
			}
			opticalFlow->calc(previousFrameMasked, currentFrameMasked, denseOpticalFlowGPU, stream);
		}

		FarnebackDenseOpticalFlow::FarnebackDenseOpticalFlow() : DenseOpticalFlow() {
			opticalFlow = cv::cuda::FarnebackOpticalFlow::create();
		}

#pragma endregion FarnebackDenseOpticalFlow
	}
}
