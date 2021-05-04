//
// Created by brucknem on 13.01.21.
//

#ifndef CAMERASTABILIZATION_MOG2BACKGROUNDSEGMENTATION_HPP
#define CAMERASTABILIZATION_MOG2BACKGROUNDSEGMENTATION_HPP

#include <opencv2/cudabgsegm.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/cudafilters.hpp"
#include "../TimeMeasurable.hpp"
#include "BackgroundSegmentionBase.hpp"

namespace providentia {
	namespace stabilization {
		namespace segmentation {

			/**
			 * Wrapper for the MOG2 background segmentation algorithm.
			 */
			class MOG2BackgroundSegmentation : public BackgroundSegmentionBase {
			private:

				/**
				 * The algorithm implementation.
				 */
				cv::Ptr<cv::cuda::BackgroundSubtractorMOG2> algorithm;

			protected:

				/**
				 * @copydoc
				 */
				void specificApply() override;

				/**
				 * @copydoc
				 */
				void addFilters() override;

			public:

				/**
				 * @constructor
				 *
				 * @param calculationSize The scaling size of the frame during calculation.
				 * @param history Length of the history.
				 * @param varThreshold Threshold on the squared Mahalanobis distance between the expectedPixel and the model
				 * to decide whether a expectedPixel is well described by the background model. This parameter does not
				 * affect the background update.
				 * @param detectShadows If true, the algorithm will detect shadows and mark them. It decreases the
				 * speed a bit, so if you do not need this feature, set the parameter to false.
				 */
				explicit MOG2BackgroundSegmentation(cv::Size calculationSize = cv::Size(), int history = 500,
													double varThreshold = 16,
													bool detectShadows = false);

				/**
				 * @desctructor
				 */
				~MOG2BackgroundSegmentation() override = default;
			};
		}
	}
}

#endif //CAMERASTABILIZATION_MOG2BACKGROUNDSEGMENTATION_HPP
