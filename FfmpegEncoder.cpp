#include "FfmpegEncoder.h"
#include <assert.h>


FfmpegEncoder::FfmpegEncoder()
{
}

int FfmpegEncoder::CreateEncoder(int width, int height)
{
    int ret = 0;
    m_h264Codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    assert(m_h264Codec != nullptr);

    m_h264Enc = avcodec_alloc_context3(m_h264Codec);
    assert(m_h264Enc != nullptr);

    /* put sample parameters */
    m_h264Enc->bit_rate = 512 << 10;
    /* resolution must be a multiple of two */
    m_h264Enc->width = width;
    m_h264Enc->height = height;

    /* frames per second */
    m_h264Enc->time_base.den = 25;
    m_h264Enc->time_base.num = 1;
    m_h264Enc->framerate.num = 25;
    m_h264Enc->framerate.den = 1;

    m_h264Enc->gop_size = 25; /* emit one intra frame every ten frames */
    m_h264Enc->max_b_frames = 0;
    m_h264Enc->pix_fmt = (enum AVPixelFormat) AV_PIX_FMT_YUV420P;
    //m_h264Enc->pix_fmt = (enum AVPixelFormat) AV_PIX_FMT_YUV422P;
    m_h264Enc->qmax = 2;
    m_h264Enc->qmin = 32;
    m_h264Enc->delay = 0;

    AVDictionary *options = NULL;
    av_dict_set(&options, "preset", "medium", 0);
    av_dict_set(&options, "tune", "zerolatency", 0);
    av_dict_set(&options, "profile", "baseline", 0);


    ret = avcodec_open2(m_h264Enc, m_h264Codec, NULL);
    assert(ret == 0);

    return 0;
}

int FfmpegEncoder::Encode(AVFrame *src, AVPacket *pkt)
{
    int ret;
    int got_packet = 0;

    if (src->format != AV_PIX_FMT_YUV420P) {
        if (m_swsContex == nullptr) {
            m_swsContex = sws_getContext(src->width, src->height, (enum AVPixelFormat)src->format, src->width, src->height, AV_PIX_FMT_YUV420P, 0, NULL, NULL, NULL);
        }
        
        AVFrame dst;
        memset(&dst, 0, sizeof(dst));
        dst.width = src->width;
        dst.height = src->height;
        dst.format = AV_PIX_FMT_YUV422P;
        dst.pts = av_gettime_relative();

        av_image_alloc(dst.data, dst.linesize, src->width, src->height, AV_PIX_FMT_YUV420P, 16);
        sws_scale(m_swsContex, src->data, src->linesize, 0, src->height, dst.data, dst.linesize);

        ret = avcodec_encode_video2(m_h264Enc, pkt, &dst, &got_packet);
        if (ret < 0) {
            printf("avcodec_encode_video2 err\n");
        }
        av_freep(&dst.data[0]);
    }
    else {
        ret = avcodec_encode_video2(m_h264Enc, pkt, src, &got_packet);
        if (ret < 0) {
            printf("avcodec_encode_video2 err\n");
        }
    }

   

    if (!got_packet) {
        return -1;
    }

    return 0;

}

FfmpegEncoder::~FfmpegEncoder()
{
    
}
