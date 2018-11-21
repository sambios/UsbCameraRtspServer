/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 1996-2018, Live Networks, Inc.  All rights reserved
// A test program that demonstrates how to stream - via unicast RTP
// - various kinds of file on demand, using a built-in RTSP server.
// main program

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "UsbCameraOnDemandRTSPServer.h"
#include "FfmpegH264LiveServerMediaSubsession.h"


UsbCameraOnDemandRTSPServer::UsbCameraOnDemandRTSPServer()
{

}

UsbCameraOnDemandRTSPServer::~UsbCameraOnDemandRTSPServer()
{

}

void UsbCameraOnDemandRTSPServer::SetH264File(const std::string &filename)
{
    m_h264file = filename;
}

int UsbCameraOnDemandRTSPServer::StartServer() {

  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  m_env = BasicUsageEnvironment::createNew(*scheduler);

  UserAuthenticationDatabase* authDB = NULL;
#ifdef ACCESS_CONTROL
  // To implement client access control to the RTSP server, do the following:
  authDB = new UserAuthenticationDatabase;
  authDB->addUserRecord("username1", "password1"); // replace these with real strings
  // Repeat the above with each <username>, <password> that you wish to allow
  // access to the server.
#endif

  // Create the RTSP server:
  m_rtspServer = RTSPServer::createNew(*m_env, 8554, authDB);
  if (m_rtspServer == NULL) {
    env() << "Failed to create RTSP server: " << env().getResultMsg() << "\n";
    exit(1);
  }

  char const* descriptionString
    = "Session streamed by \"testOnDemandRTSPServer\"";

  OutPacketBuffer::maxSize= 2<<20;

  // Set up each of the possible streams that can be served by the
  // RTSP server.  Each such stream is implemented using a
  // "ServerMediaSession" object, plus one or more
  // "ServerMediaSubsession" objects for each audio/video substream.

  // A H.264 video elementary stream:
  if (!m_h264file.empty()) {
    char const* streamName = "channel=0";
    char const* inputFileName = m_h264file.c_str();
    ServerMediaSession* sms
      = ServerMediaSession::createNew(env(), streamName, streamName,
				      descriptionString);
    sms->addSubsession(H264VideoFileServerMediaSubsession
		       ::createNew(env(), inputFileName, true));
    m_rtspServer->addServerMediaSession(sms);

    announceStream(m_rtspServer, sms, streamName, inputFileName);
  }

  //
#ifdef _WIN32
  {
      char const* streamName = "live";
      char const* input_type = "dshow";
      char const* input_device = "video=Integrated Camera";

      ServerMediaSession* sms = ServerMediaSession::createNew(env(), streamName, streamName, descriptionString);
      sms->addSubsession(FfmpegH264LiveServerMediaSubsession::createNew(env(), input_type, input_device));
      m_rtspServer->addServerMediaSession(sms);

      announceStream(m_rtspServer, sms, streamName, input_device);
  }
#endif 

  env().taskScheduler().doEventLoop(); // does not return

  return 0; // only to prevent compiler warning
}

void UsbCameraOnDemandRTSPServer::announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName, char const* inputFileName) {
  char* url = rtspServer->rtspURL(sms);
  UsageEnvironment& env = rtspServer->envir();
  env << "\n\"" << streamName << "\" stream, from the file \""
      << inputFileName << "\"\n";
  env << "Play this stream using the URL \"" << url << "\"\n";
  delete[] url;
}
