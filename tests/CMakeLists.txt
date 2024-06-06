cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCES "*.cpp")

message("=========================")
# message(CMAKE_C_COMPILER="${CMAKE_C_COMPILER}")
# message(CMAKE_CPP_COMPILER="${CMAKE_CPP_COMPILER}")
# message(CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS}")
# message(CMAKE_EXE_LINKER_FLAGS="${CMAKE_EXE_LINKER_FLAGS}")
message("=========================")


set(CMAKE_VERBOSE_MAKEFILE ON)

enable_testing()

add_compile_definitions(_HIDE_WARNING)

foreach(test_src_code ${SOURCES})
    get_filename_component(test_exec_name ${test_src_code} NAME_WE)
    message("Source Code = ${test_src_code} | ${test_exec_name}")
    add_executable(${test_exec_name} ${test_src_code})
    target_compile_features(${test_exec_name} PRIVATE cxx_std_20)
    target_compile_options(${test_exec_name} PRIVATE -fsanitize=address)
    target_link_options(${test_exec_name} PRIVATE -fsanitize=address)

    # message(CMAKE_C_COMPILER="${CMAKE_C_COMPILER}")
    # message(CMAKE_CPP_COMPILER="${CMAKE_CPP_COMPILER}")

    add_test(
        NAME ${test_exec_name}
        COMMAND $<TARGET_FILE:${test_exec_name}>
    )
endforeach(test_src_code ${SOURCES})
