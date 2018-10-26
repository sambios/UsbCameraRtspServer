#pragma once
#include "liveMedia.hh"

class FfmpegH264LiveServerMediaSubsession:OnDemandServerMediaSubsession
{
public:
    static ServerMediaSubsession* createNew(UsageEnvironment &env, const char *inputType, const char *inputDevice);
protected:
    FfmpegH264LiveServerMediaSubsession(const char *inputType, const char *inputDevice, UsageEnvironment &env,
        portNumBits initialPortNum = 6970,
        Boolean multiplexRTCPWithRTP = False);
    ~FfmpegH264LiveServerMediaSubsession();


protected:
    // Override 
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
        unsigned& estBitrate) override;

    // "estBitrate" is the stream's estimated bitrate, in kbps
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
        unsigned char rtpPayloadTypeIfDynamic,
        FramedSource* inputSource) override;

};

