cmake_minimum_required(VERSION 3.2)

project(ffmegH264RtspServer)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-D__STDC_CONSTANT_MACROS)

message(STATUS "Home is $ENV{HOME}")

if (NOT DEFINED THIRD_PARTY_ROOT)
    if (DEFINED ENV{THIRD_PARTY_ROOT})
        set(THIRD_PARTY_ROOT $ENV{THIRD_PARTY_ROOT})
    else ()
        set(THIRD_PARTY_ROOT $ENV{THIRD_PARTY_ROOT}/main/third_party)
    endif ()
endif ()

set(Live555_ROOT ${THIRD_PARTY_ROOT}/live555)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LIB_ARCH_NAME x64-ubuntu18-gcc)
    set(IS_LINUX 1)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(LIB_ARCH_NAME mac)
    set(IS_MAC 1)
endif ()

message(STATUS "live555_root=${Live555_ROOT}")

include_directories(${Live555_ROOT}/BasicUsageEnvironment/include
        ${Live555_ROOT}/UsageEnvironment/include
        ${Live555_ROOT}/liveMedia/include
        ${Live555_ROOT}/groupsock/include
        ${Live555_ROOT}/include
        /usr/local/include
        )

#link_libraries( ${OpenCV})

link_directories(
        ${Live555_ROOT}/${LIB_ARCH_NAME}/lib
        /usr/local/lib
)

add_executable(ffmpegH264RtspServer
        CameraDevice.cpp
        FfmpegEncoder.cpp
        FfmpegFrameSource.cpp
        FfmpegH264LiveServerMediaSubsession.cpp
        UsbCameraOnDemandRTSPServer.cpp
        main.cpp
        )


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
        pthread
        )


