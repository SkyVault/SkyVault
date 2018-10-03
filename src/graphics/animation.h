#ifndef SKYVAULT_ANIMATION_H
#define SKYVAULT_ANIMATION_H

#include <vector>
#include "frame.h"

struct Animation {
    enum Playback {
        Forward,
        Backward,
        Bounce
    };

    enum State {
        Playing,
        Paused
    };

    float PercentDone();

    State GetState();
    Playback GetPlaybackType();

    std::vector<Frame>& GetFrames();
    Frame GetCurrentFrame();
    void Next();

private:
    State state{Playing};
    Playback playback{Forward};
    std::vector<Frame> frames;

    int current_frame_index{0};
};

#endif//SKYVAULT_ANIMATION_H