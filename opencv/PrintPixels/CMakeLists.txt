cmake_minimum_required(VERSION 2.8)
project( PrintPixels )
find_package( OpenCV REQUIRED )
add_executable( PrintPixels PrintPixels.cpp )


target_link_libraries( PrintPixels opencv_core opencv_imgproc opencv_video )

