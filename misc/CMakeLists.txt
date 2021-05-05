cmake_minimum_required(VERSION 3.10)

set(IMAGES
        physical_camera_sensor_highway.png
        test_frame.png
        )

file(COPY ${IMAGES} DESTINATION ./)
