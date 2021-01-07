# cmake version to be used
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name
project (Create_raster LANGUAGES CXX)

# include
include_directories(./include/proj ./include/gdal ./src)

# external libraries
set(PROJ_LIB libproj.so)
set(GDAL_LIB libgdal.so)
link_directories(./lib/proj/lib ./lib/gdal/lib)

# internal libraries
add_library(lambert93Proj SHARED ./src/lambert93Proj.cpp ./src/lambert93Proj.h)
add_library(gridCreation SHARED ./src/gridCreation.cpp ./src/gridCreation.h)
add_library(imageCreation SHARED ./src/imageCreation.cpp ./src/imageCreation.h)

# target
add_executable(create_raster ./src/main.cpp)

target_link_libraries(create_raster lambert93Proj gridCreation imageCreation ${GDAL_LIB} ${PROJ_LIB})

