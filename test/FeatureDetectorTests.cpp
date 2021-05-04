//
// Created by brucknem on 12.01.21.
//
#include "gtest/gtest.h"
#include "ImageTestBase.hpp"
#include "DynamicStabilization/detection/FeatureDetectionBase.hpp"
#include "DynamicStabilization/detection/SURFFeatureDetection.hpp"
#include "DynamicStabilization/detection/ORBFeatureDetection.hpp"
#include "DynamicStabilization/detection/FastFREAKFeatureDetection.hpp"
#include "DynamicStabilization/detection/StarBRIEFFeatureDetection.hpp"
#include "DynamicStabilization/detection/SIFTFeatureDetection.hpp"
#include <iostream>

namespace dynamic_stabilization {
	namespace tests {

		/**
		 * Setup for the feature detector toCameraSpace.
		 */
		class FeatureDetectionTests : public ImageTestBase {
		protected:

			/**
			 * @destructor
			 */
			~FeatureDetectionTests() override = default;

			/**
			 * Detected keypoints.
			 */
			std::vector<cv::KeyPoint> keypoints;

			/**
			 * Uses the given detector to detect keypoints and sorts them.
			 *
			 * @param detector The detector to use.
			 * @param nFeatures The number of keypoints to expect.
			 */
			void
			getSortedKeypoints(dynamic_stabilization::stabilization::detection::FeatureDetectionBase *detector, int nFeatures) {
				detector->detect(testImgGPU);
				keypoints = detector->getKeypoints();
				ASSERT_EQ(keypoints.size(), nFeatures);

				std::sort(keypoints.begin(), keypoints.end(), [](const cv::KeyPoint &a, const cv::KeyPoint &b) {
					if (a.pt.x == b.pt.x) {
						return a.pt.y < b.pt.y;
					}
					return a.pt.x < b.pt.x;
				});
			}

			/**
			 * Asserts that the first feature is at the given position with the given maximal error.
			 */
			void assertFirst(float x, float dx, float y, float dy) {
				EXPECT_NEAR(keypoints[0].pt.x, x, dx);
				EXPECT_NEAR(keypoints[0].pt.y, y, dy);
			}

			/**
			 * Asserts that the last feature is at the given position with the given maximal error.
			 */
			void assertLast(float x, float dx, float y, float dy) {
				EXPECT_NEAR(keypoints[keypoints.size() - 1].pt.x, x, dx);
				EXPECT_NEAR(keypoints[keypoints.size() - 1].pt.y, y, dy);
			}
		};

		/**
		 * Tests the SURF feature detector.
		 */
		TEST_F(FeatureDetectionTests, testSURFFeatureDetectionRuns) {
			auto options = dynamic_stabilization::stabilization::detection::SURFFeatureDetection::Options();
			options.hessianThreshold = 1000;
			getSortedKeypoints(new dynamic_stabilization::stabilization::detection::SURFFeatureDetection(options), 3104);

			assertFirst(13.27, 0.01, 387.006, 0.01);
			assertLast(1895.82, 0.01, 1051.98, 0.01);
		}

		/**
		 * Tests the ORB feature detector.
		 */
		TEST_F(FeatureDetectionTests, testORBFeatureDetectionRuns) {
			auto options = dynamic_stabilization::stabilization::detection::ORBFeatureDetection::Options();
			options.nfeatures = 1000;
			getSortedKeypoints(new dynamic_stabilization::stabilization::detection::ORBFeatureDetection(options), 1000);

			assertFirst(112, 0.01, 323, 0.01);
			assertLast(1852.80, 0.01, 378, 0.01);
		}

		/**
		 * Tests the SIFT feature detector.
		 */
		TEST_F(FeatureDetectionTests, testSIFTFeatureDetectionRuns) {
			auto options = dynamic_stabilization::stabilization::detection::SIFTFeatureDetection::Options();
			options.nfeatures = 1000;
			getSortedKeypoints(new dynamic_stabilization::stabilization::detection::SIFTFeatureDetection(options), 1000);

			assertFirst(13.86, 0.01, 387.17, 0.01);
			assertLast(1886.65, 0.01, 1047.14, 0.01);
		}

		/**
		 * Tests the fast feature detector with FREAK descriptors.
		 */
		TEST_F(FeatureDetectionTests, testFastFREAKFeatureDetectionRuns) {
			getSortedKeypoints(new dynamic_stabilization::stabilization::detection::FastFREAKFeatureDetection(), 5781);

			assertFirst(24, 0.01, 497, 0.01);
			assertLast(1895, 0.01, 308, 0.01);
		}

		/**
		 * Tests the Star feature detector with BRIEF descriptors.
		 */
		TEST_F(FeatureDetectionTests, testStarBRIEFFeatureDetectionRuns) {
			auto options = dynamic_stabilization::stabilization::detection::StarBRIEFFeatureDetection::Options();
			options.maxSize = 1000;
			getSortedKeypoints(new dynamic_stabilization::stabilization::detection::StarBRIEFFeatureDetection(options), 1513);

			assertFirst(192, 0.01, 281, 0.01);
			assertLast(1727, 0.01, 921, 0.01);
		}
	}
}