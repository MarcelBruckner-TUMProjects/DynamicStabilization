//
// Created by brucknem on 11.04.21.
//

#ifndef CAMERASTABILIZATION_FLANNFEATUREMATCHING_HPP
#define CAMERASTABILIZATION_FLANNFEATUREMATCHING_HPP

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
 * CPU implementation of the FLANN feature matcher.
 */
			class FlannFeatureMatching : public providentia::stabilization::matching::FeatureMatchingBase {
			private:

				/**
				 * The CPU FLANN matching algorithm.
				 */
				cv::Ptr<cv::FlannBasedMatcher> matcher;

			protected:
				/**
				 * @copydoc
				 */
				void specificMatch() override;

			public:

				/**
				 * @destructor
				 */
				~FlannFeatureMatching() override = default;

				/**
				 * @constructor
				 *
				 * @param binaryDescriptors Flag if binary desccriptors are used.
				 * @param goodMatchRatioThreshold The ratio threshold of good matches for the Lowe's ratio toCameraSpace.
				 */
				explicit FlannFeatureMatching(bool binaryDescriptors = false,
											  float goodMatchRatioThreshold = 0.75f);

				/**
				 * @constructor
				 *
				 * @params The FLANN parameters.
				 * @param goodMatchRatioThreshold The ratio threshold of good matches for the Lowe's ratio toCameraSpace.
				 */
				explicit FlannFeatureMatching(cv::flann::IndexParams *params, float goodMatchRatioThreshold = 0.75f);

			};
		}
	}
}

#endif //CAMERASTABILIZATION_FLANNFEATUREMATCHING_HPP
