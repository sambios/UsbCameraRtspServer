#include "FfmpegH264LiveServerMediaSubsession.h"
#include "FfmpegFrameSource.h"


ServerMediaSubsession* FfmpegH264LiveServerMediaSubsession::createNew(UsageEnvironment &env, const char *inputType, const char *inputDevice)
{
    return new FfmpegH264LiveServerMediaSubsession(inputType, inputDevice, env);
}

FfmpegH264LiveServerMediaSubsession::FfmpegH264LiveServerMediaSubsession(const char *inputType, 
    const char *inputDevice,
    UsageEnvironment &env,
    portNumBits initialPortNum,
    Boolean multiplexRTCPWithRTP):
    OnDemandServerMediaSubsession(env, true, initialPortNum, multiplexRTCPWithRTP)
{
}


FfmpegH264LiveServerMediaSubsession::~FfmpegH264LiveServerMediaSubsession()
{
}


// Override 
FramedSource* FfmpegH264LiveServerMediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) 
{
    estBitrate = 500; // kbps, estimate
    FFmpegH264Source *ffmpegSource = FFmpegH264Source::createNew(envir());
 
    // Create a framer for the Video Elementary Stream:
    return H264VideoStreamFramer::createNew(envir(), ffmpegSource);
}

// "estBitrate" is the stream's estimated bitrate, in kbps
RTPSink* FfmpegH264LiveServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
    unsigned char rtpPayloadTypeIfDynamic,
    FramedSource* inputSource)
{
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}