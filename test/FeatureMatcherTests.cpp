//
// Created by brucknem on 12.01.21.
//
#include "gtest/gtest.h"
#include "ImageTestBase.hpp"
#include "DynamicStabilization/detection/SURFFeatureDetection.hpp"
#include "DynamicStabilization/matching/BruteForceFeatureMatching.hpp"
#include "DynamicStabilization/matching/FlannFeatureMatching.hpp"
#include <iostream>

namespace dynamic_stabilization {
	namespace tests {

		/**
		 * Setup for the feature matcher toCameraSpace.
		 */
		class FeatureMatcherTests : public ImageTestBase {
		public:

			/**
			 * @destructor
			 */
			~FeatureMatcherTests() override = default;

			/**
			 * Asserts that the given matcher can match two identical sets of features perfectly.
			 */
			void assertMatcher(dynamic_stabilization::stabilization::matching::FeatureMatchingBase *matcher) {
				auto options = dynamic_stabilization::stabilization::detection::SURFFeatureDetection::Options();
				options.hessianThreshold = 1000;
				auto detector = std::make_shared<dynamic_stabilization::stabilization::detection::SURFFeatureDetection>(options);
				detector->detect(testImgGPU);

				matcher->match(detector, detector);

				std::vector<cv::DMatch> goodMatches = matcher->getGoodMatches();

				for (const auto &goodMatch : goodMatches) {
					ASSERT_EQ(goodMatch.distance, 0);
					ASSERT_EQ(goodMatch.trainIdx, goodMatch.queryIdx);
				}
			}
		};

		/**
		 * Tests the Brute Force feature matcher.
		 */
		TEST_F(FeatureMatcherTests, testBruteForceFeatureMatcherRuns) {
			assertMatcher(new dynamic_stabilization::stabilization::matching::BruteForceFeatureMatching(cv::NORM_L2));
		}

		/**
		 * Tests the Flann feature matcher
		 */
		TEST_F(FeatureMatcherTests, testFlannFeatureMatcherRuns) {
			assertMatcher(new dynamic_stabilization::stabilization::matching::FlannFeatureMatching());
		}
	}
}