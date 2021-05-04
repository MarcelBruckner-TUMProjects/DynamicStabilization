//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_BACKGROUNDSEGMENTIONBASE_HPP
#define CAMERASTABILIZATION_BACKGROUNDSEGMENTIONBASE_HPP

#include <opencv2/cudabgsegm.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/cudafilters.hpp"
#include "../TimeMeasurable.hpp"
#include "BackgroundSegmentionBase.hpp"

namespace providentia {
	namespace stabilization {
		namespace segmentation {

/**
 * Base class for the background segmentation wrappers.
 */
			class BackgroundSegmentionBase : public providentia::utils::TimeMeasurable {
			private:

				/**
				 * An internal used mask consisting of on 255 values used for
				 * calculating the background mask as the difference of 255 - foreground mask.
				 */
				cv::cuda::GpuMat all255Mask;

			protected:
				/**
				 * The size used for background calculation.
				 */
				cv::Size calculationSize;
				/**
				 * A CUDA stream used for async calculations.
				 */
				cv::cuda::Stream stream;

				/**
				 * The frame used during calculations.
				 */
				cv::cuda::GpuMat calculationFrame;

				/**
				 * The fore- and background masks.
				 */
				cv::cuda::GpuMat foregroundMask, backgroundMask;

				/**
				 * Some additional filters used during post processing.
				 */
				std::vector<cv::Ptr<cv::cuda::Filter>> filters;

				/**
				 * Subclass specifc algorithm implementation.
				 */
				virtual void specificApply() = 0;

				/**
				 * Additional postprocessing steps on the raw background segmentation result of the algorithm.
				 */
				void postProcess();

				/**
				 * @constructor
				 */
				explicit BackgroundSegmentionBase(cv::Size calculationSize = cv::Size());

				/**
				 * Add optional filters to the postprocessing step.
				 */
				virtual void addFilters();

			public:

				/**
				 * @destructor
				 */
				~BackgroundSegmentionBase() override = default;

				/**
				 * Appends the given frame to the internal history of writeFrames and calculates the background segmentation.
				 *
				 * @param frame The new frame to apply.
				 */
				virtual void segment(const cv::cuda::GpuMat &frame);

				/**
				 * Clears the masks to all background.
				 */
				void clearMasks(const cv::Size &size = cv::Size());

				/**
				 * @get
				 *
				 * @return The background mask.
				 */
				const cv::cuda::GpuMat &getBackgroundMask(const cv::Size &size = cv::Size());

				/**
				 * @get
				 *
				 * @return The foreground mask.
				 */
				const cv::cuda::GpuMat &getForegroundMask(const cv::Size &size = cv::Size());

				/**
				 * Draws the foreground and background masks aside.
				 */
				cv::Mat draw() const;
			};
		}
	}
}

#endif //CAMERASTABILIZATION_BACKGROUNDSEGMENTIONBASE_HPP
