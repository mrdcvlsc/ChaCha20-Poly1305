cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCES "*.cpp")

if(WIN32)
    # disable windows asan for now since I don't know how to make it work
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /fsanitize=address")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)

enable_testing()

add_compile_definitions(_HIDE_WARNING)

foreach(test_src_code ${SOURCES})
    get_filename_component(test_exec_name ${test_src_code} NAME_WE)
    add_executable(${test_exec_name} ${test_src_code})
    target_compile_features(${test_exec_name} PRIVATE cxx_std_20)

    add_test(
        NAME ${test_exec_name}
        COMMAND $<TARGET_FILE:${test_exec_name}>
    )
endforeach(test_src_code ${SOURCES})
