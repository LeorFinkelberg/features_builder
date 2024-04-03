cmake_minimum_required (VERSION 3.1)
project (FeatureBuilder)

find_package(SCIP CONFIG REQUIRED)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
set(CMAKE_CXX_STANDARD 11)

include_directories(${SCIP_INCLUDE_DIRS})

add_executable(FeatureBuilder feature_builder.cpp)

target_link_libraries(FeatureBuilder ${SCIP_LIBRARIES})