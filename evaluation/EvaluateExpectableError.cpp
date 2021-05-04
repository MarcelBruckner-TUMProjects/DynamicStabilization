//
// Created by brucknem on 02.02.21.
//

#include "Commons.hpp"
#include "Eigen/Dense"
#include "camera/RenderingPipeline.hpp"
#include "objects/WorldObject.hpp"

using namespace providentia::evaluation;

/**
 * Setup to visualize the rendering pipeline.
 */
class Setup : public providentia::evaluation::ImageSetup {
public:
	/**
	 * The itnrinsics of the pinhole camera model.
	 */
	std::vector<double> intrinsics = providentia::camera::getS40NCamFarIntrinsics();

	/**
	 * The [width, height] of the image.
	 */
	Eigen::Vector2d imageSize{1920, 1200};

	int maxDistance = 2000;

	/**
	 * Some [x, y, z] translation of the camera in world space.
	 */
	Eigen::Vector3i translation{maxDistance / 2., maxDistance / 2., maxDistance / 2.};

	/**
	 * Some [x, y, z] euler angle rotation of the camera around the world axis
	 */
	Eigen::Vector3i rotation{900, 0, 0};

	double scalingFactor = 20.;
	int originalDistance = (int) (400. * scalingFactor);
	Eigen::Vector3i pointToMove{maxDistance / 2., maxDistance / 2., maxDistance / 2.};

	std::vector<std::string> pointToMoveTrackbarName{"[Point] X", "[Point] Y", "[Point] Z"};
	std::vector<std::string> translationTrackbarName{"[Translation] X", "[Translation] Y", "[Translation] Z"};
	std::vector<std::string> rotationTrackbarName{"[Rotation] X", "[Rotation] Y", "[Rotation] Z"};

	Eigen::Vector2d actualPixel;

	explicit Setup() : ImageSetup() {}

	Eigen::Matrix<double, 2, 1> render(Eigen::Vector3d vector, const cv::Vec3d &color) {
		return render(vector.x(), vector.y(), vector.z(), color);
	}

	Eigen::Matrix<double, 2, 1> render(double x, double y, double z, const cv::Vec3d &color) {
		return providentia::camera::render(getActualPoint(translation).cast<double>(), rotation.cast<double>() / 10.,
										   intrinsics, Eigen::Vector4d(x, y, z, 1), color,
										   finalFrame);
	}

	Eigen::Vector3d getActualPoint(const Eigen::Vector3i &point) const {
		Eigen::Vector3d result =
			(point.cast<double>() -
			 Eigen::Vector3d{maxDistance / 2., maxDistance / 2., maxDistance / 2.}).cast<double>() /
			scalingFactor;
//		result.y() *= 20.;
		return result;
	}

	void init() override {
		ImageSetup::init();

		for (int i = 0; i < pointToMoveTrackbarName.size(); i++) {
			cv::createTrackbar(pointToMoveTrackbarName[i], windowName, &pointToMove.data()[i], maxDistance);
		}

		for (int i = 0; i < translationTrackbarName.size(); i++) {
			cv::createTrackbar(translationTrackbarName[i], windowName, &translation.data()[i], maxDistance);
		}

		for (int i = 0; i < rotationTrackbarName.size(); i++) {
			cv::createTrackbar(rotationTrackbarName[i], windowName, &rotation.data()[i], 3600);
		}
	}

	void specificMainLoop() override {
		finalFrame = cv::Mat::zeros(cv::Size(imageSize[0], imageSize[1]), CV_64FC4);
		actualPixel = render(getActualPoint(pointToMove + Eigen::Vector3i{0, originalDistance, 0}), {255, 255, 255});
	}

protected:
	void specificAddMessages() override {
		int i = 0;
		int offset = 5;
		int x = 5;
		std::string unit = "m";
		Eigen::Vector3d p = getActualPoint(pointToMove + Eigen::Vector3i{0, originalDistance, 0});
		addText(finalFrame, std::to_string(p.x()) + " " + unit, 2, x, offset + i++ * 40);
		addText(finalFrame, std::to_string(p.y()) + " " + unit, 2, x, offset + i++ * 40);
		addText(finalFrame, std::to_string(p.z()) + " " + unit, 2, x, offset + i++ * 40);

		i = 1;
		offset = 5;
		x = 400;
		addText(finalFrame, "=>", 2, x, offset + i++ * 40);

		i = 0;
		offset = 25;
		x = 500;
		unit = "px";
		addText(finalFrame, std::to_string(std::round(actualPixel.x())) + " " + unit, 2, x, offset + i++ * 40);
		addText(finalFrame, std::to_string(std::round(actualPixel.y())) + " " + unit, 2, x, offset + i++ * 40);

		i = 3;
		offset = 15;
		x = 5;
		unit = "m";
		addText(finalFrame, "Camera", 2, x, offset + i++ * 40);

		addText(finalFrame, "- Translation", 2, x, offset + i++ * 40);
		auto actualTranslation = getActualPoint(translation);
		addText(finalFrame, std::to_string(actualTranslation.x()) + " " + unit, 2, x, offset + i++ * 40);
		addText(finalFrame, std::to_string(actualTranslation.y()) + " " + unit, 2, x, offset + i++ * 40);
		addText(finalFrame, std::to_string(actualTranslation.z()) + " " + unit, 2, x, offset + i++ * 40);

		unit = "deg";
		addText(finalFrame, "- Rotation", 2, x, offset + i++ * 40);
		auto actualRotation = rotation.cast<double>() / 10.;
		addText(finalFrame, std::to_string(actualRotation.x()) + " " + unit, 2, x, offset + i++ * 40);
		addText(finalFrame, std::to_string(actualRotation.y()) + " " + unit, 2, x, offset + i++ * 40);
		addText(finalFrame, std::to_string(actualRotation.z()) + " " + unit, 2, x, offset + i++ * 40);

	}
};

int main(int argc, char const *argv[]) {
	Setup setup;
	setup.setRenderingScaleFactor(1);
	setup.init();
	setup.mainLoop();
	return 0;
}
