file(GLOB SRCS *.cpp)

include_directories(${CMAKE_BINARY_DIR}/src/)

add_executable(test_parser ${SRCS})
target_link_libraries(test_parser gtest)
