cmake_minimum_required(VERSION 3.17)
project(spbu_c_homeworks C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_LIB
        library/commonUtils/numericOperations.c
        library/values/values.c library/values/values.h library/list/arrayList.c library/list/arrayList.h)
include_directories(library)
add_library(library ${SOURCE_LIB})
add_executable(example example.c library/values/values.c library/list/arrayList.c)
