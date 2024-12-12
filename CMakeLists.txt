cmake_minimum_required(VERSION 3.10)

# Project name
project(YoloV8)

# CXX set
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

# set target os and architecture
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# target environment
set(CMAKE_SYSROOT "/opt/poky/5.0.5/sysroots/cortexa76-poky-linux")
set(CMAKE_FIND_ROOT_PATH "/opt/poky/5.0.5/sysroots/cortexa76-poky-linux")

# search for program in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# tool-chain
set(BUILD_PATH "/opt/poky/5.0.5/sysroots/x86_64-pokysdk-linux")
set(CMAKE_TOOLCHAIN_FILE "${BUILD_PATH}/usr/bin")
set(CMAKE_C_COMPILER "${BUILD_PATH}/usr/bin/aarch64-poky-linux/aarch64-poky-linux-gcc")
set(CMAKE_CXX_COMPILER "${BUILD_PATH}/usr/bin/aarch64-poky-linux/aarch64-poky-linux-g++")

# compiler flags
set(CMAKE_C_FLAGS 
    "-mcpu=cortex-a76 -march=armv8.2-a+crypto -mbranch-protection=standard -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security")
set(CMAKE_CXX_FLAGS 
    "-mcpu=cortex-a76 -march=armv8.2-a+crypto -mbranch-protection=standard -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security")

# set Debug Flag
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Debug)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/Debug)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/Debug)
else()
    add_compile_options(-O3)
    add_link_options(-s)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Release)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/Release)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/Release)
endif()

# default library path
set(DEFAULT_LIB_INC "${CMAKE_SYSROOT}/usr/include")
set(DEFAULT_LIB_LIB "${CMAKE_SYSROOT}/usr/lib")

link_directories(${DEFAULT_LIB_LIB})
include_directories(${DEFAULT_LIB_INC})

# opencv path
set(OPENCV_LIB_INC "${CMAKE_SYSROOT}/usr/include/opencv4")

# Eigen path
set(EIGEN_LIB_INC "${CMAKE_SYSROOT}/usr/include/eigen3")
# Global compiler options
add_compile_options(-Wall -Wno-unknown-pragmas -fPIE)

# Include directories (add paths here as needed)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Make library
add_library(program 
    SHARED 
    src/yoloV8.cpp src/BYTETracker.cpp src/kalmanFilter.cpp src/lapjv.cpp 
    src/STrack.cpp src/utils.cpp src/yolov8main.cpp
)

# libncnn.a vs ncnn
target_link_libraries(
    program 
    ncnn 
    -lopencv_gapi -lopencv_stitching -lopencv_ts -lopencv_alphamat -lopencv_aruco
    -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_fuzzy -lopencv_hfs
    -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_quality
    -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo
    -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow
    -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_plot
    -lopencv_videostab -lopencv_videoio -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc
    -lopencv_video -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_xphoto
    -lopencv_photo -lopencv_imgproc -lopencv_core
    gomp
)

target_include_directories(
    program 
    PRIVATE
    "${CMAKE_SOURCE_DIR}/include" 
    ${EIGEN_LIB_INC}
    ${DEFAULT_LIB_INC} 
    ${OPENCV_LIB_INC}
)

target_compile_options(program PRIVATE -fPIC)
