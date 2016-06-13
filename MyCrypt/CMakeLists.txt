cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(PROJECT_NAME MyCrypt)
message(STATUS "************  ${PROJECT_NAME} ************")
project(${PROJECT_NAME})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PORT_TYPE POSIX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(PORT_TYPE WIN)
	add_definitions( /wd4996 )
endif()

file (GLOB SRCS "src/*.c*")
file (GLOB HDRS "include/*.h")


include_directories(include)
include_directories(../gsi/include)
include_directories(../gri/include)
include_directories(../gsu/include)

add_executable(${PROJECT_NAME} ${HDRS} ${SRCS})

link_directories(${LIBRARY_OUTPUT_PATH})
find_library(GSI gsi PATH lib)
find_library(GRI gri PATH lib)
find_library(GSU gsu PATH lib)
if(GSI)
    #found
    message(STATUS "Library Not Found")
else()
    #not found
	 message(STATUS "Library ${GSI} Found")
endif()
find_package (Threads)

target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
