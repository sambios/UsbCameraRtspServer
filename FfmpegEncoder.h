#pragma once


#pragma once

#ifdef __cplusplus
extern "C" {
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
#include <libavutil/time.h>

}

#endif 


class FfmpegEncoder
{
    AVCodec *m_h264Codec;
    AVCodecContext *m_h264Enc;
    SwsContext *m_swsContex = nullptr;

public:
    FfmpegEncoder();
    int CreateEncoder(int width, int height);
    int Encode(AVFrame *src, AVPacket *pkt);
    ~FfmpegEncoder();
};

