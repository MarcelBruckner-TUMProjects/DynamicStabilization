//
// Created by brucknem on 13.01.21.
//
#include <opencv2/cudawarping.hpp>

#include "DynamicStabilizationBase.hpp"
#include "SURFBFDynamicStabilization.hpp"
#include "ORBBFDynamicStabilization.hpp"
#include "FastFREAKBFDynamicStabilization.hpp"

#include "OpticalFlow.hpp"
#include "Commons.hpp"
#include "CSVWriter.hpp"
#include <boost/filesystem/convenience.hpp>

using namespace providentia::evaluation;

/**
 * Setup to visualize the total stabilization algorithm.
 */
class Setup : public ::VideoSetup {
private:
	/**
	 * The matcher used to match the features.
	 */
	std::vector<providentia::stabilization::DynamicStabilizationBase> stabilizers;
	std::vector<std::string> stabilizerNames = {
		"SURF",
//		"ORB",
//		"FAST"
	};
	std::vector<providentia::opticalflow::FarnebackDenseOpticalFlow> opticalFlows;

	std::shared_ptr<::CSVWriter> csvWriter;
	std::shared_ptr<::CSVWriter> timeWriter;
	int frameId = -1;
	bool withMask = false;
	int padding = 10;
	boost::filesystem::path evaluationPath;
	bool writeBadFrames = false;
	int warmup = 30;

public:
	explicit Setup() : VideoSetup() {
		stabilizers.emplace_back(providentia::stabilization::SURFBFDynamicStabilization{});
//		stabilizers.emplace_back(providentia::stabilization::ORBBFDynamicStabilization{});
//		stabilizers.emplace_back(providentia::stabilization::FastFREAKBFDynamicStabilization{});

		for (int i = 0; i <= stabilizers.size(); i++) {
			opticalFlows.emplace_back();
		}
	}

	void init() override {
		VideoSetup::init();

		evaluationPath =
			outputFolder / "DynamicStabilization" / boost::filesystem::path(inputResource).filename().string();
		if (!boost::filesystem::is_directory(evaluationPath)) {
			boost::filesystem::create_directories(evaluationPath);
		}

		csvWriter = std::make_shared<::CSVWriter>(evaluationPath / (boost::filesystem::path(inputResource).filename()
																		.string() + getNowSuffix() + ".csv"));

		timeWriter = std::make_shared<::CSVWriter>(evaluationPath / (boost::filesystem::path(inputResource).filename()
																		 .string() + "_times_" + getNowSuffix() +
																	 ".csv"));
		*csvWriter << "Frame" << "Original";
		for (const auto &name : stabilizerNames) {
			*csvWriter << name;
		}
		*csvWriter << newline;
		*timeWriter << "Frame" << "Original";
		for (const auto &name : stabilizerNames) {
			*timeWriter << name;
		}
		*timeWriter << newline;
	}

	void calculateOpticalFlows() {
		std::vector<cv::cuda::GpuMat> frames{pad(frameGPU, padding)};
		auto size = pad(frameGPU, padding).size();

		std::vector<cv::cuda::GpuMat> backgroundMasks{stabilizers[0].getBackgroundMask(size)};

		for (const auto &stabilizer :stabilizers) {
			frames.emplace_back(pad(stabilizer.getStabilizedFrame(), padding));
			backgroundMasks.emplace_back(pad(stabilizer.getBackgroundMask(size), padding));
		}

		int i = 0;
		for (auto &opticalFlow : opticalFlows) {
			if (withMask) {
				opticalFlow.calculate(frames[i], backgroundMasks[i]);
			} else {
				opticalFlow.calculate(frames[i]);
			}
			i++;
		}
	}

	void specificMainLoop() override {
		frameId++;

//		std::vector<cv::Mat> frames{
//			::pad(::addText(frameCPU, "Frame: " + std::to_string(frameId), 2, 5, 5), padding)
//		};

		std::vector<cv::Mat> frames{
			::pad(frameCPU, padding)
		};

		*timeWriter << frameId << 0;

		int i = 0;
		for (auto &stabilizer :stabilizers) {
			stabilizer.stabilize(frameGPU);
			*timeWriter << stabilizer.getTotalMilliseconds();
//
//			frames.emplace_back(::pad(
//				::addRuntimeToFrame(cv::Mat(stabilizer.getStabilizedFrame()), stabilizerNames[i], stabilizer
//					.getTotalMilliseconds(), 2, 5, 5), padding)
//			);

			frames.emplace_back(::pad(cv::Mat(stabilizer.getStabilizedFrame()), padding));
			i++;
		}
		*timeWriter << newline;

		calculateOpticalFlows();
//		cv::hconcat(frames, finalFrame);

		frames.clear();
		for (auto &opticalFlow : opticalFlows) {
//			frames.emplace_back(
//				::addText(opticalFlow.draw(),
//						  "Mean pixel shift: " + std::to_string(opticalFlow.getMagnitudeMean()), 2, 5, 5)
//			);
			frames.emplace_back(opticalFlow.draw());
		}

		cv::hconcat(frames, finalFrame);
//		cv::hconcat(frames, bufferCPU);
//		finalFrame = ::vconcat({finalFrame, bufferCPU});
//
//		if (withMask) {
//			frames.clear();
//			frames.emplace_back(::cvtColor(stabilizers[0].getBackgroundMask(frameGPU.size()), cv::COLOR_GRAY2BGR));
//
//			for (auto &stabilizer :stabilizers) {
//				::pad(::cvtColor(stabilizer.getBackgroundMask(frameGPU.size()), cv::COLOR_GRAY2BGR), padding);
//			}
//			cv::hconcat(frames, bufferCPU);
//			finalFrame = ::vconcat({finalFrame, bufferCPU});
//		}
//
//		if (frameId < warmup) {
//			return;
//		}
//
//		double originalMagnitudeMean = opticalFlows[0].getMagnitudeMean();
//
//		*csvWriter << frameId;
//		bool write = false;
//		std::string frameName = "frame_" + std::to_string(frameId);
//
//		int flowNumber = 0;
//		for (auto &opticalFlow: opticalFlows) {
//			double currentMagnitudeMean = opticalFlow.getMagnitudeMean();
//			*csvWriter << currentMagnitudeMean;
//
//			if (originalMagnitudeMean - currentMagnitudeMean < 0) {
//				frameName += "_";
//				switch (flowNumber) {
//					case 1:
//						frameName += "surf";
//						break;
//					case 2:
//						frameName += "orb";
//						break;
//					case 3:
//						frameName += "fast";
//						break;
//					default:
//						break;
//				}
//				write = true;
//			}
//			flowNumber++;
//		}
//		if (write && writeBadFrames) {
//			cv::imwrite((evaluationPath / (frameName + ".png")).string(), finalFrame);
//		}
		*csvWriter << newline;
	}
};

int main(int argc, char const *argv[]) {
	Setup setup;
	setup.fromCLI(argc, argv);
	setup.setRenderingScaleFactor(0.4);
	setup.mainLoop();
	return 0;
}
