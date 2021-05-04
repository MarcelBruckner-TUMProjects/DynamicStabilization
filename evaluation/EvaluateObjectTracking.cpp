//
// Created by brucknem on 13.01.21.
//
#include <opencv2/cudawarping.hpp>

#include "DynamicStabilizationBase.hpp"
#include "SURFBFDynamicStabilization.hpp"
#include "ORBBFDynamicStabilization.hpp"
#include "FastFREAKBFDynamicStabilization.hpp"

#include "Commons.hpp"
#include "CSVWriter.hpp"
#include <boost/filesystem/convenience.hpp>
#include "ObjectTracking.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include "warping/FrameWarping.hpp"
#include "Color.hpp"

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
	std::vector<std::string> stabilizerNames = {"Original"};

	ObjectTracking tracking;
	std::vector<cv::Rect2d> originalBoundingBoxes;
	std::vector<cv::Scalar> boundingBoxColors;
	std::vector<std::string> objectNames;

	CSVWriter *csvWriter;
	int frameId = 0;
	int trackerType = 2;
	int numParallelRuns = 1;
	int warmup = 35;

public:
	explicit Setup() : VideoSetup() {}

	boost::program_options::variables_map fromCLI(int argc, const char **argv) override {
		auto vm = VideoSetup::fromCLI(argc, argv);

		cv::RNG rng(43);
		std::vector<std::string> rawBboxes;
		boost::split(rawBboxes, vm["bboxes"].as<std::string>(), [](char c) { return c == ','; });
		boost::split(objectNames, vm["names"].as<std::string>(), [](char c) { return c == ','; });

		if (rawBboxes.size() % 4 != 0) {
			std::cout << "The bounding boxes have to be defined as packs of 4 [x, y, w, h] values." << std::endl;
			exit(EXIT_FAILURE);
		}
		if (rawBboxes.size() / 4 != objectNames.size()) {
			std::cout << "There must be given exactly the same number of names and bounding boxes." << std::endl;
			exit(EXIT_FAILURE);
		}

		std::vector<int> rawBboxValues;
		BOOST_FOREACH(std::string value, rawBboxes) { rawBboxValues.emplace_back(std::stoi(value)); };
		for (int i = 0; i < rawBboxes.size(); i += 4) {
			originalBoundingBoxes.emplace_back(
				rawBboxValues[i + 0], rawBboxValues[i + 1],
				rawBboxValues[i + 2], rawBboxValues[i + 3]
			);
		}

		boundingBoxColors.emplace_back(Color::beige);
		boundingBoxColors.emplace_back(Color::red);
		boundingBoxColors.emplace_back(Color::limegreen);
		return vm;
	}

	void addAdditionalOptions(po::options_description *desc) override {
		desc->add_options()
			("bboxes,b", po::value<std::string>()->default_value(""), "The initial bounding boxes of the vehicles to "
																	  "track.")
			("names,n", po::value<std::string>()->default_value(""), "The names of the vehicles to track.");
	}

	void init() override {
		VideoSetup::init();
		for (int i = 0; i < numParallelRuns; i++) {
			stabilizers.emplace_back(providentia::stabilization::SURFBFDynamicStabilization());
//			stabilizers.emplace_back(providentia::stabilization::ORBBFDynamicStabilization());
//			stabilizers.emplace_back(providentia::stabilization::FastFREAKBFDynamicStabilization());
			stabilizerNames.emplace_back("SURF");
//			stabilizerNames.emplace_back("ORB");
//			stabilizerNames.emplace_back("Fast");
		}

		outputFolder = outputFolder / "ObjectTracking";
		if (!boost::filesystem::is_directory(outputFolder)) {
			boost::filesystem::create_directories(outputFolder);
		}

		getNextFrame();

		csvWriter = new CSVWriter(outputFolder / (boost::filesystem::path(inputResource).filename().string() +
												  ".csv"));
		*csvWriter << "Frame";
		*csvWriter << "Vehicle";
		for (const auto &name : stabilizerNames) {
			TrackerWrapper::addHeader(csvWriter, name);
		}

		for (int i = 0; i < originalBoundingBoxes.size(); i++) {
			tracking.addTracker(frameCPU, trackerType, originalBoundingBoxes[i], boundingBoxColors[i]);
		}
		*csvWriter << newline;
	}

	void specificMainLoop() override {
		frameId++;
		bufferCPU = tracking.track(frameCPU);

		if (tracking.isTrackingLost()) {
			exit(EXIT_SUCCESS);
		}

		bufferGPU.upload(bufferCPU);

		std::vector<cv::Mat> homographies;
		homographies.emplace_back(cv::Mat::eye(3, 3, CV_64F));
		for (auto &stabilizer : stabilizers) {
			stabilizer.stabilize(frameGPU);
			homographies.emplace_back(stabilizer.getHomography());
		}

		std::vector<cv::Mat> resultFrames;
		for (int i = 0; i < homographies.size(); i++) {
//			resultFrames.emplace_back(
//				::addText(
//					(tracking * homographies[i]).draw(
//						cv::Mat(providentia::stabilization::FrameWarping::warp(frameGPU, homographies[i]))
//					), stabilizerNames[i] + " - Frame: " + std::to_string(frameId), 2, 5,
//					frameCPU.rows - 50)
//			);
			resultFrames.emplace_back(
				(tracking * homographies[i]).draw(
					cv::Mat(providentia::stabilization::FrameWarping::warp(frameGPU, homographies[i]))
				)
			);
		}

		std::vector<providentia::evaluation::ObjectTracking> warpedTrackers;
		for (auto &homographie : homographies) {
			warpedTrackers.emplace_back(tracking * homographie);
		}

		if (warmup < frameId) {
			for (int i = 0; i < originalBoundingBoxes.size(); i++) {
				*csvWriter << frameId << objectNames[i];
				int j = 0;
				for (const auto &warpedTracker : warpedTrackers) {
					bufferCPU = resultFrames[j];
					auto midPoint = warpedTracker.getMidpoint(i);
					std::stringstream ss;
					ss << objectNames[i] << " ";
					ss << "[";
					ss << std::setw(7) << midPoint.x;
					ss << ", ";
					ss << std::setw(7) << midPoint.y;
					ss << "]";
					bufferCPU = ::addText(bufferCPU, ss.str(), 2, 5, (int) (5. + frameCPU.rows * 0.05 * i),
										  boundingBoxColors[i]);
					resultFrames[j] = bufferCPU;
					j++;
					*csvWriter << warpedTracker.getTrackers()[i];
				}
				*csvWriter << newline;
			}
		}
		cv::hconcat(resultFrames, finalFrame);
	}
};

int main(int argc, char const *argv[]) {
	Setup setup;
	setup.fromCLI(argc, argv);
	setup.setRenderingScaleFactor(0.4);
//	setup.setRenderingScaleFactor(1);
	setup.mainLoop();
	return 0;
}
