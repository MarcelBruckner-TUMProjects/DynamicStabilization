//
// Created by brucknem on 30.03.21.
//

#ifndef CAMERASTABILIZATION_OBJECTTRACKING_HPP
#define CAMERASTABILIZATION_OBJECTTRACKING_HPP

#include <string>
#include <opencv2/opencv.hpp>
#include <utility>
#include "Commons.hpp"
#include "CSVWriter.hpp"

namespace providentia {
	namespace evaluation {
		class CSVWriter;

		class TrackerWrapper {
		private:
			const static std::string TRACKER_TYPES[8];

			cv::Ptr<cv::Tracker> tracker;
			cv::Rect bbox;
			bool trackingSuccessful = false;
			cv::Scalar color;
			bool frameBorderReached = false;
			bool fullyEnteredImage = false;

		public:
			TrackerWrapper(int trackerType, const cv::Rect2d &bbox, cv::Scalar color);

			virtual ~TrackerWrapper() = default;

			void init(const cv::Mat &frame);

			bool empty();

			void track(const cv::Mat &frame);

			cv::Point2d getLowerMidpoint() const;

			cv::Point2d getMidpoint() const;

			cv::Mat draw(const cv::Mat &frame);

			bool isTrackingSuccessful() const;

			cv::Rect getBbox() const;

			void toCSV(CSVWriter *csvWriter);

			static void addHeader(CSVWriter *csvWriter, const std::string &name);

			friend TrackerWrapper operator*(const TrackerWrapper &tracker, cv::Mat homography);

			bool isFrameBorderReached() const;

			void setFrameBorderReached(bool frameBorderReached);

			bool isBboxInImage(const cv::Mat &frame, float percentage) const;
		};

		class ObjectTracking {
		private:
			std::vector<TrackerWrapper> trackers;

		public:
			explicit ObjectTracking();

			virtual ~ObjectTracking() = default;

			void addTracker(cv::Mat frame, int trackerType, const cv::Rect2d &bbox, cv::Scalar color);

			void addTracker(TrackerWrapper tracker);;

			cv::Mat track(const cv::Mat &frame);

			bool isInitialized(int i);

			cv::Point2d getLowerMidpoint(int i) const;

			cv::Point2d getMidpoint(int i) const;

			std::vector<cv::Point2d> getMidpoints() const;

			cv::Mat draw(const cv::Mat &frame);

			bool isTrackingSuccessful(int i) const;

			cv::Rect getBbox(int i) const;

			friend ObjectTracking operator*(const ObjectTracking &tracking, const cv::Mat &homography);

			const std::vector<TrackerWrapper> &getTrackers() const;

			bool isTrackingLost() const;

			void resetTracking();
		};
	}
}

#endif //CAMERASTABILIZATION_OBJECTTRACKING_HPP
