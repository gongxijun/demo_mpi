cmake_minimum_required(VERSION 3.0)

project(main)

find_package(MPI REQUIRED)
# add this line only when you are using openmpi which has a different c++ bindings
#add_definitions(-DOMPI_SKIP_MPICXX)

set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(MPI_INCLUDE_PATH)
message( ${MPI_INCLUDE_PATH})
# Use imported targets would make things much eazier. Thanks Levi for pointing it out.
add_executable(shear_sort shear_sort.cc)
target_link_libraries(shear_sort
        ${MPI_LIBRARIES})