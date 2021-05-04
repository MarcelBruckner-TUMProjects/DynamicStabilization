//
// Created by brucknem on 18.01.21.
//

#ifndef CAMERASTABILIZATION_OPTICALFLOW_HPP
#define CAMERASTABILIZATION_OPTICALFLOW_HPP

#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudaarithm.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudaoptflow.hpp"

namespace providentia {
	namespace opticalflow {

		/**
		 * Base class for dense optical flow algorithms
		 */
		class DenseOpticalFlow {
		protected:
			/**
			 * The current and previous GPU frame.
			 */
			cv::cuda::GpuMat currentFrame, previousFrame, mask;

			cv::Ptr<cv::cuda::Filter> gaussianBlur;

			/**
			 * The calculated dense optical flow.
			 */
			cv::Mat denseOpticalFlowCPU;
			cv::cuda::GpuMat denseOpticalFlowGPU;

			/**
			 * Buffer matrices.
			 */
			cv::Mat hsv, magnitude, angle, angles;

			/**
			 * The three channels of the optical flow.
			 */
			std::vector<cv::Mat> flowParts{3};

			/**
			 * The CUDA stream for async calculations on GPU.
			 */
			cv::cuda::Stream stream;

			/**
			 * Color writeFrames for visualization.
			 */
			cv::Mat hsv8, bgr;

			/**
			 * Color writeFrames for merging the channels.
			 */
			std::array<cv::Mat, 3> _hsv;

			/**
			 * @constructor
			 */
			explicit DenseOpticalFlow();

			/**
			 * Initializes the frame buffers on the first input frame.
			 */
			void initialize();

			/**
			 * Algorithm specific calculation function.
			 */
			virtual void specificCalculate() = 0;

		public:

			/**
			 * @destructor
			 */
			virtual ~DenseOpticalFlow() = default;

			/**
			 * @get the mean of the magnitudes over the flow field.
			 */
			double getMagnitudeMean();

			/**
			 * @get the mean of the angles over the flow field.
			 */
			double getAngleMean();

			/**
			 * Calculates the image moments of the optical flow magnitude mat.
			 */
			std::vector<double> getFlowMoments();

			/**
			 * Draws the optical flow field as BGR image.
			 * @return
			 */
			const cv::Mat &draw() const;

			/**
			 * Main calculation function.
			 *
			 * @param _frame The frame to process.
			 */
			void calculate(const cv::cuda::GpuMat &_frame, const cv::cuda::GpuMat &mask = cv::cuda::GpuMat());

		};

		/**
		 * Wrapper of the Farneback dense optical flow algorithm.
		 */
		class FarnebackDenseOpticalFlow : public DenseOpticalFlow {
		private:

			/**
			 * The algorithm implementation.
			 */
			cv::Ptr<cv::cuda::FarnebackOpticalFlow> opticalFlow;

		protected:
			/**
			 * @copydoc
			 */
			void specificCalculate() override;

		public:
			/**
			 * @constructor
			 */
			explicit FarnebackDenseOpticalFlow();

			/**
			 * @destructor
			 */
			~FarnebackDenseOpticalFlow() override = default;

		};

	}
}

#endif //CAMERASTABILIZATION_OPTICALFLOW_HPP
