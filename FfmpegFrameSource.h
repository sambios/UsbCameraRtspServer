#pragma once
#include "liveMedia.hh"
#include "CameraDevice.hpp"
#include "FfmpegEncoder.h"
#include <thread>

class FFmpegH264Source : public FramedSource {

    CameraDevice m_capture;
    FfmpegEncoder m_encoder;
    std::thread *m_thread;

public:
    static FFmpegH264Source* createNew(UsageEnvironment& env);
    FFmpegH264Source(UsageEnvironment& env);
    ~FFmpegH264Source();

protected:
    unsigned maxFrameSize() const override;
private:
 
    virtual void doGetNextFrame();
    virtual void doStopGettingFrames();

    void FfmpegGetNextFrame();

private:
    EventTriggerId m_eventTriggerId;

};

