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

NALBoundaryCondition findNALBoundaryCondition(uint8_t last_frame_nal_type, uint8_t current_frame_nal_type,
                                              MediaType media_type) {
  if (media_type == MediaType::H264) {
    // if (last_frame_nal_type == current_frame_nal_type) {
    //   return NALBoundaryCondition::NO_CHANGE;
    // }
    if ((last_frame_nal_type == H264_NAL_SLICE) && (current_frame_nal_type == H264_NAL_IDR)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_IDR;
    }
    if ((last_frame_nal_type == H264_NAL_SLICE) && (current_frame_nal_type == H264_NAL_SPS)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_SPS;
    }
    return NALBoundaryCondition::NO_CHANGE;
  }
  if (media_type == MediaType::H265) {
    // if (last_frame_nal_type == current_frame_nal_type) {
    //   return NALBoundaryCondition::NO_CHANGE;
    // }
    if ((last_frame_nal_type == HEVC_NAL_TRAIL_R) &&
        ((current_frame_nal_type == HEVC_NAL_IDR_W_RADL) || (current_frame_nal_type == HEVC_NAL_IDR_N_LP))) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_IDR;
    }
    if ((last_frame_nal_type == HEVC_NAL_TRAIL_R) && (current_frame_nal_type == HEVC_NAL_VPS)) {
      return NALBoundaryCondition::NAL_BOUNDARY_START_SPS;
    }
    return NALBoundaryCondition::NO_CHANGE;
  }
  return NALBoundaryCondition::NAL_ERROR;
}

} // namespace vtpl::common