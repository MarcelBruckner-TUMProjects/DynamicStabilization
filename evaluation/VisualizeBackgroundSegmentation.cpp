//
// Created by brucknem on 13.01.21.
//
#include "Commons.hpp"
#include "DynamicStabilization/segmentation/BackgroundSegmentionBase.hpp"
#include "DynamicStabilization/segmentation/MOG2BackgroundSegmentation.hpp"

/**
 * Setup to visualize the background segmentation.
 */
class Setup : public dynamic_stabilization::evaluation::VideoSetup {
private:
	/**
	 * The matcher used to match the features.
	 */
	std::shared_ptr<dynamic_stabilization::stabilization::segmentation::BackgroundSegmentionBase> segmentor;

public:
	explicit Setup() : VideoSetup() {
		segmentor = std::make_shared<dynamic_stabilization::stabilization::segmentation::MOG2BackgroundSegmentation>();
	}

	void specificMainLoop() override {
		segmentor->segment(frameGPU);
		totalAlgorithmsDuration = segmentor->getTotalMilliseconds();
		finalFrame = segmentor->draw();
	}

	void specificAddMessages() override {
		addRuntimeToFinalFrame("MOG2 Background segmentation", segmentor->getTotalMilliseconds(), 5, 20);
	}
};

int main(int argc, char const *argv[]) {
	Setup setup;
	setup.mainLoop();
	return 0;
}
