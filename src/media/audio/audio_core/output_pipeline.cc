// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/media/audio/audio_core/output_pipeline.h"

#include "src/media/audio/audio_core/effects_stage.h"
#include "src/media/audio/audio_core/ring_buffer.h"
#include "src/media/audio/audio_core/tap_stage.h"
#include "src/media/audio/audio_core/usage_settings.h"

namespace media::audio {
namespace {

std::vector<fuchsia::media::Usage> UsagesFromRenderUsages(
    const std::vector<fuchsia::media::AudioRenderUsage>& render_usages) {
  std::vector<fuchsia::media::Usage> usages;

  std::transform(
      render_usages.cbegin(), render_usages.cend(), std::back_inserter(usages),
      [](auto usage) { return fuchsia::media::Usage::WithRenderUsage(fidl::Clone(usage)); });
  return usages;
}

}  // namespace

OutputPipeline::OutputPipeline(const PipelineConfig& config, const Format& output_format,
                               uint32_t max_block_size_frames,
                               TimelineFunction ref_clock_to_fractional_frame)
    : Stream(output_format) {
  uint32_t usage_mask = 0;
  stream_ = CreateMixStage(
      config.root(), output_format, max_block_size_frames,
      fbl::MakeRefCounted<VersionedTimelineFunction>(ref_clock_to_fractional_frame), &usage_mask);
  static constexpr uint32_t kAllUsages =
      1 << static_cast<uint32_t>(fuchsia::media::AudioRenderUsage::BACKGROUND) |
      1 << static_cast<uint32_t>(fuchsia::media::AudioRenderUsage::MEDIA) |
      1 << static_cast<uint32_t>(fuchsia::media::AudioRenderUsage::INTERRUPTION) |
      1 << static_cast<uint32_t>(fuchsia::media::AudioRenderUsage::SYSTEM_AGENT) |
      1 << static_cast<uint32_t>(fuchsia::media::AudioRenderUsage::COMMUNICATION);
  FX_CHECK(usage_mask == kAllUsages)
      << "Pipeline does not cover all usages (" << usage_mask << " vs " << kAllUsages << ")";
}

std::shared_ptr<Mixer> OutputPipeline::AddInput(std::shared_ptr<Stream> stream,
                                                const fuchsia::media::Usage& usage) {
  TRACE_DURATION("audio", "OutputPipeline::AddInput", "stream", stream.get());
  streams_.emplace_back(stream, fidl::Clone(usage));
  return LookupStageForUsage(usage).AddInput(std::move(stream));
}

void OutputPipeline::RemoveInput(const Stream& stream) {
  TRACE_DURATION("audio", "OutputPipeline::RemoveInput", "stream", &stream);
  auto it = std::find_if(streams_.begin(), streams_.end(),
                         [&stream](auto& pair) { return pair.first.get() == &stream; });
  FX_CHECK(it != streams_.end());
  LookupStageForUsage(it->second).RemoveInput(stream);
  streams_.erase(it);
}

std::shared_ptr<Stream> OutputPipeline::CreateMixStage(
    const PipelineConfig::MixGroup& spec, const Format& output_format,
    uint32_t max_block_size_frames,
    fbl::RefPtr<VersionedTimelineFunction> ref_clock_to_fractional_frame, uint32_t* usage_mask) {
  auto stage = std::make_shared<MixStage>(output_format, max_block_size_frames,
                                          ref_clock_to_fractional_frame);
  for (const auto& usage : spec.input_streams) {
    auto mask = 1 << static_cast<uint32_t>(usage);
    FX_CHECK((*usage_mask & mask) == 0);
    *usage_mask |= mask;
  }

  // If we have effects, we should add that stage in now.
  std::shared_ptr<Stream> root = stage;
  if (!spec.effects.empty()) {
    auto effects_stage = EffectsStage::Create(spec.effects, root);
    FX_DCHECK(effects_stage);
    if (effects_stage) {
      root = std::move(effects_stage);
    }
  }

  // If this is the loopback stage, allocate the loopback ring buffer. Note we want this to be
  // after any effects that may have been applied.
  if (spec.loopback) {
    FX_DCHECK(!loopback_) << "Only a single loopback point is allowed.";
    const uint32_t ring_size = output_format.frames_per_second();
    auto endpoints =
        RingBuffer::AllocateSoftwareBuffer(output_format, ref_clock_to_fractional_frame, ring_size);
    loopback_ = std::move(endpoints.reader);
    root = std::make_shared<TapStage>(std::move(root), std::move(endpoints.writer));
  }

  std::vector<fuchsia::media::Usage> usages = UsagesFromRenderUsages(spec.input_streams);
  mix_stages_.emplace_back(stage, UsagesFromRenderUsages(spec.input_streams));
  for (const auto& input : spec.inputs) {
    auto substage = CreateMixStage(input, output_format, max_block_size_frames,
                                   ref_clock_to_fractional_frame, usage_mask);
    stage->AddInput(substage);
  }
  return root;
}

MixStage& OutputPipeline::LookupStageForUsage(const fuchsia::media::Usage& usage) {
  for (auto& [mix_stage, stage_usages] : mix_stages_) {
    for (const auto& stage_usage : stage_usages) {
      if (fidl::Equals(stage_usage, usage)) {
        return *mix_stage;
      }
    }
  }
  FX_CHECK(false);
  __UNREACHABLE;
}

}  // namespace media::audio
