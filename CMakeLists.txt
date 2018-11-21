cmake_minimum_required(VERSION 2.8)

project(ffmegH264RtspServer)

add_definitions(-D__STDC_CONSTANT_MACROS -std=c++11)

#find_package(Ffmpeg REQUIRED)
#message(STATUS "Ffmpeg library status:")
#message(STATUS "   Version:${Ffmpeg_VERSION}")
#message(STATUS "   Libs:${Ffmpeg_LIBS}")
#message(STATUS "   Include:${Ffmpeg_INCLUDE_DIRS}")
message(STATUS "Home is $ENV{HOME}")

set(THIRD_PARTY_ROOT /home/hsyuan/work/third_party)
set(Live555_ROOT ${THIRD_PARTY_ROOT}/live555)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(LIB_ARCH_NAME x64-ubuntu18-gcc)
	set(IS_LINUX 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
	set(LIB_ARCH_NAME mac)
	set(IS_MAC 1)
endif()


include_directories(${Live555_ROOT}/BasicUsageEnvironment/include
    ${Live555_ROOT}/UsageEnvironment/include
	${Live555_ROOT}/liveMedia/include
	${Live555_ROOT}/groupsock/include
       )

#link_libraries( ${OpenCV})

link_directories(
	${Live555_ROOT}/${LIB_ARCH_NAME}/lib
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


