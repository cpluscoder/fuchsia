// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "apps/media/src/ffmpeg/ffmpeg_video_decoder.h"

#include <algorithm>

#include "apps/media/lib/timeline/timeline.h"
#include "apps/media/lib/timeline/timeline_rate.h"
#include "apps/media/src/ffmpeg/ffmpeg_formatting.h"
#include "lib/ftl/logging.h"
extern "C" {
#include "third_party/ffmpeg/libavutil/imgutils.h"
}

namespace media {

FfmpegVideoDecoder::FfmpegVideoDecoder(AvCodecContextPtr av_codec_context)
    : FfmpegDecoderBase(std::move(av_codec_context)) {
  FTL_DCHECK(context());

  // Turn on multi-proc decoding by allowing the decoder to use three threads
  // (the calling thread and the two specified here). FF_THREAD_FRAME means
  // that threads are assigned an entire frame.
  // TODO(dalesat): Consider using FF_THREAD_SLICE.
  context()->thread_count = 2;
  context()->thread_type = FF_THREAD_FRAME;

  // Determine the frame layout we will use.
  frame_buffer_size_ = LayoutFrame(
      PixelFormatFromAVPixelFormat(context()->pix_fmt),
      VideoStreamType::Extent(context()->coded_width, context()->coded_height),
      &line_stride_, &plane_offset_);
}

FfmpegVideoDecoder::~FfmpegVideoDecoder() {}

void FfmpegVideoDecoder::OnNewInputPacket(const PacketPtr& packet) {
  FTL_DCHECK(context());
  FTL_DCHECK(packet->pts() != Packet::kUnknownPts);

  if (pts_rate() == TimelineRate::Zero) {
    set_pts_rate(packet->pts_rate());
  }

  // We put the pts here so it can be recovered later in CreateOutputPacket.
  // Ffmpeg deals with the frame ordering issues.
  context()->reordered_opaque = packet->pts();
}

int FfmpegVideoDecoder::BuildAVFrame(const AVCodecContext& av_codec_context,
                                     AVFrame* av_frame,
                                     PayloadAllocator* allocator) {
  FTL_DCHECK(av_frame);
  FTL_DCHECK(allocator);

  Extent visible_size(av_codec_context.width, av_codec_context.height);
  const int result =
      av_image_check_size(visible_size.width(), visible_size.height(), 0, NULL);
  if (result < 0) {
    return result;
  }

  // FFmpeg has specific requirements on the allocation size of the frame.  The
  // following logic replicates FFmpeg's allocation strategy to ensure buffers
  // are not overread / overwritten.  See ff_init_buffer_info() for details.

  // When lowres is non-zero, dimensions should be divided by 2^(lowres), but
  // since we don't use this, just FTL_DCHECK that it's zero.
  FTL_DCHECK(av_codec_context.lowres == 0);
  Extent coded_size(
      std::max(visible_size.width(),
               static_cast<uint32_t>(av_codec_context.coded_width)),
      std::max(visible_size.height(),
               static_cast<uint32_t>(av_codec_context.coded_height)));

  uint8_t* buffer = static_cast<uint8_t*>(
      allocator->AllocatePayloadBuffer(frame_buffer_size_));

  // TODO(dalesat): For investigation purposes only...remove one day.
  if (first_frame_) {
    first_frame_ = false;
    colorspace_ = av_codec_context.colorspace;
    coded_size_ = coded_size;
  } else {
    if (av_codec_context.colorspace != colorspace_) {
      FTL_LOG(WARNING) << " colorspace changed to "
                       << av_codec_context.colorspace << std::endl;
    }
    if (coded_size.width() != coded_size_.width()) {
      FTL_LOG(WARNING) << " coded_size width changed to " << coded_size.width()
                       << std::endl;
    }
    if (coded_size.height() != coded_size_.height()) {
      FTL_LOG(WARNING) << " coded_size height changed to "
                       << coded_size.height() << std::endl;
    }
    colorspace_ = av_codec_context.colorspace;
    coded_size_ = coded_size;
  }

  if (buffer == nullptr) {
    FTL_LOG(ERROR) << "failed to allocate buffer of size "
                   << frame_buffer_size_;
    return -1;
  }

  // Decoders require a zeroed buffer.
  std::memset(buffer, 0, frame_buffer_size_);

  FTL_DCHECK(line_stride_.size() == plane_offset_.size());

  for (size_t plane = 0; plane < plane_offset_.size(); ++plane) {
    av_frame->data[plane] = buffer + plane_offset_[plane];
    av_frame->linesize[plane] = line_stride_[plane];
  }

  // TODO(dalesat): Do we need to attach colorspace info to the packet?

  av_frame->width = coded_size.width();
  av_frame->height = coded_size.height();
  av_frame->format = av_codec_context.pix_fmt;
  av_frame->reordered_opaque = av_codec_context.reordered_opaque;

  FTL_DCHECK(av_frame->data[0] == buffer);
  av_frame->buf[0] = CreateAVBuffer(buffer, frame_buffer_size_, allocator);

  return 0;
}

PacketPtr FfmpegVideoDecoder::CreateOutputPacket(const AVFrame& av_frame,
                                                 PayloadAllocator* allocator) {
  FTL_DCHECK(allocator);

  // Recover the pts deposited in Decode.
  set_next_pts(av_frame.reordered_opaque);

  return DecoderPacket::Create(av_frame.reordered_opaque, pts_rate(),
                               av_frame.key_frame,
                               av_buffer_ref(av_frame.buf[0]));
}

}  // namespace media
