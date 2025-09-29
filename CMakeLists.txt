cmake_minimum_required(VERSION 3.2)

project(ffmpegH264RtspServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D__STDC_CONSTANT_MACROS)

set(Live555_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/live)
message(STATUS "live555_root=${Live555_ROOT}")

# Include paths for our app and live555
include_directories(
   ${Live555_ROOT}/BasicUsageEnvironment/include
   ${Live555_ROOT}/UsageEnvironment/include
   ${Live555_ROOT}/liveMedia/include
   ${Live555_ROOT}/groupsock/include
   ${Live555_ROOT}/include
   /usr/local/include
)

# Build live555 libraries from source (Linux: exclude Windows-specific dir)
file(GLOB LIVE_USAGEENVIRONMENT_SRC
   ${Live555_ROOT}/UsageEnvironment/*.cpp
)
add_library(UsageEnvironment STATIC ${LIVE_USAGEENVIRONMENT_SRC})
target_include_directories(UsageEnvironment PUBLIC ${Live555_ROOT}/UsageEnvironment/include)
set_target_properties(UsageEnvironment PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(UsageEnvironment PUBLIC SOCKLEN_T=socklen_t)

file(GLOB LIVE_BASICUSAGEENVIRONMENT_SRC
   ${Live555_ROOT}/BasicUsageEnvironment/*.cpp
)
add_library(BasicUsageEnvironment STATIC ${LIVE_BASICUSAGEENVIRONMENT_SRC})
target_include_directories(BasicUsageEnvironment PUBLIC ${Live555_ROOT}/BasicUsageEnvironment/include)
target_link_libraries(BasicUsageEnvironment PUBLIC UsageEnvironment)
set_target_properties(BasicUsageEnvironment PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(BasicUsageEnvironment PUBLIC SOCKLEN_T=socklen_t)

file(GLOB LIVE_GROUPSOCK_SRC
   ${Live555_ROOT}/groupsock/*.cpp
   ${Live555_ROOT}/groupsock/inet.c
)
add_library(groupsock STATIC ${LIVE_GROUPSOCK_SRC})
target_include_directories(groupsock PUBLIC ${Live555_ROOT}/groupsock/include)
target_link_libraries(groupsock PUBLIC UsageEnvironment)
set_target_properties(groupsock PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(groupsock PUBLIC SOCKLEN_T=socklen_t)

file(GLOB LIVE_LIVEMEDIA_SRC
   ${Live555_ROOT}/liveMedia/*.cpp
   ${Live555_ROOT}/liveMedia/rtcp_from_spec.c
)
add_library(liveMedia STATIC ${LIVE_LIVEMEDIA_SRC})
target_include_directories(liveMedia PUBLIC ${Live555_ROOT}/liveMedia/include)
target_link_libraries(liveMedia PUBLIC groupsock BasicUsageEnvironment UsageEnvironment)
set_target_properties(liveMedia PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(liveMedia PUBLIC SOCKLEN_T=socklen_t)

# Our executable
add_executable(ffmpegH264RtspServer
   CameraDevice.cpp
   FfmpegEncoder.cpp
   FfmpegFrameSource.cpp
   FfmpegH264LiveServerMediaSubsession.cpp
   UsbCameraOnDemandRTSPServer.cpp
   main.cpp
)

# Link order matters for static libs
target_link_libraries(ffmpegH264RtspServer
   liveMedia
   BasicUsageEnvironment
   UsageEnvironment
   groupsock
   avcodec
   avformat
   avutil
   swscale
   avfilter
   avdevice
   ssl
   crypto
   pthread
)
