#ifndef H264_DECODER_H
#define H264_DECODER_H

#include <string>
#include <vector>
#include <memory>
#include "RawFrame.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h> /* av_image_alloc*/
#include <libavutil/avutil.h>
#include <libswscale/swscale.h> /* swsContext */

#ifdef __cplusplus
}
#endif

class H264Decoder
{

public:
    H264Decoder(const std::string &fileName, uint32_t startingFrom = 1, uint32_t numFramesToDecode = UINT32_MAX);

    ~H264Decoder();
    H264Decoder(const H264Decoder &other) = delete;
    H264Decoder &operator=(const H264Decoder &other) = delete;
    bool wasInitializedCorrectly();
    void decode(uint8_t yPositionPercentage);
    const std::vector<std::unique_ptr<RawFrame>> &getFrameCollection() const;

private:
    static const uint16_t INBUF_SIZE = 4096;
    std::string fileName;
    uint32_t startingFrom;
    uint32_t numFramesToDecode;
    uint32_t numFramesDecodedSofar = 0;
    int openCodecresult = -1;
    std::vector<std::unique_ptr<RawFrame>> frameCollection;

    AVCodec *codec = nullptr;
    AVCodecParserContext *parser = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVFrame *frame = nullptr;
    AVPacket *packet = nullptr;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    SwsContext *swsContext = nullptr;

    bool processFrame(uint8_t yPositionPercentage);
    SwsContext *getSWSContext();
};

#endif