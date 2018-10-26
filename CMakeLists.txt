cmake_minimum_required(VERSION 2.8)

project(ffmegH264RtspServer)

add_definitions(-D__STDC_CONSTANT_MACROS -std=c++11)

#find_package(Ffmpeg REQUIRED)
#message(STATUS "Ffmpeg library status:")
#message(STATUS "   Version:${Ffmpeg_VERSION}")
#message(STATUS "   Libs:${Ffmpeg_LIBS}")
#message(STATUS "   Include:${Ffmpeg_INCLUDE_DIRS}")
message(STATUS "Home is $ENV{HOME}")

#set(Live555_ROOT /mnt/hgfs/hsyuan/third-party/live.2018.09.18)
set(Live555_ROOT $ENV{HOME}/third_party/live)

include_directories(${Live555_ROOT}/BasicUsageEnvironment/include
    ${Live555_ROOT}/UsageEnvironment/include
	${Live555_ROOT}/liveMedia/include
	${Live555_ROOT}/groupsock/include
	/usr/include/x86_64-linux-gnu
       )

#link_libraries( ${OpenCV})

link_directories(
    ${Live555_ROOT}/groupsock
    ${Live555_ROOT}/UsageEnvironment
	${Live555_ROOT}/BasicUsageEnvironment
	${Live555_ROOT}/liveMedia
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
	avcodec-ffmpeg
    avformat-ffmpeg
    avutil-ffmpeg
    swscale-ffmpeg
	avfilter-ffmpeg
	avdevice-ffmpeg
    pthread
)


