//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_BRUTEFORCEFEATUREMATCHING_HPP
#define CAMERASTABILIZATION_BRUTEFORCEFEATUREMATCHING_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/cudafeatures2d.hpp"
#include "../detection/FeatureDetectionBase.hpp"
#include "../TimeMeasurable.hpp"
#include "FeatureMatchingBase.hpp"

namespace providentia {
	namespace stabilization {
		namespace matching {
/**
 * Brute force feature matching on GPU.
 */
			class BruteForceFeatureMatching : public providentia::stabilization::matching::FeatureMatchingBase {
			private:
				/**
				 * The CUDA GPU stream used during matching.
				 */
				cv::cuda::Stream stream;

				/**
				 * The Brute Force matching algorithm.
				 */
				cv::Ptr<cv::cuda::DescriptorMatcher> matcher;

			protected:
				/**
				 * @copydoc
				 */
				void specificMatch() override;

			public:
				/**
				 * @destructor
				 */
				~BruteForceFeatureMatching() override = default;

				/**
				 * @constructor
				 *
				 * @param norm The norm used to compare the features.
				 * @param goodMatchRatioThreshold The ratio threshold of good matches for the Lowe's ratio toCameraSpace.
				 */
				explicit BruteForceFeatureMatching(cv::NormTypes norm, float goodMatchRatioThreshold = 0.75f);

			};
		}
	}
}

#endif //CAMERASTABILIZATION_BRUTEFORCEFEATUREMATCHING_HPP
