// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#include "frameutil.h"
#include <cstdint>
#include <string>

constexpr int LAST_9_BITS  = 0x1FU;
constexpr int LAST_10_BITS = 0x3FU;

namespace vtpl::common {

uint8_t extractNALType(uint8_t raw_nal_byte, MediaType media_type) {
  if (MediaType::H264 == media_type) {
    return raw_nal_byte & LAST_9_BITS;
  }
  if (MediaType::H265 == media_type) {
    return (raw_nal_byte >> 1U) & LAST_10_BITS;
  }
  return raw_nal_byte;
}

bool isDataNALType(uint8_t raw_nal_byte, MediaType media_type) {
  if (vtpl::common::MediaType::H264 == media_type) {
    uint8_t nal_type = vtpl::common::extractNALType(raw_nal_byte, media_type);
    if (nal_type == H264_NAL_IDR) {
      return true;
    }
    if ((nal_type == H264_NAL_SPS) ||
        (nal_type == H264_NAL_PPS) /*|| (nal_unti_type == H264_NAL_SEI)*/) { // FIXME(Monotosh)
      return false;
    }
    if (nal_type < H264_NAL_IDR) {
      return true;
    }
    return false;
  }

  if (vtpl::common::MediaType::H265 == media_type) {
    uint8_t nal_type = vtpl::common::extractNALType(raw_nal_byte, media_type);
    if ((nal_type == HEVC_NAL_IDR_W_RADL) || (nal_type == HEVC_NAL_IDR_N_LP)) {
      return true;
    }
    if ((nal_type == HEVC_NAL_SPS) || (nal_type == HEVC_NAL_PPS) || (nal_type == HEVC_NAL_VPS)) {
      return false;
    }
    if (nal_type < 31) {
      return true;
    }
    return false;
  }

  if (vtpl::common::MediaType::MJPG == media_type) {
    return true;
  }
  return false;
}

bool isKeyFrame(uint8_t raw_nal_byte, MediaType media_type) {
  const uint8_t nal_type = extractNALType(raw_nal_byte, media_type);

  bool is_idr_detected = false;

  if (media_type == MediaType::H264) {
    if (nal_type == H264_NAL_IDR) {
      is_idr_detected = true;
    }
  }
  if (media_type == MediaType::H265) {
    if ((nal_type == HEVC_NAL_IDR_W_RADL) || (nal_type == HEVC_NAL_IDR_N_LP)) {
      is_idr_detected = true;
    }
  }
  return is_idr_detected;
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
    case H264_NAL_PREFIX:
      return "H264_PREFIX";
      break;
    case H264_NAL_SUB_SPS:
      return "H264_SUB_SPS";
      break;
    case H264_NAL_DPS:
      return "H264_DPS";
      break;
    case H264_NAL_RESERVED17:
      return "H264_RESERVED17";
      break;
    case H264_NAL_RESERVED18:
      return "H264_RESERVED18";
      break;
    case H264_NAL_AUXILIARY_SLICE:
      return "H264_AUXILIARY_SLICE";
      break;
    case H264_NAL_EXTEN_SLICE:
      return "H264_EXTEN_SLICE";
      break;
    case H264_NAL_DEPTH_EXTEN_SLICE:
      return "H264_DEPTH_EXTEN_SLICE";
      break;
    case H264_NAL_RESERVED22:
      return "H264_RESERVED22";
      break;
    case H264_NAL_RESERVED23:
      return "H264_RESERVED23";
      break;
    case H264_NAL_UNSPECIFIED24:
      return "H264_UNSPECIFIED24";
      break;
    case H264_NAL_UNSPECIFIED25:
      return "H264_UNSPECIFIED25";
      break;
    case H264_NAL_UNSPECIFIED26:
      return "H264_UNSPECIFIED26";
      break;
    case H264_NAL_UNSPECIFIED27:
      return "H264_UNSPECIFIED27";
      break;
    case H264_NAL_UNSPECIFIED28:
      return "H264_UNSPECIFIED28";
      break;
    case H264_NAL_UNSPECIFIED29:
      return "H264_UNSPECIFIED29";
      break;
    case H264_NAL_UNSPECIFIED30:
      return "H264_UNSPECIFIED30";
      break;
    case H264_NAL_UNSPECIFIED31:
      return "H264_UNSPECIFIED31";
      break;
    default:
      return "H264_UNKNOWN";
    }
  }
  if (H265 == media_type) {
    auto nalu1 = static_cast<HEVCNALUnitType>(nalu);
    switch (nalu1) {
    case HEVC_NAL_TRAIL_N:
      return "HEVC_TRAIL_N";
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
    case HEVC_NAL_VCL_N10:
      return "HEVC_VCL_N10";
      break;
    case HEVC_NAL_VCL_R11:
      return "HEVC_VCL_R11";
      break;
    case HEVC_NAL_VCL_N12:
      return "HEVC_VCL_N12";
      break;
    case HEVC_NAL_VCL_R13:
      return "HEVC_VCL_R13";
      break;
    case HEVC_NAL_VCL_N14:
      return "HEVC_VCL_N14";
      break;
    case HEVC_NAL_VCL_R15:
      return "HEVC_VCL_R15";
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
    case HEVC_NAL_RSV_IRAP_VCL22:
      return "HEVC_RSV_IRAP_VCL22";
      break;
    case HEVC_NAL_RSV_IRAP_VCL23:
      return "HEVC_RSV_IRAP_VCL23";
      break;
    case HEVC_NAL_RSV_VCL24:
      return "HEVC_RSV_VCL24";
      break;
    case HEVC_NAL_RSV_VCL25:
      return "HEVC_RSV_VCL25";
      break;
    case HEVC_NAL_RSV_VCL26:
      return "HEVC_RSV_VCL26";
      break;
    case HEVC_NAL_RSV_VCL27:
      return "HEVC_RSV_VCL27";
      break;
    case HEVC_NAL_RSV_VCL28:
      return "HEVC_RSV_VCL28";
      break;
    case HEVC_NAL_RSV_VCL29:
      return "HEVC_RSV_VCL29";
      break;
    case HEVC_NAL_RSV_VCL30:
      return "HEVC_RSV_VCL30";
      break;
    case HEVC_NAL_RSV_VCL31:
      return "HEVC_RSV_VCL31";
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
    case HEVC_NAL_RSV_NVCL41:
      return "HEVC_RSV_NVCL41";
      break;
    case HEVC_NAL_RSV_NVCL42:
      return "HEVC_RSV_NVCL42";
      break;
    case HEVC_NAL_RSV_NVCL43:
      return "HEVC_RSV_NVCL43";
      break;
    case HEVC_NAL_RSV_NVCL44:
      return "HEVC_RSV_NVCL44";
      break;
    case HEVC_NAL_RSV_NVCL45:
      return "HEVC_RSV_NVCL45";
      break;
    case HEVC_NAL_RSV_NVCL46:
      return "HEVC_RSV_NVCL46";
      break;
    case HEVC_NAL_RSV_NVCL47:
      return "HEVC_RSV_NVCL47";
      break;
    case HEVC_NAL_UNSPEC48:
      return "HEVC_UNSPEC48";
      break;
    case HEVC_NAL_UNSPEC49:
      return "HEVC_UNSPEC49";
      break;
    case HEVC_NAL_UNSPEC50:
      return "HEVC_UNSPEC50";
      break;
    case HEVC_NAL_UNSPEC51:
      return "HEVC_UNSPEC51";
      break;
    case HEVC_NAL_UNSPEC52:
      return "HEVC_UNSPEC52";
      break;
    case HEVC_NAL_UNSPEC53:
      return "HEVC_UNSPEC53";
      break;
    case HEVC_NAL_UNSPEC54:
      return "HEVC_UNSPEC54";
      break;
    case HEVC_NAL_UNSPEC55:
      return "HEVC_UNSPEC55";
      break;
    case HEVC_NAL_UNSPEC56:
      return "HEVC_UNSPEC56";
      break;
    case HEVC_NAL_UNSPEC57:
      return "HEVC_UNSPEC57";
      break;
    case HEVC_NAL_UNSPEC58:
      return "HEVC_UNSPEC58";
      break;
    case HEVC_NAL_UNSPEC59:
      return "HEVC_UNSPEC59";
      break;
    case HEVC_NAL_UNSPEC60:
      return "HEVC_UNSPEC60";
      break;
    case HEVC_NAL_UNSPEC61:
      return "HEVC_UNSPEC61";
      break;
    case HEVC_NAL_UNSPEC62:
      return "HEVC_UNSPEC62";
      break;
    case HEVC_NAL_UNSPEC63:
      return "HEVC_UNSPEC63";
      break;
    default:
      return "HEVC_UNKNOWN";
    }
  }
  return "UNKNOWN_UNKNOWN";
}

std::string getCodecString(MediaType media_type) {
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

bool isNALBoundaryDetected(uint8_t raw_nal_byte, MediaType media_type) {

  if (media_type == MediaType::H264) {
    if (extractNALType(raw_nal_byte, media_type) == H264_NAL_SPS) {
      return true;
    }
  }
  if (media_type == MediaType::H265) {
    if (extractNALType(raw_nal_byte, media_type) == HEVC_NAL_VPS) {
      return true;
    }
  }
  if (media_type == MediaType::MJPEG) {
    return true;
  }
  return false;
}

bool IDRBoundaryDetector::isDetected(uint8_t raw_nal_byte, MediaType media_type) {
  if (!IS_VIDEO(media_type)) {
    return false;
  }

  if (last_media_type_ != media_type) {
    last_nal_type_ = NAL_NONE;
  }

  const uint8_t nal_type = extractNALType(raw_nal_byte, media_type);

  bool is_idr_boundary_detected = false;

  if (media_type == MediaType::H264) {
    if (((last_nal_type_ == H264_NAL_SLICE) || (last_nal_type_ == H264_NAL_NONE)) && (nal_type == H264_NAL_IDR)) {
      is_idr_boundary_detected = true;
    }
  }
  if (media_type == MediaType::H265) {
    if (((last_nal_type_ == HEVC_NAL_TRAIL_R) || (last_nal_type_ == HEVC_NAL_NONE)) &&
        ((nal_type == HEVC_NAL_IDR_W_RADL) || (nal_type == HEVC_NAL_IDR_N_LP))) {
      is_idr_boundary_detected = true;
    }
  }

  last_nal_type_   = nal_type;
  last_media_type_ = media_type;

  return is_idr_boundary_detected;
}

} // namespace vtpl::common