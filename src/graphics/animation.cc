#include "animation.h"

float Animation::PercentDone() {

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
    current_frame_index %= frames.size();
}
