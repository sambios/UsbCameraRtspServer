#pragma once
#include "liveMedia.hh"
#include <iostream>

class UsbCameraOnDemandRTSPServer
{
    RTSPServer* m_rtspServer;
    UsageEnvironment* m_env;
    std::string m_h264file;

    UsageEnvironment &env() {
        return *m_env;
    }

    void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
        char const* streamName, char const* inputFileName); // fwd

public:
    UsbCameraOnDemandRTSPServer();
    virtual ~UsbCameraOnDemandRTSPServer();
    void SetH264File(const std::string &filename);

    int StartServer();
    int StopServer();



};