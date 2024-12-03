// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#pragma once
#ifndef frameutil_h
#define frameutil_h

#include <frameutil_export.h>
#include <stdint.h>
#include <string>

typedef enum { UDP = 0, TCP = 1 } Protocol;
typedef enum { H_FRAME = 0, I_FRAME = 1, P_FRAME = 2, CONNECT_HEADER = 3, AUDIO_FRAME = 16, UNKNOWN_FRAME } FrameType;
// typedef enum { VIDEO = 0, AUDIO = 1, METADATA_TYPE = 2 } PacketMedia;

typedef enum {
  MJPEG         = 0,
  MJPG          = 0,
  MJPEG_TYPE    = 0,
  MPEG          = 1,
  MPEG_TYPE     = 1,
  H264          = 2,
  H264_TYPE     = 2,
  MLAW          = 3,
  PCMU          = 3,
  ALAW          = 4,
  PCMA          = 4,
  L16           = 5,
  ACC_CODEC     = 6,
  ACC           = 6,
  UNKNOWN       = 7,
  UNKNOWN_CODEC = 6,
  H265          = 8,
  H265_CODEC    = 8
} MediaType;

namespace vtpl::common {
enum NALBoundaryCondition { NO_CHANGE, NAL_ERROR, NAL_BOUNDARY_START_SPS, NAL_BOUNDARY_START_IDR, NAL_BOUNDARY_END };
NALBoundaryCondition FRAMEUTIL_EXPORT findNALBoundaryCondition(uint8_t last_frame_nal_type,
                                                                uint8_t current_frame_nal_type, MediaType media_type);
uint8_t FRAMEUTIL_EXPORT              extractNALType(uint8_t raw_byte, MediaType media_type);

std::string FRAMEUTIL_EXPORT getCodecString(unsigned char frameType);

inline bool IS_AUDIO(int32_t x) {
  return (x == MLAW || x == PCMU || x == ALAW || x == PCMA || x == L16 || x == ACC_CODEC || x == UNKNOWN_CODEC);
}
inline bool IS_VIDEO(int32_t x) { return (x == MJPG || x == H264 || x == H265); }

enum H264Or5NalUnitType {
  NAL_NONE = 0,
};

enum H264NALUnitType {
  H264_NAL_NONE            = H264Or5NalUnitType::NAL_NONE,
  H264_NAL_SLICE           = 1,
  H264_NAL_DPA             = 2,
  H264_NAL_DPB             = 3,
  H264_NAL_DPC             = 4,
  H264_NAL_IDR             = 5, // I-FRAME
  H264_NAL_SEI             = 6,
  H264_NAL_SPS             = 7, // CONNECT-HEADER
  H264_NAL_PPS             = 8, // CONNECT-HEADER
  H264_NAL_AUD             = 9,
  H264_NAL_END_SEQUENCE    = 10,
  H264_NAL_END_STREAM      = 11,
  H264_NAL_FILLER_DATA     = 12,
  H264_NAL_SPS_EXT         = 13,
  H264_NAL_AUXILIARY_SLICE = 19
};
/**
 * NAL unit type codes
 */
enum HEVCNALUnitType {
  HEVC_NAL_NONE       = H264Or5NalUnitType::NAL_NONE,
  HEVC_NAL_TRAIL_N    = 0,
  HEVC_NAL_TRAIL_R    = 1, // P-FRAME
  HEVC_NAL_TSA_N      = 2,
  HEVC_NAL_TSA_R      = 3,
  HEVC_NAL_STSA_N     = 4,
  HEVC_NAL_STSA_R     = 5,
  HEVC_NAL_RADL_N     = 6,
  HEVC_NAL_RADL_R     = 7,
  HEVC_NAL_RASL_N     = 8,
  HEVC_NAL_RASL_R     = 9,
  HEVC_NAL_BLA_W_LP   = 16,
  HEVC_NAL_BLA_W_RADL = 17,
  HEVC_NAL_BLA_N_LP   = 18,
  HEVC_NAL_IDR_W_RADL = 19, // I-FRAME
  HEVC_NAL_IDR_N_LP   = 20,
  HEVC_NAL_CRA_NUT    = 21,
  HEVC_NAL_VPS        = 32, // CONNECT-HEADER
  HEVC_NAL_SPS        = 33, // CONNECT-HEADER
  HEVC_NAL_PPS        = 34, // CONNECT-HEADER
  HEVC_NAL_AUD        = 35,
  HEVC_NAL_EOS_NUT    = 36,
  HEVC_NAL_EOB_NUT    = 37,
  HEVC_NAL_FD_NUT     = 38,
  HEVC_NAL_SEI_PREFIX = 39, // H-FRAME
  HEVC_NAL_SEI_SUFFIX = 40
};

std::string FRAMEUTIL_EXPORT getCodecString(unsigned char frameType);

class FRAMEUTIL_EXPORT NALBoundaryDetector {
public:
  bool isDetected(const uint8_t* data, int32_t media_type);

private:
  uint8_t last_frame_NAL_type_{NAL_NONE};
  uint8_t last_media_type_{0};
};

} // namespace vtpl::common
#endif // frameutil_h
