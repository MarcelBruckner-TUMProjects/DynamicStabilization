//
// Created by brucknem on 12.01.21.
//

#include "FeatureMatchingBase.hpp"

namespace providentia {
	namespace stabilization {
		namespace matching {

			void FeatureMatchingBase::match(
				const std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> &frameFeatureDetector,
				const std::shared_ptr<providentia::stabilization::detection::FeatureDetectionBase> &referenceFeatureDetector
			) {
				clear();
				frameDetector = frameFeatureDetector;
				referenceFrameDetector = referenceFeatureDetector;

				if (frameDetector->getKeypoints().empty() || referenceFrameDetector->getKeypoints().empty()) {
					addTimestamp("Matching finished", 0);
					return;
				}

				specificMatch();

				//-- Filter matches using the Lowe's ratio toCameraSpace
				goodMatches.clear();

				for (auto &knnMatchCPU : knnMatchesCPU) {
					if (knnMatchCPU[0].distance < goodMatchRatioThreshold * knnMatchCPU[1].distance) {
						goodMatches.push_back(knnMatchCPU[0]);
					}
				}

				//-- Localize the object
				frameMatchedPoints.clear();
				referenceMatchedPoints.clear();

				for (auto &goodMatch : goodMatches) {
					//-- Get the keypoints from the good matches
					frameMatchedPoints.push_back(frameDetector->getKeypoints()[goodMatch.queryIdx].pt);
					referenceMatchedPoints.push_back(referenceFrameDetector->getKeypoints()[goodMatch.trainIdx].pt);
				}

				if (shouldUseFundamentalMatrix) {
					filterUsingFundamentalMatrix();
				} else {
					fundamentalMatches = goodMatches;
				}

				addTimestamp("Matching finished", 0);
			}

			void FeatureMatchingBase::filterUsingFundamentalMatrix() {
				try {
					fundamentalMatrix = cv::findFundamentalMat(frameMatchedPoints, referenceMatchedPoints,
															   cv::FM_RANSAC, 1.0, 0.975, fundamentalMatrixInlierMask);
				} catch (const std::exception &e) {
					std::cout << "Error in fundamental matrix calculation: " << e.what() << std::endl;
					return;
				}
				fundamentalMatches.clear();
				for (int i = 0; i < goodMatches.size(); i++) {
					if (fundamentalMatrixInlierMask.at<bool>(i, 0)) {
						fundamentalMatches.push_back(goodMatches[i]);
					}
				}

				//-- Localize the object
				frameMatchedPoints.clear();
				referenceMatchedPoints.clear();
				for (auto &goodMatch : fundamentalMatches) {
					//-- Get the keypoints from the good matches
					frameMatchedPoints.push_back(frameDetector->getKeypoints()[goodMatch.queryIdx].pt);
					referenceMatchedPoints.push_back(referenceFrameDetector->getKeypoints()[goodMatch.trainIdx].pt);
				}
			}

			cv::Mat FeatureMatchingBase::draw() {
				drawMatches(
					cv::Mat(frameDetector->getOriginalFrame()), frameDetector->getKeypoints(),
					cv::Mat(referenceFrameDetector->getOriginalFrame()), referenceFrameDetector->getKeypoints(),
					fundamentalMatches, drawFrame, cv::Scalar::all(-1),
					cv::Scalar::all(-1), std::vector<char>(),
					cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
				return drawFrame;
			}

			const std::vector<cv::DMatch> &FeatureMatchingBase::getGoodMatches() const {
				return goodMatches;
			}

			const std::vector<cv::Point2f> &FeatureMatchingBase::getFrameMatchedPoints() const {
				return frameMatchedPoints;
			}

			const std::vector<cv::Point2f> &FeatureMatchingBase::getReferenceMatchedPoints() {
				return referenceMatchedPoints;
			}

			FeatureMatchingBase::FeatureMatchingBase(float goodMatchRatioThreshold)
				: providentia::utils::TimeMeasurable("FeatureMatchingBase", 1), goodMatchRatioThreshold(
				goodMatchRatioThreshold) {
			}

			void FeatureMatchingBase::setShouldUseFundamentalMatrix(bool value) {
				shouldUseFundamentalMatrix = value;
			}

		}
	}
}