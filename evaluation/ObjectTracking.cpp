//
// Created by brucknem on 30.03.21.
//

#include "ObjectTracking.hpp"
#include <opencv2/tracking.hpp>
#include <utility>

namespace providentia {
	namespace evaluation {
		const std::string TrackerWrapper::TRACKER_TYPES[] = {"MIL", "KCF", "CSRT"};

		cv::Ptr<cv::Tracker> getTracker(const std::string &trackerType) {
			if (trackerType == "MIL")
				return cv::TrackerMIL::create();
			if (trackerType == "KCF") {
				return cv::TrackerKCF::create();
			}
			return cv::TrackerCSRT::create();
		}

		ObjectTracking::ObjectTracking() = default;

		void ObjectTracking::addTracker(cv::Mat frame, int trackerType, const cv::Rect2d &bbox, cv::Scalar color) {
			trackers.emplace_back(
				trackerType,
				bbox,
				color
			);
			trackers.back().init(frame);
		}

		bool ObjectTracking::isInitialized(int i) {
			return !trackers[i].empty();
		}

		cv::Point2d ObjectTracking::getLowerMidpoint(int i) const {
			auto bbox = trackers[i].getBbox();
			return {bbox.x + 0.5 * bbox.width, static_cast<double>(bbox.y + bbox.height)};
		}

		cv::Point2d ObjectTracking::getMidpoint(int i) const {
			return trackers[i].getMidpoint();
		}

		cv::Mat ObjectTracking::track(const cv::Mat &frame) {
			for (auto &tracker :trackers) {
				tracker.track(frame);
			}
			return draw(frame);
		}

		cv::Rect ObjectTracking::getBbox(int i) const {
			return trackers[i].getBbox();
		}

		bool ObjectTracking::isTrackingSuccessful(int i) const {
			return trackers[i].isTrackingSuccessful();
		}

		cv::Mat ObjectTracking::draw(const cv::Mat &frame) {
			cv::Mat result = frame.clone();

			for (auto &tracker :trackers) {
				result = tracker.draw(result);
			}

			return result;
		}

		std::vector<cv::Point2d> ObjectTracking::getMidpoints() const {
			std::vector<cv::Point2d> result;
			for (auto &tracker:trackers) {
				result.emplace_back(tracker.getMidpoint());
			}
			return result;
		}

		void ObjectTracking::addTracker(TrackerWrapper tracker) {
			trackers.emplace_back(tracker);
		}

		ObjectTracking operator*(const ObjectTracking &tracking, const cv::Mat &homography) {
			ObjectTracking warped{tracking};

			std::vector<TrackerWrapper> warpedTrackers;
			for (const auto &tracker : warped.trackers) {
				warpedTrackers.emplace_back(tracker * homography);
			}
			warped.trackers = warpedTrackers;
			return warped;
		}

		const std::vector<TrackerWrapper> &ObjectTracking::getTrackers() const {
			return trackers;
		}

		bool ObjectTracking::isTrackingLost() const {
			for (const auto &tracker :trackers) {
				if (!tracker.isFrameBorderReached()) {
					return false;
				}
			}
			return true;
		}

		void ObjectTracking::resetTracking() {
			for (auto &tracker :trackers) {
				tracker.setFrameBorderReached(false);
			}
		}

		TrackerWrapper::TrackerWrapper(int trackerType, const cv::Rect2d &bbox, cv::Scalar color) :
			tracker(getTracker(TRACKER_TYPES[trackerType])), bbox(bbox),
			color(std::move(color)) {}

		void TrackerWrapper::init(const cv::Mat &frame) {
			tracker->init(frame, bbox);
		}

		bool TrackerWrapper::empty() {
			return bbox.empty();
		}

		void TrackerWrapper::track(const cv::Mat &frame) {
			if (frameBorderReached) {
				return;
			}
			trackingSuccessful = tracker->update(frame, bbox);
			fullyEnteredImage = isBboxInImage(frame, 0.025) || fullyEnteredImage;
			frameBorderReached = !isBboxInImage(frame, 0.015) && fullyEnteredImage;
		}

		bool TrackerWrapper::isBboxInImage(const cv::Mat &frame, float percentage) const {
			float xp = frame.cols * percentage;
			float yp = frame.rows * percentage;
			return !(bbox.x < xp ||
					 bbox.y < yp ||
					 bbox.x + bbox.width > frame.cols - xp ||
					 bbox.y + bbox.height > frame.rows - yp);
		}

		cv::Point2d TrackerWrapper::getLowerMidpoint() const {
			if (!trackingSuccessful || frameBorderReached || !fullyEnteredImage) {
				return {-1, -1};
			}
			return {bbox.x + 0.5 * bbox.width, static_cast<double>(bbox.y + bbox.height)};
		}

		cv::Point2d TrackerWrapper::getMidpoint() const {
			if (!trackingSuccessful || frameBorderReached || !fullyEnteredImage) {
				return {-1, -1};
			}
			return {bbox.x + 0.5 * bbox.width, bbox.y + 0.5 * bbox.height};
		}

		cv::Mat TrackerWrapper::draw(const cv::Mat &frame) {
			cv::Mat result = frame.clone();

			if (!empty() && isTrackingSuccessful()) {
				cv::rectangle(result, getBbox(), color, 8, 1);

				cv::Point2d midpoint = getMidpoint();
				cv::circle(result, midpoint, 3, color, -1);
			}
			return result;
		}

		bool TrackerWrapper::isTrackingSuccessful() const {
			return trackingSuccessful;
		}

		cv::Rect TrackerWrapper::getBbox() const {
			if (!trackingSuccessful || frameBorderReached || !fullyEnteredImage) {
				return {-1, -1, -1, -1};
			}
			return bbox;
		}

		void TrackerWrapper::toCSV(CSVWriter *csvWriter) {
			*csvWriter << getBbox() << getMidpoint();
		}

		void TrackerWrapper::addHeader(CSVWriter *csvWriter, const std::string &name) {
			*csvWriter << name + " [x]"
					   << name + " [y]"
					   << name + " [w]"
					   << name + " [h]"
					   << name + " [mx]"
					   << name + " [my]";
		}

		TrackerWrapper operator*(const TrackerWrapper &tracker, cv::Mat homography) {
			TrackerWrapper warped{tracker};

			cv::Vec3d topLeft = {(double) warped.bbox.x, (double) warped.bbox.y, 1};
			cv::Vec3d lowerRight = {topLeft(0) + warped.bbox.width, topLeft(1) + warped.bbox.height, 1};

			topLeft = cv::Mat(homography * topLeft).col(0);
			lowerRight = cv::Mat(homography * lowerRight).col(0);

			topLeft = topLeft / topLeft(2);
			lowerRight = lowerRight / lowerRight(2);

			warped.bbox = cv::Rect(
				cv::Point2d{topLeft(0), topLeft(1)},
				cv::Point2d{lowerRight(0), lowerRight(1)}
			);
			return warped;
		}

		bool TrackerWrapper::isFrameBorderReached() const {
			return frameBorderReached;
		}

		void TrackerWrapper::setFrameBorderReached(bool _frameBorderReached) {
			frameBorderReached = _frameBorderReached;
		}
	}
}