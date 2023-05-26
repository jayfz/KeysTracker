#include <fstream>
#include <memory>
#include <iostream>
#include <map>
#include "H264Decoder.h"
#include "RawFrame.h"
#include "ManagedMidiFile.h"
#include "Keyboard.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h> /* av_image_alloc*/
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
    // AVPacket *av_packet_alloc();

#ifdef __cplusplus
}
#endif

void markShortNotes(std::vector<MidiKeyboardEvent> &events);

H264Decoder::H264Decoder(const std::string &fileName, uint32_t startingFrom, uint32_t numFramesToDecode) : fileName(fileName), startingFrom(startingFrom), numFramesToDecode(numFramesToDecode)
{
    this->packet = av_packet_alloc();
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);
    this->codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    this->parser = av_parser_init(codec->id);
    this->codecContext = avcodec_alloc_context3(this->codec);
    this->openCodecresult = avcodec_open2(this->codecContext, this->codec, NULL); //< 0
    this->frame = av_frame_alloc();
    this->frameCollection.reserve(7200); // about 4 minutes worth of frames at 30fps. (30*4*60)
}
H264Decoder::~H264Decoder()
{
    if (this->codecContext && this->frame)
        if (this->packet)
        {
            av_packet_free(&this->packet);
            this->packet = nullptr;
            processFrame(); // at the end we need to "flush the decoder", that's done calling processFrame with this->packet as nullptr;
        }

    if (this->parser)
    {
        av_parser_close(this->parser);
        this->parser = nullptr;
    }

    if (this->codecContext)
    {
        avcodec_free_context(&this->codecContext);
        this->codecContext = nullptr;
    }

    if (this->frame)
    {
        av_frame_free(&this->frame);
    }

    if (this->packet)
    {
        av_packet_free(&this->packet);
    }

    if (this->swsContext)
    {
        sws_freeContext(this->swsContext);
    }
}

bool H264Decoder::wasInitializedCorrectly()
{
    bool correct = true;

    if (!codec || !parser || !codecContext || !frame || !packet || openCodecresult < 0)
        correct = false;

    return correct;
}

void H264Decoder::decode(Keyboard &keyboard, ManagedMidiFile &midifile)
{
    std::ifstream inputFile;
    inputFile.open(this->fileName, std::ios_base::binary);
    bool shouldStopDecoding = false;

    if (!inputFile.is_open())
    {
        return; // error
    }

    while (!inputFile.eof())
    {
        inputFile.read(reinterpret_cast<char *>(this->inbuf), H264Decoder::INBUF_SIZE);
        size_t bytesRead = inputFile.gcount();
        uint8_t *inbufAddress = inbuf;

        while (bytesRead > 0)
        {
            int parserResult = av_parser_parse2(this->parser, this->codecContext, &this->packet->data, &this->packet->size,
                                                inbufAddress, bytesRead, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

            if (parserResult < 0)
                return;

            inbufAddress += parserResult;
            bytesRead -= parserResult;

            if (this->packet->size)
            {
                shouldStopDecoding = this->processFrame();
                if (shouldStopDecoding)
                    break;
            }
        }

        if (shouldStopDecoding)
            break;
    }

    // RawFrame::saveBigFormatFrame(this->frameCollection);

    std::vector<MidiKeyboardEvent> events;
    keyboard.generateMidiEvents(this->frameCollection, events);
    markShortNotes(events);

    uint32_t previousTick = 0;

    for (uint32_t i = 0; i < events.size(); i++)
    {
        MidiKeyboardEvent event = events[i];
        uint32_t fixedTick = event.tick;

        // note has been marked as short, so forget about it
        if (event.key == 255)
            continue;

        if (std::abs(static_cast<int32_t>(event.tick) - static_cast<int32_t>(previousTick)) <= 20)
        {
            fixedTick = previousTick;
        }
        else
        {
            previousTick = event.tick;
        }

        if (event.left)
            midifile.addLeftHandEvent(fixedTick, event.key + 12, event.velocity);
        else
            midifile.addRightHandEvent(fixedTick, event.key + 12, event.velocity);
    }

    midifile.save();
}

bool H264Decoder::processFrame()
{

    int sendPacketResult;

    sendPacketResult = avcodec_send_packet(this->codecContext, this->packet);
    if (sendPacketResult < 0)
    {
        return false; // error
    }

    while (sendPacketResult >= 0)
    {
        sendPacketResult = avcodec_receive_frame(this->codecContext, this->frame);
        if (sendPacketResult == AVERROR(EAGAIN) || sendPacketResult == AVERROR_EOF)
            return false;

        if (sendPacketResult < 0)
        {
            return true; // error fprintf(stderr, "Error during decoding\n"); exit(1);
        }

        SwsContext *swsctx = this->getSWSContext();

        if (!swsctx)
            return true;

        uint8_t *dst_data[4] = {nullptr};
        int dst_linesize[4] = {0};

        av_image_alloc(dst_data, dst_linesize, this->frame->width, this->frame->height, AV_PIX_FMT_RGB24, 1);
        sws_scale(swsctx, frame->data, frame->linesize, 0, this->frame->height, dst_data, dst_linesize);

        if ((static_cast<uint32_t>(this->frame->coded_picture_number) >= this->startingFrom) && this->numFramesDecodedSofar < this->numFramesToDecode)
        {
            auto rawFrame = std::make_unique<RawFrame>(this->frame->width, 8, this->frame->coded_picture_number);
            size_t copyStartingFrom = (this->frame->width * 3) * (this->frame->height / 2);
            rawFrame->copyData(dst_data[0], copyStartingFrom);
            this->numFramesDecodedSofar += 1;
            frameCollection.push_back(move(rawFrame));

            std::cout << "Proccessed frame number " << this->numFramesDecodedSofar << std::endl;
            // RawFrame rawFrame(this->frame->width, this->frame->height, this->frame->coded_picture_number);
            // rawFrame.save();
        }
        else if (this->numFramesDecodedSofar == this->numFramesToDecode)
        {
            av_freep(&dst_data[0]);
            return true;
        }

        av_freep(&dst_data[0]);
    }

    return false;
}

SwsContext *H264Decoder::getSWSContext()
{
    // Convert the frame to RGB24 format using libswscale

    if (this->swsContext)
        return this->swsContext;

    enum AVPixelFormat sourcePixelFormat = static_cast<AVPixelFormat>(frame->format); // YUV420P most likely
    enum AVPixelFormat destinationPixelFormat = AV_PIX_FMT_RGB24;

    this->swsContext = sws_getContext(this->frame->width, this->frame->height, sourcePixelFormat, this->frame->width, this->frame->height, destinationPixelFormat, SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (!this->swsContext)
    {
        // std::cerr << "Could not initialize the conversion context." << std::endl;
        return nullptr;
        // exit(1);
    }

    return this->swsContext;
}

void markShortNotes(std::vector<MidiKeyboardEvent> &events)
{
    // key: key
    // value: tick, index,
    std::map<uint8_t, std::tuple<int, int>> temp;

    uint32_t index = 0;
    // for (auto &event : events)
    for (uint32_t i = 0; i < events.size(); i++)
    {
        auto event = events[i];
        auto it = temp.find(event.key);

        // if (event.tick >= 20700 && event.key == 69)
        // {
        //     std::cout << "Something fishy is happening around here " << std::endl;
        // }

        if (it == temp.end())
        {
            temp[event.key] = {event.tick, index};
        }
        else
        {
            if (event.velocity > 0)
            {
                std::cout << "Ok, this should have not happened" << std::endl;
                break;
            }

            int tempTick = std::get<0>(it->second);
            int tempIndex = std::get<1>(it->second);
            if (std::abs(static_cast<int32_t>(tempTick) - static_cast<int32_t>(event.tick) < 2))
            {
                // 255 means to me, ignore these events, both on and off
                events[tempIndex].key = 255;
                event.key = 255;
            }

            temp.erase(event.key);
        }

        index += 1;
    }
}