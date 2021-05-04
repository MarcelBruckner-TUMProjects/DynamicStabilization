//
// Created by brucknem on 12.01.21.
//
#include "gtest/gtest.h"
#include "ImageTestBase.hpp"
#include "detection/SURFFeatureDetection.hpp"
#include "matching/BruteForceFeatureMatching.hpp"
#include "matching/FlannFeatureMatching.hpp"
#include <iostream>

namespace providentia {
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
			void assertMatcher(providentia::stabilization::matching::FeatureMatchingBase *matcher) {
				auto options = providentia::stabilization::detection::SURFFeatureDetection::Options();
				options.hessianThreshold = 1000;
				auto detector = std::make_shared<providentia::stabilization::detection::SURFFeatureDetection>(options);
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
			assertMatcher(new providentia::stabilization::matching::BruteForceFeatureMatching(cv::NORM_L2));
		}

		/**
		 * Tests the Flann feature matcher
		 */
		TEST_F(FeatureMatcherTests, testFlannFeatureMatcherRuns) {
			assertMatcher(new providentia::stabilization::matching::FlannFeatureMatching());
		}
	}
}