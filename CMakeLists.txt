cmake_minimum_required(VERSION 3.2.2)
project(PA193_test_parser_Ethereum)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
	${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
	include_directories("${gtest_SOURCE_DIR}/include")
endif()


# Project settings

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
		-Wall -Wextra -pedantic -fstack-protector"
	)
endif (CMAKE_COMPILER_IS_GNUCC)

include_directories(${CMAKE_SOURCE_DIR}/src/)

set(SRCS
	${CMAKE_SOURCE_DIR}/src/parser.cpp
	${CMAKE_SOURCE_DIR}/src/block.cpp
	${CMAKE_SOURCE_DIR}/src/rlp.cpp
)

set(TEST_SRCS
	${SRCS}
	${CMAKE_SOURCE_DIR}/test/main.cpp
	${CMAKE_SOURCE_DIR}/test/block.cpp
	${CMAKE_SOURCE_DIR}/test/rlp.cpp
)

set(MAIN ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(ethereum_parser ${MAIN} ${SRCS})

add_executable(test_parser ${TEST_SRCS} ${SRCS})
target_link_libraries(test_parser gtest) 

