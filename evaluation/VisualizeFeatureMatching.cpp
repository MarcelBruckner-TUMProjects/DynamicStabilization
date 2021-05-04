//
// Created by brucknem on 13.01.21.
//

#include "Commons.hpp"
#include "matching/FeatureMatchingBase.hpp"
#include "matching/BruteForceFeatureMatching.hpp"

#include "detection/FeatureDetectionBase.hpp"
#include "detection/SURFFeatureDetection.hpp"

/**
 * Setup to visualize the feature matching.
 */
class Setup : public providentia::evaluation::VideoSetup {
private:
	/**
	 * The current frame feature detector applied in the main loop.
	 */
	std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> frameDetector, referenceFrameDetector;

	/**
	 * The matcher used to match the features.
	 */
	std::shared_ptr<providentia::stabilization::matching::FeatureMatchingBase> matcher, matcherWithoutFundamental;

public:
	explicit Setup() : VideoSetup() {
//        providentia::features::SIFTFeatureDetection detector(100);
//        frameDetector = std::make_shared<providentia::features::SIFTFeatureDetection>(detector);
//        referenceFrameDetector = std::make_shared<providentia::features::SIFTFeatureDetection>(detector);

		providentia::stabilization::detection::SURFFeatureDetection detector;
		frameDetector = std::make_shared<providentia::stabilization::detection::SURFFeatureDetection>(detector);
		referenceFrameDetector = std::make_shared<providentia::stabilization::detection::SURFFeatureDetection>(
			detector);

//        providentia::features::ORBFeatureDetection detector(1000);
//        frameDetector = std::make_shared<providentia::features::ORBFeatureDetection>(detector);
//        referenceFrameDetector = std::make_shared<providentia::features::ORBFeatureDetection>(detector);

//        providentia::features::FastFREAKFeatureDetection detector;
//        frameDetector = std::make_shared<providentia::features::FastFREAKFeatureDetection>(detector);
//        referenceFrameDetector = std::make_shared<providentia::features::FastFREAKFeatureDetection>(detector);

		matcher = std::make_shared<providentia::stabilization::matching::BruteForceFeatureMatching>(cv::NORM_L2);
		matcherWithoutFundamental = std::make_shared<providentia::stabilization::matching::BruteForceFeatureMatching>
			(cv::NORM_L2);
		matcherWithoutFundamental->setShouldUseFundamentalMatrix(false);
//        matcher = std::make_shared<providentia::features::BruteForceFeatureMatching>(cv::NORM_HAMMING);
//        matcher = std::make_shared<providentia::features::FlannFeatureMatching>(true);


	}

	void specificMainLoop() override {
		frameDetector->detect(frameGPU);
		if (referenceFrameDetector->isEmpty()) {
			referenceFrameDetector->detect(frameGPU);
		}

		matcher->match(frameDetector, referenceFrameDetector);
//		matcherWithoutFundamental->match(frameDetector, referenceFrameDetector);
//		totalAlgorithmsDuration = frameDetector->getTotalMilliseconds() + matcher->getTotalMilliseconds() +
//								  matcherWithoutFundamental->getTotalMilliseconds();
//
//		cv::vconcat(matcherWithoutFundamental->draw(), matcher->draw(), finalFrame);

		finalFrame = matcher->draw();
	}

	void specificAddMessages() override {
//		addRuntimeToFinalFrame("Feature detection", frameDetector->getTotalMilliseconds(), 5, 20);
//		addRuntimeToFinalFrame("Feature matching", matcher->getTotalMilliseconds(), 5, 40);
	}
};

int main(int argc, char const *argv[]) {
	Setup setup;
	setup.fromCLI(argc, argv);
	setup.setRenderingScaleFactor(1);
	setup.mainLoop();
	return 0;
}
