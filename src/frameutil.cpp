// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#include "frameutil.h"
#include <cstdint>
constexpr int LAST_9_BITS  = 0x1FU;
constexpr int LAST_10_BITS = 0x3FU;

namespace vtpl::common {

uint8_t extractNALType(uint8_t raw_byte, MediaType media_type) {
  if (MediaType::H264 == media_type) {
    return raw_byte & LAST_9_BITS;
  }
  if (MediaType::H265 == media_type) {
    return (raw_byte >> 1U) & LAST_10_BITS;
  }
  return raw_byte;
}

NALBoundaryCondition findNALBoundaryCondition(uint8_t lastFrameNALType, uint8_t currentFrameNALType,
                                              MediaType mediaType) {
  if (mediaType == MediaType::H264) {
    if (((lastFrameNALType == H264_NAL_SLICE) || (lastFrameNALType == H264_NAL_NONE)) &&
        (currentFrameNALType == H264_NAL_IDR)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_IDR;
    }
    if (((lastFrameNALType == H264_NAL_SLICE) || (lastFrameNALType == H264_NAL_NONE) ||
         (lastFrameNALType == H264_NAL_AUD)) &&
        (currentFrameNALType == H264_NAL_SPS)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_SPS;
    }
    return NALBoundaryCondition::NO_CHANGE;
  }
  if (mediaType == MediaType::H265) {
    if (((lastFrameNALType == HEVC_NAL_TRAIL_R) || (lastFrameNALType == HEVC_NAL_NONE)) &&
        (currentFrameNALType == HEVC_NAL_IDR_W_RADL)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_IDR;
    }
    if (((lastFrameNALType == HEVC_NAL_TRAIL_R) || (lastFrameNALType == HEVC_NAL_NONE) ||
         (lastFrameNALType == HEVC_NAL_AUD)) &&
        (currentFrameNALType == HEVC_NAL_VPS)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_SPS;
    }
    return NALBoundaryCondition::NO_CHANGE;
  }
  return NALBoundaryCondition::NAL_ERROR;
}

MediaType convertVideoCodecToMediaType(int32_t media_type) {
  switch (media_type) {
  case 2:
    return MediaType::H264;
    break;
  case 8:
    return MediaType::H265;
    break;
  default:
    break;
  }
  return MediaType::UNKNOWN;
}

uint8_t getNALUnitType(const uint8_t* data, int32_t media_type) {
  return vtpl::common::extractNALType(*(data + 4), convertVideoCodecToMediaType(media_type));
}

bool findNALBoundary(uint8_t currentFrameNALType, int32_t media_type, uint8_t lastFrameNALType) {
  return (vtpl::common::findNALBoundaryCondition(lastFrameNALType, currentFrameNALType,
                                                 convertVideoCodecToMediaType(media_type)) ==
          vtpl::common::NALBoundaryCondition::NAL_BOUNDARY_START_SPS);
}

std::string getCodecString(unsigned char frameType) {
  switch (frameType) {
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

  const uint8_t currentNALUnitType = getNALUnitType(data, media_type);
  if (last_media_type_ != media_type) {
    last_frame_NAL_type_ = NAL_NONE;
  }
  const bool is_nal_boundary_detected = findNALBoundary(currentNALUnitType, media_type, last_frame_NAL_type_);
  last_frame_NAL_type_                = currentNALUnitType;
  last_media_type_                    = media_type;
  return is_nal_boundary_detected;
}
} // namespace vtpl::common