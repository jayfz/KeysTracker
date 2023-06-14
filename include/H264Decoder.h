#ifndef H264_DECODER_H
#define H264_DECODER_H

// #include "PixelLine.h"
// #include "RawFrame.h"
#include "FrameProcessor.h"
#include <memory>
#include <string>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h> /* av_image_alloc*/
#include <libswscale/swscale.h> /* swsContext */
}

class H264Decoder
{

  public:
    H264Decoder(FrameProcessor *processor, const std::string &fileName,
                uint32_t startingFrom = 0, uint32_t numFramesToDecode = UINT32_MAX);

    ~H264Decoder();
    H264Decoder(const H264Decoder &other) = delete;
    H264Decoder &operator=(const H264Decoder &other) = delete;
    bool wasInitializedCorrectly();
    void decode();

  private:
    static const uint16_t INBUF_SIZE = 4096;
    FrameProcessor *processor;
    std::string fileName;
    uint32_t startingFrom;
    uint32_t numFramesToDecode;
    uint32_t numFramesDecodedSofar = 0;
    int openCodecresult = -1;

    AVCodec *codec = nullptr;
    AVCodecParserContext *parser = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVFrame *frame = nullptr;
    AVPacket *packet = nullptr;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    SwsContext *swsContext = nullptr;

    bool processFrame();
    SwsContext *getSWSContext();
};

#endif