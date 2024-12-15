// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#include "frameutil.h"
#include <cstdint>
#include <string>

constexpr int LAST_9_BITS  = 0x1FU;
constexpr int LAST_10_BITS = 0x3FU;

namespace vtpl::common {

namespace {
MediaType convertVideoCodecToMediaType(int32_t media_type) {
  // switch (media_type) {
  // case 2:
  //   return MediaType::H264;
  //   break;
  // case 8:
  //   return MediaType::H265;
  //   break;
  // default:
  //   break;
  // }
  return static_cast<MediaType>(media_type);
}

uint8_t getNALUnitType(const uint8_t* data, int32_t media_type) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  return vtpl::common::extractNALType(*(data + 4), convertVideoCodecToMediaType(media_type));
}

bool findNALBoundary(uint8_t current_frame_nal_type, int32_t media_type, uint8_t last_frame_nal_type) {
  return (vtpl::common::findNALBoundaryCondition(last_frame_nal_type, current_frame_nal_type,
                                                 convertVideoCodecToMediaType(media_type)) ==
          vtpl::common::NALBoundaryCondition::NAL_BOUNDARY_START_SPS);
}
} // namespace

uint8_t extractNALType(uint8_t raw_byte, MediaType media_type) {
  if (MediaType::H264 == media_type) {
    return raw_byte & LAST_9_BITS;
  }
  if (MediaType::H265 == media_type) {
    return (raw_byte >> 1U) & LAST_10_BITS;
  }
  return raw_byte;
}

std::string extractNALTypeString(uint8_t raw_nal_byte, MediaType media_type) {
  if (UNKNOWN == media_type) {
    return "UNKNOWN";
  }
  if (MJPG == media_type) {
    return "MJPEG_NONE";
  }
  if (!IS_VIDEO(media_type)) {
    return "NOT_VIDEO";
  }
  auto nalu = extractNALType(raw_nal_byte, media_type);
  if (H264 == media_type) {
    auto nalu1 = static_cast<H264NALUnitType>(nalu);
    switch (nalu1) {
    case H264_NAL_NONE:
      return "H264_NONE";
      break;
    case H264_NAL_SLICE:
      return "H264_SLICE";
      break;
    case H264_NAL_DPA:
      return "H264_DPA";
      break;
    case H264_NAL_DPB:
      return "H264_DPB";
      break;
    case H264_NAL_DPC:
      return "H264_DPC";
      break;
    case H264_NAL_IDR:
      return "H264_IDR";
      break;
    case H264_NAL_SEI:
      return "H264_SEI";
      break;
    case H264_NAL_SPS:
      return "H264_SPS";
      break;
    case H264_NAL_PPS:
      return "H264_PPS";
      break;
    case H264_NAL_AUD:
      return "H264_AUD";
      break;
    case H264_NAL_END_SEQUENCE:
      return "H264_END_SEQUENCE";
      break;
    case H264_NAL_END_STREAM:
      return "H264_END_STREAM";
      break;
    case H264_NAL_FILLER_DATA:
      return "H264_FILLER_DATA";
      break;
    case H264_NAL_SPS_EXT:
      return "H264_SPS_EXT";
      break;
    case H264_NAL_AUXILIARY_SLICE:
      return "H264_AUXILIARY_SLICE";
      break;
    default:
      return "H264_UNKNOWN";
    }
  }
  if (H265 == media_type) {
    auto nalu1 = static_cast<HEVCNALUnitType>(nalu);
    switch (nalu1) {
    case HEVC_NAL_NONE:
      return "HEVC_NONE";
      break;
    case HEVC_NAL_TRAIL_R:
      return "HEVC_TRAIL_R";
      break;
    case HEVC_NAL_TSA_N:
      return "HEVC_TSA_N";
      break;
    case HEVC_NAL_TSA_R:
      return "HEVC_TSA_R";
      break;
    case HEVC_NAL_STSA_N:
      return "HEVC_STSA_N";
      break;
    case HEVC_NAL_STSA_R:
      return "HEVC_STSA_R";
      break;
    case HEVC_NAL_RADL_N:
      return "HEVC_RADL_N";
      break;
    case HEVC_NAL_RADL_R:
      return "HEVC_RADL_R";
      break;
    case HEVC_NAL_RASL_N:
      return "HEVC_RASL_N";
      break;
    case HEVC_NAL_RASL_R:
      return "HEVC_RASL_R";
      break;
    case HEVC_NAL_BLA_W_LP:
      return "HEVC_BLA_W_LP";
      break;
    case HEVC_NAL_BLA_W_RADL:
      return "HEVC_BLA_W_RADL";
      break;
    case HEVC_NAL_BLA_N_LP:
      return "HEVC_BLA_N_LP";
      break;
    case HEVC_NAL_IDR_W_RADL:
      return "HEVC_IDR_W_RADL";
      break;
    case HEVC_NAL_IDR_N_LP:
      return "HEVC_IDR_N_LP";
      break;
    case HEVC_NAL_CRA_NUT:
      return "HEVC_CRA_NUT";
      break;
    case HEVC_NAL_VPS:
      return "HEVC_VPS";
      break;
    case HEVC_NAL_SPS:
      return "HEVC_SPS";
      break;
    case HEVC_NAL_PPS:
      return "HEVC_PPS";
      break;
    case HEVC_NAL_AUD:
      return "HEVC_AUD";
      break;
    case HEVC_NAL_EOS_NUT:
      return "HEVC_EOS_NUT";
      break;
    case HEVC_NAL_EOB_NUT:
      return "HEVC_EOB_NUT";
      break;
    case HEVC_NAL_FD_NUT:
      return "HEVC_FD_NUT";
      break;
    case HEVC_NAL_SEI_PREFIX:
      return "HEVC_SEI_PREFIX";
      break;
    case HEVC_NAL_SEI_SUFFIX:
      return "HEVC_SEI_SUFFIX";
      break;
    default:
      return "HEVC_UNKNOWN";
    }
  }
  return "UNKNOWN_UNKNOWN";
}

NALBoundaryCondition findNALBoundaryCondition(uint8_t last_frame_nal_type, uint8_t current_frame_nal_type,
                                              MediaType media_type) {
  if (media_type == MediaType::H264) {
    if (((last_frame_nal_type == H264_NAL_SLICE) || (last_frame_nal_type == H264_NAL_NONE)) &&
        (current_frame_nal_type == H264_NAL_IDR)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_IDR;
    }
    if (((last_frame_nal_type == H264_NAL_SLICE) || (last_frame_nal_type == H264_NAL_NONE) ||
         (last_frame_nal_type == H264_NAL_AUD)) &&
        (current_frame_nal_type == H264_NAL_SPS)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_SPS;
    }
    return NALBoundaryCondition::NO_CHANGE;
  }
  if (media_type == MediaType::H265) {
    if (((last_frame_nal_type == HEVC_NAL_TRAIL_R) || (last_frame_nal_type == HEVC_NAL_NONE)) &&
        (current_frame_nal_type == HEVC_NAL_IDR_W_RADL)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_IDR;
    }
    if (((last_frame_nal_type == HEVC_NAL_TRAIL_R) || (last_frame_nal_type == HEVC_NAL_NONE) ||
         (last_frame_nal_type == HEVC_NAL_AUD)) &&
        (current_frame_nal_type == HEVC_NAL_VPS)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_SPS;
    }
    return NALBoundaryCondition::NO_CHANGE;
  }
  return NALBoundaryCondition::NAL_ERROR;
}

std::string getCodecString(unsigned char media_type) {
  switch (media_type) {
  case MJPG:
    return "MJPEG";
  case MPEG:
    return "MPEG";
  case H264:
    return "H264";
  case PCMU:
    return "PCMU";
  case PCMA:
    return "PCMA";
  case L16:
    return "L16";
  case ACC:
    return "ACC";
  case H265:
    return "H265";
  default:
    return "UNKN";
  }
}

bool isNALBoundaryDetected(const uint8_t* data, int32_t media_type) {
  if (!IS_VIDEO(media_type)) {
    return false;
  }

  return findNALBoundary(getNALUnitType(data, media_type), media_type, NAL_NONE);
}

bool NALBoundaryDetector::isDetected(const uint8_t* data, int32_t media_type) {
  if (!IS_VIDEO(media_type)) {
    return false;
  }

  const uint8_t current_frame_nal_type = getNALUnitType(data, media_type);
  if (last_media_type_ != media_type) {
    last_frame_NAL_type_ = NAL_NONE;
  }
  const bool is_nal_boundary_detected = findNALBoundary(current_frame_nal_type, media_type, last_frame_NAL_type_);
  last_frame_NAL_type_                = current_frame_nal_type;
  last_media_type_                    = media_type;
  return is_nal_boundary_detected;
}

} // namespace vtpl::common