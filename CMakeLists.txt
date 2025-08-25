cmake_minimum_required(VERSION 3.29)
project(Parallel_Orlandi_Francesco)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenMP REQUIRED)
add_executable(Parallel_Orlandi_Francesco main.cpp
        DSequential.h DSequential.cpp
        DParallel.h DParallel.cpp
        PasswordAOS.cpp PasswordSOA.cpp
        Media.h Media.cpp)
target_link_libraries(Parallel_Orlandi_Francesco crypt OpenMP::OpenMP_CXX)