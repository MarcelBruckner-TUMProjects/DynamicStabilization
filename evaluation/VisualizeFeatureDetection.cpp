//
// Created by brucknem on 13.01.21.
//
#include <opencv2/cudawarping.hpp>
#include "DynamicStabilization/detection/FeatureDetectionBase.hpp"
#include "DynamicStabilization/detection/SURFFeatureDetection.hpp"
#include "DynamicStabilization/detection/ORBFeatureDetection.hpp"
#include "DynamicStabilization/detection/FastFREAKFeatureDetection.hpp"
#include "DynamicStabilization/detection/StarBRIEFFeatureDetection.hpp"
#include "DynamicStabilization/detection/SIFTFeatureDetection.hpp"

#include "Commons.hpp"

/**
 * Setup to visualize the feature detection.
 */
class Setup : public dynamic_stabilization::evaluation::VideoSetup {
private:
	/**
	 * The detectors and scaling factors that are applied in the main loop.
	 */
	std::vector<std::pair<dynamic_stabilization::stabilization::detection::FeatureDetectionBase *, float>> detectors;

public:
	explicit Setup() : VideoSetup() {
		detectors.emplace_back(std::make_pair(new dynamic_stabilization::stabilization::detection::SURFFeatureDetection(), 1.0));
		detectors.emplace_back(std::make_pair(new dynamic_stabilization::stabilization::detection::ORBFeatureDetection(), 1.0));
		detectors.emplace_back(std::make_pair(new dynamic_stabilization::stabilization::detection::SIFTFeatureDetection(), 1.0));
		detectors.emplace_back(
			std::make_pair(new dynamic_stabilization::stabilization::detection::FastFREAKFeatureDetection(), 1.0));
		detectors.emplace_back(
			std::make_pair(new dynamic_stabilization::stabilization::detection::StarBRIEFFeatureDetection(), 1.0));
	}

	void specificMainLoop() override {
		for (const auto &entry : detectors) {
			cv::cuda::resize(frameGPU, bufferGPU, cv::Size(), entry.second, entry.second);
			entry.first->detect(bufferGPU);
			totalAlgorithmsDuration += entry.first->getTotalMilliseconds();
			bufferCPU = entry.first->draw();
			cv::resize(bufferCPU, bufferCPU, frameCPU.size());
			dynamic_stabilization::evaluation::addRuntimeToFrame(bufferCPU,
													   std::string(typeid(*entry.first).name()) + " [" +
													   std::to_string(entry.second) + "]",
													   entry.first->getTotalMilliseconds(),
													   2, 5, 20);

			if (finalFrame.empty()) {
				finalFrame = bufferCPU.clone();
			} else {
				cv::hconcat(std::vector<cv::Mat>{finalFrame, bufferCPU}, finalFrame);
			}
		}
	}
};

int main(int argc, char const *argv[]) {
	Setup setup;
	setup.setRenderingScaleFactor(1);
	setup.mainLoop();
	return 0;
}
