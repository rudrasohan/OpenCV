cmake_minimum_required(VERSION 2.8)
project( OPENCV )
find_package( OpenCV REQUIRED )
add_executable( canny canny.cpp )
target_link_libraries( canny ${OpenCV_LIBS} )
add_executable( video video.cpp )
target_link_libraries( video ${OpenCV_LIBS} )

