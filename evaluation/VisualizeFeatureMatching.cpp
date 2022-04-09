//
// Created by brucknem on 13.01.21.
//

#include "Commons.hpp"
#include "DynamicStabilization/matching/FeatureMatchingBase.hpp"
#include "DynamicStabilization/matching/BruteForceFeatureMatching.hpp"

#include "DynamicStabilization/detection/FeatureDetectionBase.hpp"
#include "DynamicStabilization/detection/SURFFeatureDetection.hpp"

/**
 * Setup to visualize the feature matching.
 */
class Setup : public dynamic_stabilization::evaluation::VideoSetup {
private:
    /**
     * The current frame feature detector applied in the main loop.
     */
    std::shared_ptr<dynamic_stabilization::stabilization::detection::FeatureDetectionBase> frameDetector, referenceFrameDetector;

    /**
     * The matcher used to match the features.
     */
    std::shared_ptr<dynamic_stabilization::stabilization::matching::FeatureMatchingBase> matcher, matcherWithoutFundamental;

public:
    explicit Setup() : VideoSetup() {
//        dynamic_stabilization::features::SIFTFeatureDetection detector(100);
//        frameDetector = std::make_shared<dynamic_stabilization::features::SIFTFeatureDetection>(detector);
//        referenceFrameDetector = std::make_shared<dynamic_stabilization::features::SIFTFeatureDetection>(detector);

        dynamic_stabilization::stabilization::detection::SURFFeatureDetection detector;
        frameDetector = std::make_shared<dynamic_stabilization::stabilization::detection::SURFFeatureDetection>(
                detector);
        referenceFrameDetector = std::make_shared<dynamic_stabilization::stabilization::detection::SURFFeatureDetection>(
                detector);

//        dynamic_stabilization::features::ORBFeatureDetection detector(1000);
//        frameDetector = std::make_shared<dynamic_stabilization::features::ORBFeatureDetection>(detector);
//        referenceFrameDetector = std::make_shared<dynamic_stabilization::features::ORBFeatureDetection>(detector);

//        dynamic_stabilization::features::FastFREAKFeatureDetection detector;
//        frameDetector = std::make_shared<dynamic_stabilization::features::FastFREAKFeatureDetection>(detector);
//        referenceFrameDetector = std::make_shared<dynamic_stabilization::features::FastFREAKFeatureDetection>(detector);

        matcher = std::make_shared<dynamic_stabilization::stabilization::matching::BruteForceFeatureMatching>(
                cv::NORM_L2);
        matcherWithoutFundamental = std::make_shared<dynamic_stabilization::stabilization::matching::BruteForceFeatureMatching>
                (cv::NORM_L2);
        matcherWithoutFundamental->setShouldUseFundamentalMatrix(false);
//        matcher = std::make_shared<dynamic_stabilization::features::BruteForceFeatureMatching>(cv::NORM_HAMMING);
//        matcher = std::make_shared<dynamic_stabilization::features::FlannFeatureMatching>(true);

    }

    void specificMainLoop() override {
        writeFrames = true;
        outputFolder = "visualize_feature_matching";
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
