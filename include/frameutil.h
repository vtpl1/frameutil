// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#pragma once
#ifndef frameurtil_h
#define frameurtil_h

#include <stdint.h>

typedef enum { UDP = 0, TCP = 1 } Protocol;
typedef enum { H_FRAME = 0, I_FRAME = 1, P_FRAME = 2, CONNECT_HEADER = 3, AUDIO_FRAME = 16, UNKNOWN_FRAME } FrameType;
typedef enum { VIDEO = 0, AUDIO = 1, METADATA_TYPE = 2 } PacketMedia;
typedef enum { MJPG = 0, MPEG = 1, H264 = 2, PCMU = 3, PCMA = 4, L16 = 5, ACC = 6, UNKNOWN = 7, H265 = 8 } MediaType;

namespace vtpl::common {
enum NALBoundaryCondition { NO_CHANGE, NAL_ERROR, NAL_BOUNDARY_START_SPS, NAL_BOUNDARY_START_IDR, NAL_BOUNDARY_END };
NALBoundaryCondition findNALBoundaryCondition(uint8_t last_frame_nal_type, uint8_t current_frame_nal_type,
                                              MediaType media_type);
uint8_t              extractNALType(uint8_t raw_byte, MediaType media_type);

enum H264NALUnitType {
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
} // namespace vtpl::common
#endif // frameurtil_h
