//
// Created by brucknem on 12.01.21.
//

#ifndef CAMERASTABILIZATION_FEATUREDETECTIONBASE_HPP
#define CAMERASTABILIZATION_FEATUREDETECTIONBASE_HPP

#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/cuda.hpp"
#include <opencv2/cudafeatures2d.hpp>

#include "../TimeMeasurable.hpp"

namespace providentia {
	namespace stabilization {
		namespace detection {

			/**
			 * Base class for all feature detectors.
			 */
			class FeatureDetectionBase : public providentia::utils::TimeMeasurable {
			protected:

				/**
				 * Flag if the latest mask should be used.
				 */
				bool useLatestMaskFlag = false;

				/**
				 * The current processed GPU frame.
				 */
				cv::cuda::GpuMat processedFrame;

				/**
				 * The current processed CPU frame.
				 */
				cv::Mat frameCPU;

				/**
				 * The current original frame.
				 */
				cv::cuda::GpuMat originalFrame;

				/**
				 * The masks used during processing.
				 */
				cv::cuda::GpuMat latestMask, noMask, currentMask;

				/**
				 * The frame with the features drawn into.
				 */
				cv::Mat drawFrame;

				/**
				 * The CPU feature keypoints.
				 */
				std::vector<cv::KeyPoint> keypointsCPU;

				/**
				 * The CPU feature descriptors.
				 */
				cv::Mat descriptorsCPU;

				/**
				 * The GPU feature descriptors.
				 */
				cv::cuda::GpuMat descriptorsGPU;

				/**
				 * The GPU feature keypoints.
				 */
				cv::cuda::GpuMat keypointsGPU;

				/**
				 * Specific detection implementation.
				 */
				virtual void specificDetect() = 0;

				/**
				 * Sets the current mask from the latest or an empty mask based on the useLatestMask flag.
				 * @see FeatureDetectionBase#useLatestMask
				 */
				void setCurrentMask(cv::Size size = cv::Size());

				/**
				 * @constructor
				 */
				FeatureDetectionBase();

			public:
				/**
				 * @destructor
				 */
				~FeatureDetectionBase() override = default;

				/**
				 * @get
				 */
				const std::vector<cv::KeyPoint> &getKeypoints() const;

				/**
				 * @get
				 */
				const cv::Mat &getDescriptorsCPU() const;

				/**
				 * @get
				 */
				const cv::cuda::GpuMat &getDescriptorsGPU() const;

				/**
				 * @get Gets the current mask from the latest or an empty mask based on the useLatestMask flag.
				 */
				const cv::cuda::GpuMat &getCurrentMask(cv::Size size = cv::Size());

				/**
				 * @get
				 */
				const cv::cuda::GpuMat &getOriginalFrame() const;

				/**
				 * Flag if there is a frame and corresponding keypoints and descriptors present.
				 *
				 * @return true if a detection has been performed previously, false else.
				 */
				bool isEmpty();

				/**
				 * Grayscales the given frame.
				 * Detects keypoints and features using the subclass specific implementations.
				 *
				 * @param frame The frame used for detection.
				 */
				void detect(const cv::cuda::GpuMat &frame);

				/**
				 * Detects keypoints and features using the latest mask or without a mask.
				 *
				 * @param frame The frame used for detection.
				 * @param useLatestMask Flag whether or not to use the latest mask.
				 *
				 * @see FeatureDetectionBase#useLatestMask
				 * @see FeatureDetectionBase#detect(cv::cuda::GpuMat)
				 */
				void detect(const cv::cuda::GpuMat &frame, bool useLatestMask);

				/**
				 * Detects keypoints and features using the given mask.
				 *
				 * @param frame The frame used for detection.
				 * @param mask The mask used for detection.
				 *
				 * @see FeatureDetectionBase#detect(cv::cuda::GpuMat)
				 */
				void detect(const cv::cuda::GpuMat &frame, const cv::cuda::GpuMat &mask);

				/**
				 * Draws the detected features.
				 *
				 * @return The original image with the features drawn.
				 */
				cv::Mat draw();
			};

		}
	}
}
#endif //CAMERASTABILIZATION_FEATUREDETECTIONBASE_HPP
