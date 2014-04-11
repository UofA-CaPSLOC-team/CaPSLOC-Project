#####################################
cmake_minimum_required (VERSION 2.8)
SET(raspicam_DIR /usr/local/lib/cmake)
project (simpletest_raspicam)
find_package(raspicam REQUIRED)
add_executable (simpletest_raspicam simpletest_raspicam.cpp)
target_link_libraries (simpletest_raspicam ${raspicam_LIBS})
#####################################
