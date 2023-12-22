enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/catch2" AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/catch2/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/catch2)
elseif (EXISTS "${CMAKE_CURRENT_LIST_DIR}/Catch2" AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/Catch2/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Catch2)
else()
    find_package(Catch2 3 QUIET)
    if(NOT ${Catch2_FOUND})
        MESSAGE(STATUS "Catch2 not found. Skipping tests")
        return()
    endif()
endif()

file(GLOB_RECURSE TEST_SOURCE_FILES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_LIST_DIR} "src/*.cpp")
file(GLOB_RECURSE TEST_HEADER_FILES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_LIST_DIR} "src/*.h")

add_executable(${PROJECT_NAME}-test ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})

target_include_directories(${PROJECT_NAME}-test PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(${PROJECT_NAME}-test PRIVATE ${PROJECT_NAME} Catch2::Catch2)

include(CTest)
include(Catch)

catch_discover_tests(${PROJECT_NAME}-test WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
