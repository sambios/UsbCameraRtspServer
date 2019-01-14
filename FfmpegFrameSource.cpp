#include "FfmpegFrameSource.h"
#include <iostream>

FFmpegH264Source * FFmpegH264Source::createNew(UsageEnvironment& env) {
    return new FFmpegH264Source(env);
}

FFmpegH264Source::FFmpegH264Source(UsageEnvironment& env) : FramedSource(env)
{
    OutPacketBuffer::maxSize = 1048576; // 1<<20 
}

FFmpegH264Source::~FFmpegH264Source()
{
    std::cout << "FfmpegH264Source dtor.." << std::endl;
}

void FFmpegH264Source::doStopGettingFrames()
{
    std::cout << "doStopGettingFrames.." << std::endl;
    FramedSource::doStopGettingFrames();
}


void FFmpegH264Source::doGetNextFrame()
{
    FfmpegGetNextFrame();
}

 unsigned FFmpegH264Source::maxFrameSize() const
{
    return 300 * 1024; 
}

void FFmpegH264Source::FfmpegGetNextFrame()
{
    //printf("FfmpegGetNextFrame...\n");
    if (!isCurrentlyAwaitingData()) return; // we're not ready for the data yet
    
    if (!m_capture.IsReady()) {
#ifdef __linux__
        m_capture.Open("video4linux2", "/dev/video0", 640, 480, 30);
#elif __APPLE__
        m_capture.Open("avfoundation", "0", 640, 480, 30);
#else
        m_capture.Open("dshow", "video=Integrated Camera", 640, 480, 30);
#endif 
        m_encoder.CreateEncoder(640, 480);
    }

    AVFrame *rawFrame = m_capture.Capture();
    if (rawFrame == nullptr) {
        std::cout << "Capture failed, frame is null!" << std::endl;
        return;
    }
    
    AVPacket pkt;
    memset(&pkt, 0,sizeof(pkt));
    av_init_packet(&pkt);
    int ret = m_encoder.Encode(rawFrame, &pkt);
    if (ret < 0) {
        fFrameSize = 0;
    }
    else {
        
        u_int8_t* newFrameDataStart = pkt.data; 
        unsigned newFrameSize = pkt.size;

        // Deliver the data here:
        if (newFrameSize > fMaxSize) {
            fFrameSize = fMaxSize;
            fNumTruncatedBytes = newFrameSize - fMaxSize;
        }
        else {
            fFrameSize = newFrameSize;
        }

        memmove(fTo, newFrameDataStart, fFrameSize);

        av_packet_unref(&pkt);
        fPresentationTime.tv_sec = av_gettime_relative() / 1000000;
        fPresentationTime.tv_usec = 1000 * av_gettime_relative();
    }

    av_frame_unref(rawFrame);
    FramedSource::afterGetting(this);

}
