cmake_minimum_required(VERSION 2.4.8)
project(camera)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Release)
set(OpenCV_DIR "/usr/local/opencv2.4.13")
find_package( OpenCV REQUIRED )

INCLUDE_DIRECTORIES(${OPENCV_INCLUDE_DIRS})

set(SOURCE_FILES camera.cpp)
add_executable(camera ${SOURCE_FILES})
target_link_libraries(camera ${OpenCV_LIBS})
add_executable(camera_two camera_two.cpp)
target_link_libraries(camera_two ${OpenCV_LIBS})
add_executable(camera_mono camera_mono.cpp)
target_link_libraries(camera_mono ${OpenCV_LIBS})
add_executable(camera_orb camera_orb.cpp)
target_link_libraries(camera_orb ${OpenCV_LIBS})
