########################################################################################################################
### Library ###
########################################################################################################################

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

########################################################################################################################
### Dynamic Stabilization ###
########################################################################################################################
add_library(DynamicStabilization
        detection/FeatureDetectionBase.cpp
        detection/SURFFeatureDetection.cpp
        detection/ORBFeatureDetection.cpp
        detection/FastFREAKFeatureDetection.cpp
        detection/StarBRIEFFeatureDetection.cpp
        detection/SIFTFeatureDetection.cpp
        DynamicStabilizationBase.cpp
        SURFBFDynamicStabilization.cpp
        ORBBFDynamicStabilization.cpp
        FastFREAKBFDynamicStabilization.cpp
        matching/FeatureMatchingBase.cpp
        matching/BruteForceFeatureMatching.cpp
        matching/FlannFeatureMatching.cpp
        segmentation/MOG2BackgroundSegmentation.cpp
        segmentation/BackgroundSegmentionBase.cpp
        warping/FrameWarping.cpp
        TimeMeasurable.cpp
        )

target_include_directories(DynamicStabilization
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(DynamicStabilization
        ${OpenCV_LIBS}
        )
