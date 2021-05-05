# Automated Camera Stabilization and Calibration for Intelligent Transportation Systems - Dynamic Stabilization

- [See the project website of Providentia++](https://innovation-mobility.com/en)
- [See the overview repository for general information about my Guided Research](https://github.com/Brucknem/GuidedResearch)
- [See the website for information and evaluation results of the project](https://brucknem.github.io/DynamicStabilization)

*** 

# Introduction

The cameras are constantly exposed to wind and vibrations from passing vehicles.
These influences propagate into the video stream and result in jittery motion of the images.
We propose a pipeline to counteract the shaky motions of the cameras using a digital image stabilization approach.
The approach is based on visual image features that are matched between the current and a stable reference frame.
The feature matching is used to minimize the reprojection-error between the frames and results in a homographic transformation.
We use the transformation to align the static backgrounds of the frames, thus mitigating the real-world motion of the camera in image space.

[Please see the report for in depth information about the dynamic stabilization procedure](https://github.com/Brucknem/GuidedResearch/blob/main/report/report.pdf)

***

### Pipeline Overview
![The uncalibrated camera pose.](https://github.com/Brucknem/GuidedResearch/blob/main/report/diagrams/DynamicStabilization_large.png?raw=true)
<p align="center">
      The proposed dynamic stabilization pipeline.<br>
      The input is the video and a stable reference frame.
      The frames are segmented into the foreground and background to extract the static scene.
      On the static scene visual features are detected and the feature descriptors are matched.
      We use the homographic transformation minimizing the reprojection-error based on the matching to warp the video frame.
      The stabilized frame is used to update the background segmentation mask.
</p>


***

# Running in Standalone Mode

This part is currently work in progress and will be updated in the next days.

***

# Dependencies

## External Dependencies

These dependencies have to be installed on your system. Follow their instructions on how to install them.

- [CUDA](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html) - You have to install CUDA and the
  Nvidia drivers according to your GPU.

There is no way to automate this proces as it is dependend on your system.

***

- [CMake](https://cmake.org/) - For building the libraries
- [Ceres Solver](http://ceres-solver.org/) - The non-linear solver for the static calibration
- [OpenCV](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html) - With CUDA support for the dynamic
  stabilization

To facilitate the setup of CMake, Ceres and OpenCV you can use the [setup script](/extern/setup_ceres_opencv.sh).
This should install all necessary dependencies and pull & compile the libraries from source.
This might be outdated by now, but I will update the script in the next days.  
If compile errors arise, the CMake output is a good start to debug.

- [Boost](https://www.boost.org/)

## Internal Dependencies

These dependencies are pulled by CMake when the project is built. You `do not` have to install them manually.

- [GoogleTest](https://github.com/google/googletest)
- [YAML-CPP](https://github.com/jbeder/yaml-cpp.git)
