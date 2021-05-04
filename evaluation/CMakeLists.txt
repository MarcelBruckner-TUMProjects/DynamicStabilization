cmake_minimum_required(VERSION 3.10)

########################################################################################################################
### Packages ###
########################################################################################################################
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options REQUIRED filesystem REQUIRED date_time)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)


########################################################################################################################
### Utils lib ###
########################################################################################################################

add_library(Utils OpticalFlow.cpp CSVWriter.cpp ObjectTracking.cpp Commons.cpp Color.cpp)

target_include_directories(Utils
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        )

target_link_libraries(Utils PUBLIC ${OpenCV_LIBS} ${Boost_LIBRARIES} DynamicStabilization)


########################################################################################################################
### Visualize ###
########################################################################################################################

add_executable(VisualizeFeatureDetection VisualizeFeatureDetection.cpp)
target_link_libraries(VisualizeFeatureDetection DynamicStabilization  Utils ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(VisualizeFeatureDetection PUBLIC "${PROJECT_BINARY_DIR}")

add_executable(VisualizeFeatureMatching VisualizeFeatureMatching.cpp)
target_link_libraries(VisualizeFeatureMatching DynamicStabilization  Utils ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(VisualizeFeatureMatching PUBLIC "${PROJECT_BINARY_DIR}")

add_executable(VisualizeBackgroundSegmentation VisualizeBackgroundSegmentation.cpp)
target_link_libraries(VisualizeBackgroundSegmentation DynamicStabilization  Utils ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(VisualizeBackgroundSegmentation PUBLIC "${PROJECT_BINARY_DIR}")

########################################################################################################################
### Evaluation ###
########################################################################################################################

add_executable(EvaluateOpticalFlow EvaluateOpticalFlow.cpp)
target_link_libraries(EvaluateOpticalFlow DynamicStabilization  Utils ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(EvaluateOpticalFlow PUBLIC "${PROJECT_BINARY_DIR}")

add_executable(EvaluateObjectTracking EvaluateObjectTracking.cpp)
target_link_libraries(EvaluateObjectTracking DynamicStabilization  Utils ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_include_directories(EvaluateObjectTracking PUBLIC "${PROJECT_BINARY_DIR}")
