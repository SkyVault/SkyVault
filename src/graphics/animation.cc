#include "animation.h"
#include <iostream>

Animation::Animation(std::vector<Frame> _frames) {
  for (const auto f: _frames)
      this->frames.push_back(f);
}

float Animation::PercentDone() {
    return 100.0f * static_cast<float>(frames.size() / current_frame_index);
}

Animation::State Animation::GetState() { return state; }
Animation::Playback Animation::GetPlaybackType() { return playback; }

std::vector<Frame>& Animation::GetFrames() { return frames; }

Frame Animation::GetCurrentFrame() { 
    if (frames.size() == 0) {
        return Frame{};
    }
    return frames[current_frame_index]; 
}

void Animation::Next() {
    current_frame_index++;
    if (frames.size() != 0)
        current_frame_index %= (int)frames.size();
    else
        current_frame_index = 0;
}
