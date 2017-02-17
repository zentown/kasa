/**
 * fileparser_avc.cpp
 *
 * History:
 *  2014/12/08 - [Zhi He] create file
 *
 * Copyright (C) 2014 - 2024, the Ambarella Inc.
 *
 * All rights reserved. No Part of this file may be reproduced, stored
 * in a retrieval system, or transmitted, in any form, or by any means,
 * electronic, mechanical, photocopying, recording, or otherwise,
 * without the prior consent of the Ambarella Inc.
 */

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "lwmd_if.h"
#include "lwmd_log.h"

#include "internal.h"

#include "osal.h"
#include "framework.h"
#include "modules_interface.h"
#include "codec_interface.h"

#include "fileparser_avc.h"

IMediaFileParser *gfCreateBasicAVCParser()
{
    return CAVCFileParser::Create();
}

TU8 *__next_start_code(TU8 *p, TIOSize len, TU32 &prefix_len, TU8 &nal_type)
{
    TUint state = 0;

    while (len) {
        switch (state) {
            case 0:
                if (!(*p)) {
                    state = 1;
                }
                break;

            case 1: //0
                if (!(*p)) {
                    state = 2;
                } else {
                    state = 0;
                }
                break;

            case 2: //0 0
                if (!(*p)) {
                    state = 3;
                } else if (1 == (*p)) {
                    prefix_len = 3;
                    nal_type = p[1] & 0x1f;
                    return (p - 2);
                } else {
                    state = 0;
                }
                break;

            case 3: //0 0 0
                if (!(*p)) {
                    state = 3;
                } else if (1 == (*p)) {
                    prefix_len = 4;
                    nal_type = p[1] & 0x1f;
                    return (p - 3);
                } else {
                    state = 0;
                }
                break;

            default:
                LOG_FATAL("impossible to comes here\n");
                break;

        }
        p++;
        len --;
    }

    return NULL;
}

CAVCFileParser *CAVCFileParser::Create()
{
    CAVCFileParser *result = new CAVCFileParser();
    if ((result) && (EECode_OK != result->Construct())) {
        result->Delete();
        result = NULL;
    }
    return result;
}

void CAVCFileParser::Delete()
{
    if (mpIO) {
        mpIO->Delete();
        mpIO = NULL;
    }

    if (mpReadBuffer) {
        DDBG_FREE(mpReadBuffer, "FPAV");
        mpReadBuffer = NULL;
    }
}

CAVCFileParser::CAVCFileParser()
    : mpIO(NULL)
    , mpReadBuffer(NULL)
    , mReadBufferSize(0)
    , mReadBlockSize(0)
    , mReadNextThreashold(0)
    , mpCurrentPtr(NULL)
    , mBufRemaingSize(0)
    , mFileTotalSize(0)
    , mFileCurrentOffset(0)
    , mbFileOpend(0)
    , mbFileEOF(0)
    , mbParseMediaInfo(0)
    , mbStartReadFile(0)
    , mProfileIndicator(0)
    , mLevelIndicator(0)
    , mVideoWidth(1920)
    , mVideoHeight(1080)
    , mVideoFramerateNum(DDefaultVideoFramerateNum)
    , mVideoFramerateDen(DDefaultVideoFramerateDen)
{
}

CAVCFileParser::~CAVCFileParser()
{

}

EECode CAVCFileParser::Construct()
{
    mReadBlockSize = 20 * 1024 * 1024;
    mReadNextThreashold = 8 * 1024 * 1024;

    mReadBufferSize = mReadBlockSize + mReadNextThreashold;
    mpReadBuffer = (TU8 *) DDBG_MALLOC(mReadBufferSize, "FPAV");
    if (DUnlikely(!mpReadBuffer)) {
        LOG_FATAL("CAVCFileParser::Construct(), no memory, request size %lld\n", mReadBufferSize);
        mReadBufferSize = 0;
        return EECode_NoMemory;
    }

    mpIO = gfCreateIO(EIOType_File);

    if (DUnlikely(!mpIO)) {
        LOG_FATAL("CAVCFileParser::Construct(), gfCreateIO() fail\n");
        return EECode_NoMemory;
    }

    return EECode_OK;
}

EECode CAVCFileParser::OpenFile(TChar *name)
{
    if (DUnlikely(mbFileOpend)) {
        LOG_WARN("close previous file first\n");
        mpIO->Close();
        mbFileOpend = 0;
    }

    EECode err = mpIO->Open(name, EIOFlagBit_Read | EIOFlagBit_Binary);
    if (DUnlikely(EECode_OK != err)) {
        LOG_ERROR("CAVCFileParser::OpenFile(%s) fail, ret %d, %s\n", name, err, gfGetErrorCodeString(err));
        return err;
    }

    mpIO->Query(mFileTotalSize, mFileCurrentOffset);

    mbFileOpend = 1;

    return EECode_OK;
}

EECode CAVCFileParser::CloseFile()
{
    if (DUnlikely(!mbFileOpend)) {
        LOG_WARN("file not opened, or already closed\n");
        return EECode_OK;
    }

    mpIO->Close();
    mbFileOpend = 0;

    return EECode_OK;
}

EECode CAVCFileParser::SeekTo(TIOSize offset)
{
    if (DUnlikely(!mbFileOpend)) {
        LOG_ERROR("file not opened, or already closed\n");
        return EECode_BadState;
    }

    if (DUnlikely(offset >= mFileTotalSize)) {
        LOG_ERROR("CAVCFileParser::SeekTo, offset >= total size\n");
        return EECode_BadParam;
    }

    mpIO->Seek(offset, EIOPosition_Begin);
    mFileCurrentOffset = offset;
    return EECode_OK;
}

EECode CAVCFileParser::ReadPacket(SMediaPacket *packet)
{
    if ((!mbFileEOF) && (mBufRemaingSize < mReadNextThreashold)) {
        readDataFromFile();
    }

    TU8 *p_src = mpCurrentPtr;
    TU8 *p_src_cur = p_src;
    TU8 *p_end = NULL;
    TIOSize processing_size = 0;
    TU32 prefix_len;
    TU8 nal_type = ENalType_unspecified;
    TU32 read_frame = 0;
    //TU32 with_sps = 0;
    TU32 remaining_size = 0;

    packet->sub_packet_number = 0;

    p_src_cur = __next_start_code(p_src, mBufRemaingSize, prefix_len, nal_type);
    if (DUnlikely(!p_src_cur)) {
        LOG_ERROR("ReadPacket() fail, do not find first start code prefix?\n");
        return EECode_BadParam;
    }

    if (DUnlikely(ENalType_SEI > nal_type)) {
        read_frame = 1;
        packet->paket_type = nal_type;
        if (ENalType_IDR == nal_type) {
            packet->frame_type = EPredefinedPictureType_IDR;
        } else {
            packet->frame_type = EPredefinedPictureType_P;
        }
    } else if (DUnlikely(ENalType_SPS == nal_type)) {
        //with_sps = 1;
    }

    processing_size = (TIOSize)(p_src_cur - p_src);
    if (DUnlikely(processing_size)) {
        LOG_WARN("skip data size %lld:\n", processing_size);
        gfPrintMemory(p_src, (TU32) processing_size);

        mBufRemaingSize -= processing_size;
        mpCurrentPtr += processing_size;
        p_src = p_src_cur;
    }

    p_src_cur += prefix_len;
    remaining_size = mBufRemaingSize - prefix_len;

    do {
        p_end = __next_start_code(p_src_cur, remaining_size, prefix_len, nal_type);

        if (DUnlikely(!p_end)) {
            if (mbFileEOF) {
                LOG_NOTICE("last packet\n");
                packet->p_data = p_src;
                packet->data_len = mBufRemaingSize;
                packet->have_pts = 0;
                packet->paket_type = p_src[prefix_len] & 0x1f;
                mpCurrentPtr += mBufRemaingSize;
                mBufRemaingSize = 0;
                return EECode_OK_EOF;
            } else {
                LOG_ERROR("why here?\n");
                return EECode_DataCorruption;
            }
        } else {
            if (read_frame) {
                break;
            }

            if (DUnlikely(ENalType_SEI > nal_type)) {
                read_frame = 1;
                packet->paket_type = nal_type;
                if (ENalType_IDR == nal_type) {
                    packet->frame_type = EPredefinedPictureType_IDR;
                } else {
                    packet->frame_type = EPredefinedPictureType_P;
                }
            } else if (DUnlikely(ENalType_SPS == nal_type)) {
                //with_sps = 1;
            }
            remaining_size -= (p_end + prefix_len - p_src_cur);
            p_src_cur = p_end + prefix_len;
        }

    } while (1);

    packet->p_data = p_src;
    processing_size = (TIOSize)(p_end - p_src);
    packet->data_len = processing_size;
    packet->have_pts = 0;

    mpCurrentPtr += processing_size;
    mBufRemaingSize -= processing_size;

    return EECode_OK;
}

EECode CAVCFileParser::GetMediaInfo(SMediaInfo *info)
{
    if (!mbStartReadFile) {
        readDataFromFile();
    }

    if (!mbParseMediaInfo) {
        LOG_ERROR("can not find extradata, this file maybe not h264 file, or file is corrupted\n");
        return EECode_DataCorruption;
    }

    info->video_width = mVideoWidth;
    info->video_height = mVideoHeight;

    info->video_framerate_num = mVideoFramerateNum;
    info->video_framerate_den = mVideoFramerateDen;

    info->profile_indicator = mProfileIndicator;
    info->level_indicator = mLevelIndicator;

    info->format = StreamFormat_H264;

    return EECode_OK;
}

EECode CAVCFileParser::getMediaInfo()
{
    TU8 *p_extra = NULL;
    TU32 extrasize = 0;

    EECode err = gfGetH264Extradata(mpCurrentPtr, (TU32)mBufRemaingSize, p_extra, extrasize);
    if (DLikely(EECode_OK == err)) {
        SCodecVideoCommon *p_video_parser = gfGetVideoCodecParser(p_extra + 5, extrasize - 5, StreamFormat_H264, err);
        if (DUnlikely(!p_video_parser || EECode_OK != err)) {
            LOG_ERROR("gfGetVideoCodecParser failed, ret %d, %s\n", err, gfGetErrorCodeString(err));
            if (p_video_parser) {
                DDBG_FREE(p_video_parser, "GAVC");
            }
            return EECode_DataCorruption;
        } else {
            mProfileIndicator = p_video_parser->profile_indicator;
            mLevelIndicator = p_video_parser->level_indicator;
            mVideoWidth = p_video_parser->max_width;
            mVideoHeight = p_video_parser->max_height;
            if (p_video_parser->framerate_num && p_video_parser->framerate_den) {
                mVideoFramerateNum = p_video_parser->framerate_num;
                mVideoFramerateDen = p_video_parser->framerate_den;
            }
            LOG_NOTICE("gfGetVideoCodecParser, get video width=%u, height=%u, framerate num %d, den %d, profile indicator %d, level indicator %d\n", mVideoWidth, mVideoHeight, mVideoFramerateNum, mVideoFramerateDen, mProfileIndicator, mLevelIndicator);
        }

        if (p_video_parser) {
            DDBG_FREE(p_video_parser, "GAVC");
        }
    } else {
        LOG_ERROR("do not get extra data?\n");
        return EECode_DataCorruption;
    }

    return EECode_OK;
}

EECode CAVCFileParser::readDataFromFile()
{
    TIOSize read_buf_size = mReadBlockSize;
    memcpy(mpReadBuffer, mpCurrentPtr, mBufRemaingSize);
    mpCurrentPtr = mpReadBuffer;

    EECode err = mpIO->Read(mpReadBuffer + mBufRemaingSize, 1, read_buf_size);
    if (DUnlikely(EECode_OK != err)) {
        LOG_ERROR("CAVCFileParser::ReadDataFromFile(), mpIO->Read() ret %d, %s\n", err, gfGetErrorCodeString(err));
        mbFileEOF = 1;
        return err;
    }

    mbStartReadFile = 1;
    mBufRemaingSize += read_buf_size;

    if (DUnlikely(!mbParseMediaInfo)) {
        mbParseMediaInfo = 1;
        err = getMediaInfo();
    }

    return EECode_OK;
}

