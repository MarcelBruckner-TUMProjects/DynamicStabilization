//
// Created by brucknem on 13.01.21.
//
#include "Commons.hpp"
#include "segmentation/BackgroundSegmentionBase.hpp"
#include "segmentation/MOG2BackgroundSegmentation.hpp"

/**
 * Setup to visualize the background segmentation.
 */
class Setup : public providentia::evaluation::VideoSetup {
private:
	/**
	 * The matcher used to match the features.
	 */
	std::shared_ptr<providentia::stabilization::segmentation::BackgroundSegmentionBase> segmentor;

public:
	explicit Setup() : VideoSetup() {
		segmentor = std::make_shared<providentia::stabilization::segmentation::MOG2BackgroundSegmentation>();
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
