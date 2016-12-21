find_package( OpenCV 2.4.9 REQUIRED core highgui imgproc)

find_package(PkgConfig REQUIRED)
pkg_search_module(EIGEN REQUIRED eigen3)

find_package( GTest REQUIRED )
find_package( Qt5Widgets REQUIRED )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set( external_includes ${EIGEN_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

set( external_libs "${OpenCV_LIBS} -lpthread")


set( external_includes ${Qt5Widgets_INCLUDE_DIRS} )

set( external_libs ${Qt5Widgets_LIBRARIES} "-lpthread")
