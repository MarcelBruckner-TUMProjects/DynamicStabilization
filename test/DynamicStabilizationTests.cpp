//
// Created by brucknem on 12.01.21.
//
#include "gtest/gtest.h"
#include "ImageTestBase.hpp"
#include "DynamicStabilizationBase.hpp"
#include "SURFBFDynamicStabilization.hpp"
#include "ORBBFDynamicStabilization.hpp"
#include "FastFREAKBFDynamicStabilization.hpp"
#include <iostream>

namespace providentia {
	namespace tests {

		/**
		 * Setup for the dynamic stabilizer toCameraSpace.
		 */
		class DynamicStabilizationTests : public ImageTestBase {
		public:

			/**
			 * @destructor
			 */
			~DynamicStabilizationTests() override = default;

			/**
			 * Asserts that the given stabilizer runs and gives nearly the identity.
			 */
			void assertStabilization(providentia::stabilization::DynamicStabilizationBase *stabilizer) {
				stabilizer->stabilize(testImgGPU);

				cv::Mat homography = stabilizer->getHomography();
				for (int row = 0; row < 3; row++) {
					for (int column = 0; column < 3; column++) {
						double value = homography.at<double>(row, column);
						if (row == column) {
							value -= 1;
						}
						EXPECT_NEAR(value, 0, 1e-2);
					}
				}

				cv::Mat warpedTestImg;
				cv::cuda::GpuMat warpedTestImgGPU;
				cv::Mat testHomography = cv::getRotationMatrix2D(cv::Point2f(400, 500), -10, 1.0);
				cv::warpAffine(testImgCPU, warpedTestImg, testHomography, testImgCPU.size(), cv::INTER_LINEAR);
				warpedTestImgGPU.upload(warpedTestImg);

				stabilizer->stabilize(warpedTestImgGPU);
				homography = stabilizer->getHomography();
				homography = cv::Mat(homography.inv());

				for (int row = 0; row < 2; row++) {
					for (int column = 0; column < 2; column++) {
						double actual = homography.at<double>(row, column);
						double expected = testHomography.at<double>(row, column);
						double difference = expected - actual;
						EXPECT_NEAR(difference, 0, 1e-2);
					}
				}

				EXPECT_NEAR(homography.at<double>(0, 2) - testHomography.at<double>(0, 2), 0, 0.6);
				EXPECT_NEAR(homography.at<double>(0, 2) - testHomography.at<double>(0, 2), 0, 0.6);
			}
		};

		/**
		 * Test for the stabilizer based on the SURF feature detector and Brute force matcher.
		 */
		TEST_F(DynamicStabilizationTests, testSURFBruteForceStabilizationRuns) {
			assertStabilization(new providentia::stabilization::SURFBFDynamicStabilization());
		}

		/**
		 * Test for the stabilizer based on the ORB feature detector and Brute force matcher.
		 */
		TEST_F(DynamicStabilizationTests, testORBBruteForceStabilizationRuns) {
			assertStabilization(new providentia::stabilization::ORBBFDynamicStabilization());
		}

		/**
		 * Test for the stabilizer based on the Fast feature detector, FREAK feature descriptors and Brute force matcher.
		 */
		TEST_F(DynamicStabilizationTests, testFastFREAKBruteForceStabilizationRuns) {
			assertStabilization(new providentia::stabilization::FastFREAKBFDynamicStabilization());
		}
	}
}