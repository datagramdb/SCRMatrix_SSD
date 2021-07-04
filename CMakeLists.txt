cmake_minimum_required(VERSION 3.16)
project(SCRMatrix_SSD)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)
add_library(SCRMatrix_SSD src/matrix_status.cpp
                          include/SCRMatrix_SSD/matrix_status.h
                          src/openCloseOperations.cpp
                          include/SCRMatrix_SSD/openCloseOperations.h
        src/readOperations.cpp
        include/SCRMatrix_SSD/readOperations.h src/writeOperations.cpp include/SCRMatrix_SSD/writeOperations.h include/SCRMatrix_SSD/all.h)
target_link_libraries(SCRMatrix_SSD yaucl_fs yaucl_algorithms)

add_executable(scrm_ssd1 mains/preliminary.cpp)
target_link_libraries(scrm_ssd1 yaucl_fs yaucl_algorithms SCRMatrix_SSD)

add_executable(scrm_ssd2 mains/matrixMultiplication.cpp)
target_link_libraries(scrm_ssd2 yaucl_fs yaucl_algorithms SCRMatrix_SSD)